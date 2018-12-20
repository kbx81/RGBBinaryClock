//
// kbx81's binary clock Hardware class
// ---------------------------------------------------------------------------
// (c)2017 by kbx81. See LICENSE for details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//

// #include <cstddef>
#include <cstdint>
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/tsc.h>
#include <libopencm3/stm32/usart.h>

#include "Application.h"
#include "DateTime.h"
#include "Display.h"
#include "DisplayManager.h"
#include "Dmx-512-Rx.h"
#include "DS1722.h"
#include "DS3231.h"
#include "DS3234.h"
#include "Hardware.h"
#include "LM74.h"
#include "LM75.h"
#include "MCP9808.h"
#include "RgbLed.h"
#include "TLC59xx.h"

#if HARDWARE_VERSION == 1
  #include "Hardware_v1.h"
#elif HARDWARE_VERSION == 2
  #include "Hardware_v2.h"
#elif HARDWARE_VERSION == 3
  #include "Hardware_v3.h"
#elif HARDWARE_VERSION == 4
  #include "Hardware_v4.h"
#else
  #error HARDWARE_VERSION must be defined with a value between 1 and 4
#endif


namespace kbxBinaryClock {

namespace Hardware {


  // Type of temperature sensor connected, if any
  //
  enum TempSensorType : uint8_t {
    None,
    DS3231,
    DS3234,
    DS1722,
    LM74,
    LM75,
    MCP9808
  };

  // I2C1 state
  //
  enum I2cState : uint8_t {
    I2cIdle,
    I2cBusy
  };

  // SPI1 state
  //
  enum SpiState : uint8_t {
    SpiIdle,
    SpiBusyPeripheral,
    SpiBusyDisplay
  };

  // LED driver shift register state
  //
  enum DriverShiftRegState : uint8_t {
    SrAvailable,
    SrLoadingGsData,
    SrLoadingOtherData,
    SrContainsGsData,
    SrContainsOtherData
  };


// USART baud rates
//
static const uint32_t cUsart1BaudRate = 115200;
static const uint32_t cUsart2BaudRate = 250000;

// number of ADC channels we're using
//
static const uint8_t cAdcChannelCount = 4;

// indexes of ADC channels we're DMA'ing into our buffer
//
static const uint8_t cAdcPhototransistor = 0;
static const uint8_t cAdcTemperature = 1;
static const uint8_t cAdcVref = 2;
static const uint8_t cAdcVbat = 3;

// how frequently we will sample things
//
static const uint16_t cAdcSampleInterval = 500;

// number of samples averaged for light level and temp
//
static const uint8_t cAdcSamplesToAverage = 16;

// value used to increase precision in our maths
//
static const int16_t cBaseMultiplier = 1000;

// the last page number in the flash memory
//
static const uint32_t cFlashPageNumberMaximum = 63;

// the last page number in the flash memory
//
static const uint32_t cFlashPageSize = 0x800;

// maximum time we'll wait for an ACK on I2C
//
static const uint16_t cI2cTimeout = 5000;

// maximum number of times we'll fail requests because I2C is busy
//
static const uint8_t cI2cMaxFailBusyCount = 20;

// value by which we multiply the fractional part of temperature sensor readings
//
static const int16_t cTempFracMultiplier = 125;

// minimum frequency allowed for tones
//
static const uint8_t cToneFrequencyMinimum = 92;

// maximum volume level allowed for tones
//
static const uint8_t cToneVolumeMaximum = 7;

// maximum minimum allowed to consider a key pressed/touched
//
static const uint16_t cTscMaximumMinimum = 1536;

// minimum required difference between min and max readings
//
static const uint16_t cTscMinimumDelta = 400;

// number of samples averaged for sensors
//
static const uint8_t cTscSamplesToAverage = 32;

// calibration voltage used (3.3 volts)
//
static const int32_t cVddCalibrationVoltage = 3300;


// tracks which array element the next sample is to be written to
//
static uint8_t _adcSampleCounter = 0;

// tracks when to take a sample
//
static uint16_t _adcSampleTimer = 0;

// samples averaged for light
//
static uint16_t _adcSampleSetLight[cAdcSamplesToAverage];

// samples averaged for temperature
//
static uint16_t _adcSampleSetTemp[cAdcSamplesToAverage];

// samples averaged for voltage
//
static uint16_t _adcSampleSetVoltage[cAdcSamplesToAverage];

// tracks what's going on with the LED drivers' shift registers
//
volatile static DriverShiftRegState _ledDriverSrState = DriverShiftRegState::SrAvailable;

// threshold above which we do not BLANK during display refreshes
//  used when autoAdjustIntensities is true
static uint16_t _blankingThreshold = 300;

// counter used by delay()
//
volatile static uint32_t _delayCounter = 0;

// a place for data from the ADC to live
//
static uint16_t _bufferADC[cAdcChannelCount];

// tracks how many times tx/rx requests were blocked because the I2C is busy
//
static uint8_t _i2cBusyFailCount = 0;

// device address I2C must use to resume communication
//
volatile static uint8_t _i2cAddr;

// buffer used when I2C resumes communication
//
static uint8_t *_i2cBufferRx;

// number of bytes to transfer when I2C resumes communication
//
volatile static size_t _i2cNumberRx;

// tracks what the I2C is doing
//
volatile static I2cState _i2cState = I2cState::I2cIdle;

// tracks what the SPI is doing
//
// tracks what the SPI MISO is set to
//
static SpiPeripheral _spiActivePeripheral = SpiPeripheral::NotInitialized;

// tracks what the SPI is doing
//
volatile static SpiState _spiState = SpiState::SpiIdle;

// tracks when to silence a tone
//
static uint16_t _toneTimer = 0;

// contains the next tone's duration and frequency, if any (not zero)
//
static uint16_t _toneTimerNext = 0;
static uint16_t _toneFrequencyNext = 0;

// volume level for tones
//
static uint8_t _toneVolume = 0;

// tracks which array element the next sample is to be written to
//
static uint8_t _tscSampleCounter = 0;

// samples averaged for buttons
//
static uint16_t _tscSampleSets[cTscChannelCount][cTscSamplesToAverage];

// minimum read values for buttons
//
static uint16_t _tscMinimums[cTscChannelCount];

// maximum read values for buttons
//
static uint16_t _tscMaximums[cTscChannelCount];

// tracks which TSC channel(s) we'll sample next
//
static uint8_t _tscChannelCounter = 0;

// a place for data from the TSC to live
//
static uint16_t _tscAcquisitionValues[cTscChannelCount];

// button/touchkey states (one bit per button)
//
static uint8_t _buttonStates = 0;

// buttons that have change states since the last sampling
//
static uint8_t _buttonsChanged = 0;

// the button states the last time they were sampled
//
static uint8_t _previousButtonStates = 0;

// provides a way to calibrate the temperature indication
//
static int8_t _temperatureAdjustment = -9;

// date & time stored by Refresh()
//
static DateTime _currentDateTime;

// contains the temperature in degrees Celsius times cBaseMultiplier
//
static int32_t _temperatureXcBaseMultiplier = 0;

// contains the calculated light level based on the ADC's reading
//
static uint16_t _lightLevel = 0;

// true if it's time to refresh the ADC sampling arrays
//
static bool _adcSampleRefreshNow = false;

// state of the XBLNK pin
//
static bool _displayBlankingState = false;

// indicates to setDisplayBlankPin() that it should NOT clear the BLANK pin as
//  a display write is in progress with flicker reduction active
volatile static bool _displayBlankForWrite = false;

// true if DS3234 is detected
//
static bool _externalRtcConnected = false;

// inverts NSS state during SPI transfers (for DS1722) if true
//
static bool _invertCsForTemperatureSensor = false;

// true if the last I2C/SPI hardware refresh was the DS323x; ensures we toggle
//  which device is refreshed so one can't dominate the bus
static bool _lastRefreshWasRTC = false;

// true if RTC is set/valid
//
static bool _rtcIsSet = false;

// true if temp sensor is detected
//
static TempSensorType _externalTemperatureSensor = TempSensorType::None;

// a string used for USART tx
//
// char buffer[80];
// writeSerial(USART1, sprintf(buffer, "memoryPtr = %x - size = %d\r\n", memoryPtr, numElements), buffer);


// set up the ADC
//
void _adcSetup()
{
  uint8_t channelArray[] = {cPhototransistorChannel, ADC_CHANNEL_TEMP, ADC_CHANNEL_VREF, ADC_CHANNEL_VBAT};

	adc_power_off(ADC1);

	adc_calibrate(ADC1);
  while ((ADC_CR(ADC1) & ADC_CR_ADCAL) != 0);

  adc_power_on(ADC1);
  while ((ADC_ISR(ADC1) & ADC_ISR_ADRDY) == 0);

  adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC);
	adc_set_operation_mode(ADC1, ADC_MODE_SCAN);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_enable_temperature_sensor();
  adc_enable_vrefint();
  adc_enable_vbat_sensor();
  adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_071DOT5);
  // adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_239DOT5);
	adc_set_regular_sequence(ADC1, cAdcChannelCount, channelArray);
	adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
	adc_disable_analog_watchdog(ADC1);
  adc_set_continuous_conversion_mode(ADC1);
  ADC_CFGR1(ADC1) |= ADC_CFGR1_DMACFG;
  adc_enable_dma(ADC1);

  ADC_CR(ADC1) |= ADC_CR_ADSTART;   // adc_start_conversion_regular blocks :(
}


// Set STM32's clock to 48 MHz from 8 MHz HSE oscillator
//
void _clockSetup()
{
  rcc_clock_setup_in_hse_8mhz_out_48mhz();

  // Enable clocks to various subsystems we'll need
	// PWR must be enabled first as we can't disable the backup domain protection
  //  without it online
  rcc_periph_clock_enable(RCC_PWR);

  // SYSCFG is needed to remap USART DMA channels
  rcc_periph_clock_enable(RCC_SYSCFG_COMP);

  rcc_periph_clock_enable(RCC_DMA);

  rcc_periph_clock_enable(RCC_RTC);

  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_clock_enable(RCC_TIM3);
  rcc_periph_clock_enable(RCC_TIM14);
  rcc_periph_clock_enable(RCC_TIM15);
  rcc_periph_clock_enable(RCC_TIM16);

  rcc_periph_clock_enable(RCC_ADC);

  rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

  if (cTargetHardwareVersion >= 4)
  {
    rcc_periph_clock_enable(RCC_GPIOD);
  }

  if (cTargetHardwareVersion != 3)
  {
    // Set SYSCLK (not the default HSI) as I2C1's clock source
    // rcc_set_i2c_clock_hsi(I2C1);
  	RCC_CFGR3 |= RCC_CFGR3_I2C1SW;

    rcc_periph_clock_enable(RCC_I2C1);
  }

  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_clock_enable(RCC_SPI2);
  rcc_periph_clock_enable(RCC_USART1);
  rcc_periph_clock_enable(RCC_USART2);

  rcc_periph_clock_enable(RCC_TSC);
}


// Enable DMA interrupts
//
void _dmaIntEnable()
{
  // nvic_set_priority(NVIC_ADC_COMP_IRQ, 0);
	// nvic_enable_irq(NVIC_ADC_COMP_IRQ);
  // nvic_set_priority(NVIC_DMA1_CHANNEL1_IRQ, 0);
	// nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);
  nvic_set_priority(NVIC_DMA1_CHANNEL2_3_IRQ, 64);
	nvic_enable_irq(NVIC_DMA1_CHANNEL2_3_IRQ);
  nvic_set_priority(NVIC_DMA1_CHANNEL4_5_IRQ, 64);
	nvic_enable_irq(NVIC_DMA1_CHANNEL4_5_IRQ);
}


// Configure DMA
//
void _dmaSetup()
{
  if (cTargetHardwareVersion <= 2)
  {
    // I2C, USART1 and USART2 TX and RX remap bits set in SYSCFG_CFGR1 since
    //  SPI1 ties up the I2C1 & USART1 default DMA channels
    SYSCFG_CFGR1 |= (SYSCFG_CFGR1_USART1_RX_DMA_RMP |
                     SYSCFG_CFGR1_USART1_TX_DMA_RMP |
                     SYSCFG_CFGR1_I2C1_DMA_RMP);
  }
  else
  {
    // USART1 and USART2 TX and RX remap bits set in SYSCFG_CFGR1 since
    //  SPI1 ties up the USART's default DMA channels
    SYSCFG_CFGR1 |= (SYSCFG_CFGR1_USART1_RX_DMA_RMP |
                     SYSCFG_CFGR1_USART1_TX_DMA_RMP |
                     SYSCFG_CFGR1_USART2_DMA_RMP);
  }

  // Reset DMA channels
  dma_channel_reset(DMA1, DMA_CHANNEL1);
  dma_channel_reset(DMA1, DMA_CHANNEL2);
  dma_channel_reset(DMA1, DMA_CHANNEL3);
  dma_channel_reset(DMA1, DMA_CHANNEL4);
  dma_channel_reset(DMA1, DMA_CHANNEL5);
  dma_channel_reset(DMA1, DMA_CHANNEL6);
  dma_channel_reset(DMA1, DMA_CHANNEL7);

  // Set up ADC DMA -- it has higher priority to avoid overrun
  dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC1_DR);
  dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)_bufferADC);
  dma_set_number_of_data(DMA1, DMA_CHANNEL1, cAdcChannelCount);
  dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);
  dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
  dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_VERY_HIGH);
  dma_enable_circular_mode(DMA1, DMA_CHANNEL1);
  dma_enable_channel(DMA1, DMA_CHANNEL1);

  // Set up rx dma, note it has higher priority to avoid overrun
  // dma_set_peripheral_address(DMA1, DMA_CHANNEL2, (uint32_t)&SPI1_DR);
  // dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t)_bufferIn);
  // dma_set_number_of_data(DMA1, DMA_CHANNEL2, cPwmChannelsPerDevice * cPwmNumberOfDevices);
  // dma_set_read_from_peripheral(DMA1, DMA_CHANNEL2);
  // dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL2);
  // dma_set_peripheral_size(DMA1, DMA_CHANNEL2, DMA_CCR_PSIZE_16BIT);
  // dma_set_memory_size(DMA1, DMA_CHANNEL2, DMA_CCR_MSIZE_16BIT);
  // dma_set_priority(DMA1, DMA_CHANNEL2, DMA_CCR_PL_VERY_HIGH);
  // dma_enable_circular_mode(DMA1, DMA_CHANNEL2);

  // Set up tx dma
  // dma_set_peripheral_address(DMA1, DMA_CHANNEL3, (uint32_t)&SPI1_DR);
  // dma_set_memory_address(DMA1, DMA_CHANNEL3, (uint32_t)_bufferOut);
  // dma_set_number_of_data(DMA1, DMA_CHANNEL3, cPwmChannelsPerDevice * cPwmNumberOfDevices);
  // dma_set_read_from_memory(DMA1, DMA_CHANNEL3);
  // dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL3);
  // dma_set_peripheral_size(DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_16BIT);
  // dma_set_memory_size(DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_16BIT);
  // dma_set_priority(DMA1, DMA_CHANNEL3, DMA_CCR_PL_HIGH);
  // dma_enable_circular_mode(DMA1, DMA_CHANNEL3);

	_dmaIntEnable();
}


// Configure GPIOs
//
void _gpioSetup()
{
  if (cTargetHardwareVersion == 1)
  {
    // Configure current drive control pins
    gpio_mode_setup(cCurrentDrivePort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cCurrentDriveLowPin | cCurrentDriveHighPin);
    // Configure the SPI NSS and BLANK pins for the LED drivers
    gpio_mode_setup(cNssPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cNssDisplayPin | cBlankDisplayPin);
    // gpio_mode_setup(cBlankDisplayPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cBlankDisplayPin);
    // Configure the buzzer output
    gpio_mode_setup(cBuzzerPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cBuzzerPin);
    // Configure the analog input pin for the phototransistor
    gpio_mode_setup(cPhototransistorPort, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, cPhototransistorPin);

    // Finally, set the inital pin states for the pins we configured above
    gpio_set(cBlankDisplayPort, cBlankDisplayPin);
    gpio_clear(cBuzzerPort, cBuzzerPin);
    gpio_clear(cCurrentDrivePort, cCurrentDriveLowPin | cCurrentDriveHighPin);
  }
  else if (cTargetHardwareVersion == 2)
  {
    // Configure current drive control pins
    gpio_mode_setup(cCurrentDrivePort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cCurrentDriveLowPin | cCurrentDriveHighPin);
    // Configure the SPI NSS and BLANK pins for the LED drivers
    gpio_mode_setup(cNssPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cNssDisplayPin | cBlankDisplayPin);
    // gpio_mode_setup(cBlankDisplayPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cBlankDisplayPin);
    // Configure the analog input pin for the phototransistor
    gpio_mode_setup(cPhototransistorPort, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, cPhototransistorPin);

    // Finally, set the inital pin states for the pins we configured above
    gpio_set(cBlankDisplayPort, cBlankDisplayPin);
    gpio_clear(cCurrentDrivePort, cCurrentDriveLowPin | cCurrentDriveHighPin);
  }
  else if (cTargetHardwareVersion == 3)
  {
    // Configure current drive control pins
    gpio_mode_setup(cCurrentDrivePort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cCurrentDriveLowPin | cCurrentDriveHighPin);
    // Configure the SPI NSS pins for the LED drivers, RTC, and temp sensor
    gpio_mode_setup(cNssPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cNssDisplayPin | cNssRtcPin | cNssTemperaturePin);
    // Configure the BLANK pin for the LED drivers
    gpio_mode_setup(cBlankDisplayPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cBlankDisplayPin);
    // Configure the analog input pin for the phototransistor
    gpio_mode_setup(cPhototransistorPort, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, cPhototransistorPin);

    // Finally, set the inital pin states for the pins we configured above
    gpio_set(cNssPort, cNssDisplayPin | cNssRtcPin | cNssTemperaturePin);
    // gpio_clear(cNssPort, cNssTemperaturePin);
    gpio_set(cBlankDisplayPort, cBlankDisplayPin);
    gpio_clear(cCurrentDrivePort, cCurrentDriveLowPin | cCurrentDriveHighPin);
  }
  else if (cTargetHardwareVersion == 4)
  {
    // Configure the SPI NSS pins for the LED drivers, RTC, and temp sensor
    gpio_mode_setup(cNssPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cNssDisplayPin | cNssRtcPin | cNssTemperaturePin);
    // Configure the BLANK pin for the LED drivers
    gpio_mode_setup(cBlankDisplayPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, cBlankDisplayPin);
    // Configure the analog input pin for the phototransistor
    gpio_mode_setup(cPhototransistorPort, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, cPhototransistorPin);

    // Finally, set the inital pin states for the pins we configured above
    gpio_set(cNssPort, cNssDisplayPin | cNssRtcPin | cNssTemperaturePin);
    gpio_clear(cBlankDisplayPort, cBlankDisplayPin);
  }
  // Finally, configure the external alarm input pins
  for (uint8_t i = 0; i < cAlarmInputPinCount; i++)
  {
    gpio_mode_setup(cAlarmInputPort, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, cAlarmInputPins[i]);
  }
}


// Configure the Independent Watchdog Timer
//
void _iwdgSetup()
{
  iwdg_set_period_ms(5000);
  iwdg_start();
}


void _i2cSetup()
{
  // Configure GPIOs SCL=PB8 and SDA=PB9 so we can reset the bus nicely
  gpio_mode_setup(cI2c1Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, cI2c1SlcPin | cI2c1SdaPin);
  gpio_set_output_options(cI2c1Port, GPIO_OTYPE_OD, GPIO_OSPEED_100MHZ, cI2c1SlcPin | cI2c1SdaPin);

  gpio_set(cI2c1Port, cI2c1SlcPin | cI2c1SdaPin);
  delay(1);
  gpio_clear(cI2c1Port, cI2c1SdaPin);
  delay(1);
  gpio_clear(cI2c1Port, cI2c1SlcPin);
  delay(1);
  gpio_set(cI2c1Port, cI2c1SlcPin);
  delay(1);
  gpio_set(cI2c1Port, cI2c1SdaPin);
  delay(1);

  gpio_mode_setup(cI2c1Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, cI2c1SdaPin);
  // Clock out any remaining bits any slave is trying to send
  while (gpio_get(cI2c1Port, cI2c1SdaPin) == false)
  {
    gpio_clear(cI2c1Port, cI2c1SlcPin);
    delay(1);
    gpio_set(cI2c1Port, cI2c1SlcPin);
    delay(1);
  }

  // Now configure GPIOs for I2C use
	gpio_mode_setup(cI2c1Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, cI2c1SlcPin | cI2c1SdaPin);

	// Setup I2C1 pins as alternate function one
	gpio_set_af(cI2c1Port, GPIO_AF1, cI2c1SlcPin | cI2c1SdaPin);

	// Set alternate functions for the SCL and SDA pins of I2C1
  gpio_set_output_options(cI2c1Port, GPIO_OTYPE_OD, GPIO_OSPEED_100MHZ, cI2c1SlcPin | cI2c1SdaPin);

  // Enable heftier drivers for I2C pins...maybe helpful?
  // SYSCFG_CFGR1 |= (SYSCFG_CFGR1_I2C_PB8_FMP | SYSCFG_CFGR1_I2C_PB9_FMP);

	// Reset/Disable the I2C before changing any configuration
  i2c_reset(I2C1);

  // 400KHz - I2C Fast Mode - SYSCLK is 48 MHz
  i2c_set_speed(I2C1, i2c_speed_fm_400k, 48);

  // Set our slave address in case we should want to receive from other masters
	i2c_set_own_7bit_slave_address(I2C1, 0x11);

  // Configure ANFOFF DNF[3:0] in CR1
	// i2c_enable_analog_filter(I2C1);
	// i2c_set_digital_filter(I2C1, I2C_CR1_DNF_DISABLED);

	// Configure No-Stretch CR1
  //  (only relevant in slave mode, must be disabled in master mode)
	// i2c_disable_stretching(I2C1);

	// Once everything is configured, enable the peripheral
	i2c_peripheral_enable(I2C1);

  _i2cState = I2cState::I2cIdle;
}


void _i2cRecover()
{
  // Ensure status is busy -- it'll be reset to Idle by _i2cSetup()
  _i2cState = I2cState::I2cBusy;

  // Stop/cancel any on-going or pending DMA
  dma_channel_reset(DMA1, DMA_CHANNEL6);
  dma_channel_reset(DMA1, DMA_CHANNEL7);

  // Disable the I2C before all this
	i2c_peripheral_disable(I2C1);

  _i2cSetup();

  _i2cBusyFailCount = 0;
}


// Configure RTC and ensure it's running
//
void _rtcSetup()
{
  // these values are the power-on defaults for the prescaler.
  // we'll set them anyway to be sure they're there
	const uint32_t async = 127;
  uint32_t sync = 255;
  uint16_t timeout = 474;

  RgbLed red(2048, 0, 0, 0),
         yellow(2048, 1536, 0, 0),
         green(0, 2048, 0, 0),
         blue(0, 0, 2048, 0);

  if (rcc_is_osc_ready(RCC_LSE) == false)
  {
    blinkStatusLed(blue, red, 2, 100);

    pwr_disable_backup_domain_write_protect();

    // reset the RTC to ensure it's in a known state
    RCC_BDCR |= RCC_BDCR_BDRST;
    RCC_BDCR &= ~RCC_BDCR_BDRST;

    if (_externalRtcConnected == true)
    {
      rcc_osc_bypass_enable(RCC_LSE);
    }

    if (cTargetHardwareVersion >= 2)
    {
      rcc_osc_on(RCC_LSE);
      rcc_wait_for_osc_ready(RCC_LSE);

      rcc_set_rtc_clock_source(RCC_LSE);
    }
    else
    {
      rcc_set_rtc_clock_source(RCC_HSE);
      sync = 1953;
    }

  	rcc_enable_rtc_clock();

  	rtc_unlock();

  	// enter init mode -- this lets us test that everything is working as expected
  	RTC_ISR |= RTC_ISR_INIT;
    while (((RTC_ISR & RTC_ISR_INITF) == 0) && (--timeout > 0));

    // do a blinky thing to indicate the problem if we had to wait too long :(
    if (timeout == 0)
    {
      // reset the RTC to undo anything that might've been done above
      RCC_BDCR |= RCC_BDCR_BDRST;
      RCC_BDCR &= ~RCC_BDCR_BDRST;

      blinkStatusLed(red, yellow, 20, 500);
    }

  	// set synch prescaler, using defaults for 1Hz out
  	rtc_set_prescaler(sync, async);

  	// exit init mode
  	RTC_ISR &= ~(RTC_ISR_INIT);

    rtc_lock();

  	// And wait for synchro...
  	rtc_wait_for_synchro();
    pwr_enable_backup_domain_write_protect();
  }
  else
  {
    blinkStatusLed(blue, green, 2, 100);
  }
}


// Toggle SPI MISO between display drivers and the other peripherals
// ***** Make sure SPI is not busy/in use before switching things around! *****
void _spiSelectPeripheral(const SpiPeripheral peripheral)
{
  // We won't waste time switching things around if we don't need to
  if (_spiActivePeripheral != peripheral)
  {
    // Reset SPI, SPI_CR1 register cleared, SPI is disabled
    spi_disable(SPI1);
    // Configure GPIO pins first...
    switch (peripheral)
    {
      case SpiPeripheral::LedDriversGs:
      case SpiPeripheral::LedDriversOther:
        // Configure other peripherals MISO pin as input
        gpio_mode_setup(cSpi1Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, cSpi1MisoPin);
        // MISO=PB4 from LED drivers
        gpio_mode_setup(cSpi1AltPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cSpi1MisoDisplayPin);
        // LED drivers can cope with a higher clock rate
        // spi_set_baudrate_prescaler(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4);
        break;

      default:
        // Configure display driver peripheral MISO pin as input
        gpio_mode_setup(cSpi1AltPort, GPIO_MODE_INPUT, GPIO_PUPD_NONE, cSpi1MisoDisplayPin);
        // Configure GPIOs: SCK=PA5, MISO=PA6 and MOSI=PA7
        gpio_mode_setup(cSpi1Port, GPIO_MODE_AF, GPIO_PUPD_NONE, cSpi1MisoPin);
        // RTC/Temp sensor technically require a slower clock rate...
        // ...but they seem to work with the same fast rate as the LED drivers...
        // spi_set_baudrate_prescaler(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_8);
    }
    // Now configure SPI parameters
    switch (peripheral)
    {
      case SpiPeripheral::LedDriversGs:
        spi_fifo_reception_threshold_16bit(SPI1);
        spi_set_data_size(SPI1, SPI_CR2_DS_12BIT);
        break;

      default:
        spi_fifo_reception_threshold_8bit(SPI1);
      	spi_set_data_size(SPI1, SPI_CR2_DS_8BIT);
    }
    /*
     * Set NSS management to software.
     *
     * Note:
     * Setting nss high is very important, even if we are controlling the GPIO
     * ourselves this bit needs to be at least set to 1, otherwise the spi
     * peripheral will not send any data out.
     */
  	spi_enable_software_slave_management(SPI1);
  	spi_set_nss_high(SPI1);

     // Enable SPI1 peripheral
    spi_enable(SPI1);
    // Finally, remember what we're now set to
    _spiActivePeripheral = peripheral;
  }
}


// Configure SPI
//
void _spiSetup()
{
  // Setup SPI1 pins as alternate function zero
  gpio_set_af(cSpi1Port, GPIO_AF0, cSpi1SckPin | cSpi1MisoPin | cSpi1MosiPin);
  // MISO=PB4 from LED drivers
  gpio_set_af(cSpi1AltPort, GPIO_AF0, cSpi1MisoDisplayPin);

  // Configure GPIOs: SCK=PA5 and MOSI=PA7
  gpio_mode_setup(cSpi1Port, GPIO_MODE_AF, GPIO_PUPD_NONE, cSpi1SckPin | cSpi1MosiPin);

  // Reset SPI, SPI_CR1 register cleared, SPI is disabled
  spi_reset(SPI1);

  /* Set up SPI in Master mode with:
   * Clock baud rate: 1/8 of peripheral clock frequency
   * Clock polarity: Idle High
   * Clock phase: Data valid on 1st clock pulse
   * Frame format: MSB First
   */
  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_CRCL_8BIT, SPI_CR1_MSBFIRST);

  // Configure SPI1 for use with the display driver (should already be idle now)
  _spiSelectPeripheral(SpiPeripheral::LedDriversGs);
}


/*
 * Set up timer to fire every x milliseconds
 * This is a unusual usage of systick, be very careful with the 24bit range
 * of the systick counter!  You can range from 1 to 2796ms with this.
 */
void _systickSetup(const uint16_t xms)
{
	// div8 per ST, stays compatible with M3/M4 parts, well done ST
	systick_set_clocksource(STK_CSR_CLKSOURCE_EXT);
	// clear counter so it starts right away
	STK_CVR = 0;

	systick_set_reload(rcc_ahb_frequency / 8 / 1000 * xms);
	systick_counter_enable();
	systick_interrupt_enable();
}


// Set up the Timers
//
void _timerSetup()
{
  // Configure our beeper and status LED pins
  if (cTargetHardwareVersion == 3)
  {
    gpio_mode_setup(cLedPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cLed0Pin);
    gpio_mode_setup(cLedAltPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cLed1Pin | cLed2Pin);
    gpio_set_af(cLedPort, cLedPortAF, cLed0Pin);
    gpio_set_af(cLedAltPort, cLedPortAF, cLed1Pin | cLed2Pin);
    gpio_set_output_options(cLedPort, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, cLed0Pin);
    gpio_set_output_options(cLedAltPort, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, cLed1Pin | cLed2Pin);
  }
  else
  {
    gpio_mode_setup(cLedPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cLed0Pin | cLed1Pin | cLed2Pin);
    gpio_set_af(cLedPort, cLedPortAF, cLed0Pin | cLed1Pin | cLed2Pin);
    gpio_set_output_options(cLedPort, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, cLed0Pin | cLed1Pin | cLed2Pin);
  }

  if (cTargetHardwareVersion >= 2)
  {
    gpio_mode_setup(cBuzzerPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cBuzzerPin);
    gpio_set_af(cBuzzerPort, GPIO_AF2, cBuzzerPin);
    gpio_set_output_options(cBuzzerPort, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, cBuzzerPin);
  }

  if (cTargetHardwareVersion == 4)
  {
    gpio_mode_setup(cDriverGsckPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cDriverGsckPin);
    gpio_set_af(cDriverGsckPort, GPIO_AF4, cDriverGsckPin);
    gpio_set_output_options(cDriverGsckPort, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, cDriverGsckPin);
  }

  // Reset timer peripherals
  timer_reset(TIM1);
  timer_reset(TIM2);
  timer_reset(TIM3);
  timer_reset(TIM14);

  /* Set the timers global mode to:
   * - use no divider
   * - alignment edge
   * - count direction up
   */
  timer_set_mode(TIM1,  TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_mode(TIM2,  TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_mode(TIM3,  TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_mode(TIM14, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  timer_set_prescaler(TIM1, 8);
  timer_set_prescaler(TIM2, 0);
  timer_set_prescaler(TIM3, 0);
	timer_set_prescaler(TIM14, 0);
  timer_set_repetition_counter(TIM1, 0);
  timer_set_repetition_counter(TIM2, 0);
  timer_set_repetition_counter(TIM3, 0);
	timer_set_repetition_counter(TIM14, 0);
  timer_continuous_mode(TIM1);
  timer_continuous_mode(TIM2);
  timer_continuous_mode(TIM3);
	timer_continuous_mode(TIM14);
  timer_set_period(TIM1, 1);
  timer_set_period(TIM2, Display::cLedMaxIntensity);
  timer_set_period(TIM3, Display::cLedMaxIntensity);
	timer_set_period(TIM14, 2);

  if ((cTargetHardwareVersion == 1) || (cTargetHardwareVersion == 4))
  {
    timer_disable_oc_output(TIM3, TIM_OC1);
    timer_disable_oc_output(TIM3, TIM_OC2);
    timer_disable_oc_output(TIM3, TIM_OC3);
    timer_disable_oc_output(TIM3, TIM_OC4);
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM3, TIM_OC4, TIM_OCM_PWM1);
    timer_set_oc_value(TIM3, TIM_OC1, 0);
    timer_set_oc_value(TIM3, TIM_OC2, 0);
    timer_set_oc_value(TIM3, TIM_OC3, 0);
    timer_set_oc_value(TIM3, TIM_OC4, 0);
    timer_enable_oc_output(TIM3, TIM_OC1);
    timer_enable_oc_output(TIM3, TIM_OC2);
    timer_enable_oc_output(TIM3, TIM_OC3);
    timer_enable_oc_output(TIM3, TIM_OC4);
  }
  else if ((cTargetHardwareVersion == 2) || (cTargetHardwareVersion == 3))
  {
    timer_disable_oc_output(TIM2, TIM_OC1);
    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_disable_oc_output(TIM2, TIM_OC3);
    timer_disable_oc_output(TIM2, TIM_OC4);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);
    timer_set_oc_value(TIM2, TIM_OC1, 0);
    timer_set_oc_value(TIM2, TIM_OC2, 0);
    timer_set_oc_value(TIM2, TIM_OC3, 0);
    timer_set_oc_value(TIM2, TIM_OC4, 0);
    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_enable_oc_output(TIM2, TIM_OC2);
    timer_enable_oc_output(TIM2, TIM_OC3);
    timer_enable_oc_output(TIM2, TIM_OC4);
  }

  if (cTargetHardwareVersion >= 2)
  {
    timer_disable_oc_output(TIM1, TIM_OC1);
    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_value(TIM1, TIM_OC1, 0);

    timer_enable_break_main_output(TIM1);
    timer_enable_oc_output(TIM1, TIM_OC1);
  }

  if (cTargetHardwareVersion == 4)
  {
    timer_disable_oc_output(TIM14, TIM_OC1);
    timer_set_oc_mode(TIM14, TIM_OC1, TIM_OCM_TOGGLE);
    timer_set_oc_value(TIM14, TIM_OC1, 0);

    timer_slave_set_trigger(TIM2, TIM_SMCR_TS_ITR3);
  	timer_slave_set_mode(TIM2, TIM_SMCR_SMS_ECM1);

    timer_enable_oc_output(TIM14, TIM_OC1);

    timer_enable_irq(TIM2, TIM_DIER_UIE);

    nvic_set_priority(NVIC_TIM2_IRQ, 0);
  	nvic_enable_irq(NVIC_TIM2_IRQ);
  }

  timer_enable_preload(TIM1);
  timer_enable_preload(TIM2);
  timer_enable_preload(TIM3);
  timer_enable_preload(TIM14);

  timer_enable_counter(TIM1);
  timer_enable_counter(TIM2);
  timer_enable_counter(TIM3);
  timer_enable_counter(TIM14);
}


// Set up the Touch Sensing Controller
//
void _tscSetup()
{
  gpio_mode_setup(cTscPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cTscTouchKeyPins | cTscSamplingCapPins);
  gpio_set_output_options(cTscPort, GPIO_OTYPE_OD, GPIO_OSPEED_MED, cTscSamplingCapPins);
  gpio_set_output_options(cTscPort, GPIO_OTYPE_PP, GPIO_OSPEED_MED, cTscTouchKeyPins);
	gpio_set_af(cTscPort, GPIO_AF3, cTscTouchKeyPins | cTscSamplingCapPins);

  if (cTargetHardwareVersion == 4)
  {
    gpio_mode_setup(cTscPortC, GPIO_MODE_AF, GPIO_PUPD_NONE, cTscTouchKeyPinsC);
    gpio_set_output_options(cTscPortC, GPIO_OTYPE_PP, GPIO_OSPEED_MED, cTscTouchKeyPinsC);
    gpio_set_af(cTscPortC, GPIO_AF0, cTscTouchKeyPinsC);
  }

  /* To allow the control of the sampling capacitor I/O by the TSC peripheral,
   the corresponding GPIO must be first set to alternate output open drain
   mode and then the corresponding Gx_IOy bit in the TSC_IOSCR register must
   be set. */
	TSC_IOSCR = cTscSamplingControlBits;

  _tscChannelCounter = 0;

  /* To allow the control of the channel I/O by the TSC peripheral, the
  corresponding GPIO must be first set to alternate output push-pull mode and
  the corresponding Gx_IOy bit in the TSC_IOCCR register must be set. */
  TSC_IOCCR = cTscChannelControlBits[_tscChannelCounter];

  // The GxE bits in the TSC_IOGCSR registers specify which analog I/O groups are enabled
  TSC_IOGCSR = cTscGroupControlBits;

  // schmitt trigger hysteresis disabled for all groups
  TSC_IOHCR = 0x00;
  // TSC_IOHCR = (TSC_IOHCR_G3(2) | TSC_IOHCR_G3(3) |
  //              TSC_IOHCR_G5(3) | TSC_IOHCR_G5(4) |
  //              TSC_IOHCR_G6(2) | TSC_IOHCR_G6(3));

  // enable the end of acquisition and max count error interrupts
  TSC_ICR = TSC_ICR_EOAIC | TSC_ICR_MCEIC;
  TSC_IER = TSC_IER_EOAIE | TSC_IER_MCEIE;
  nvic_set_priority(NVIC_TSC_IRQ, 128);
  nvic_enable_irq(NVIC_TSC_IRQ);

  // define the pulse generator prescaler value
  TSC_CR |= (0x01 << TSC_CR_PGPSC_SHIFT);
  // define the duration of the high state of the charge transfer pulse (charge of CX)
  TSC_CR |= (0x01 << TSC_CR_CTPH_SHIFT);
  // define the duration of the low state of the charge transfer pulse (transfer of charge from CX to CS)
  TSC_CR |= (0x01 << TSC_CR_CTPL_SHIFT);
  // define the maximum number of charge transfer pulses that can be generated before a max count error is generated
  TSC_CR |= (0x06 << TSC_CR_MCV_SHIFT);
  // configure and enable spread spectrum
  TSC_CR |= (0x3f << TSC_CR_SSD_SHIFT) | TSC_CR_SSE;

  // Enable the TSC
  TSC_CR |= TSC_CR_TSCE;
  // Normal acquisition mode:
  //  acquisition starts when the START bit in the TSC_CR register is set
  TSC_CR |= TSC_CR_START;
}


// Set up USARTs
//
void _usartSetup()
{
  // Setup GPIO pins for USART1 & USART2 transmit and recieve
  gpio_mode_setup(cUsart1RxPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cUsart1RxPin);
  gpio_mode_setup(cUsart1TxPort, GPIO_MODE_AF, GPIO_PUPD_NONE, cUsart1TxPin);
  gpio_mode_setup(cUsart2Port, GPIO_MODE_AF, GPIO_PUPD_NONE, cUsart2RxPin | cUsart2TxPin | cUsart2DePin);

  // Setup USART1 & USART2 TX & RX pins as alternate function
  if (cUsart1TxPin == GPIO9)
  {
    gpio_set_af(cUsart1TxPort, GPIO_AF1, cUsart1TxPin);
  }
  else if (cUsart1TxPin == GPIO6)
  {
    gpio_set_af(cUsart1TxPort, GPIO_AF0, cUsart1TxPin);
  }

  if (cUsart1RxPin == GPIO10)
  {
    gpio_set_af(cUsart1RxPort, GPIO_AF1, cUsart1RxPin);
  }
  else if (cUsart1RxPin == GPIO7)
  {
    gpio_set_af(cUsart1RxPort, GPIO_AF0, cUsart1RxPin);
  }

  gpio_set_af(cUsart2Port, GPIO_AF1, cUsart2RxPin | cUsart2TxPin | cUsart2DePin);

  // Set up USART1 parameters
  usart_set_databits(USART1, 8);
	usart_set_baudrate(USART1, cUsart1BaudRate);
  usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  // Set up USART2 parameters
  usart_set_databits(USART2, 8);
	usart_set_baudrate(USART2, cUsart2BaudRate);
  usart_set_stopbits(USART2, USART_CR2_STOP_2_0BIT);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_mode(USART2, USART_MODE_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  // Enable the USART's RS-485 driver mode output
  USART2_CR3 |= USART_CR3_DEM;

  // usart_enable_rx_interrupt(USART1);
  // usart_enable_rx_interrupt(USART2);
  // usart_enable_tx_interrupt(USART1);
  // usart_enable_tx_interrupt(USART2);
  // usart_enable_error_interrupt(USART1);
  usart_enable_error_interrupt(USART2);

  // Enable the USARTs
  usart_enable(USART1);
  usart_enable(USART2);

  USART1_CR1 |= USART_CR1_TE;
  USART2_CR1 |= USART_CR1_RE;

  // nvic_set_priority(NVIC_USART1_IRQ, 0);
	// nvic_enable_irq(NVIC_USART1_IRQ);
  nvic_set_priority(NVIC_USART2_IRQ, 64);
	nvic_enable_irq(NVIC_USART2_IRQ);
}


// refreshes _temperatureXcBaseMultiplier from STM32 hardware
//
void _refreshInternalTemp()
{
  uint32_t totalTemp = 0, totalVoltage = 0;

  // Add up some values so we can compute some averages...
  for (uint8_t i = 0; i < cAdcSamplesToAverage; i++)
  {
    totalTemp += _adcSampleSetTemp[i];
    totalVoltage += _adcSampleSetVoltage[i];
  }
  // Get the averages of the last several readings
  int32_t averageTemp = totalTemp / cAdcSamplesToAverage;
  int32_t averageVoltage = totalVoltage / cAdcSamplesToAverage;
  // Determine the voltage as it affects the temperature calculation
  int32_t voltage = cVddCalibrationVoltage * (int32_t)ST_VREFINT_CAL / averageVoltage;
  // Now we can compute the temperature based on RM's formula, * cBaseMultiplier
  _temperatureXcBaseMultiplier = (averageTemp * voltage / cVddCalibrationVoltage) - ST_TSENSE_CAL1_30C;
  _temperatureXcBaseMultiplier = _temperatureXcBaseMultiplier * (110 - 30) * cBaseMultiplier;
  _temperatureXcBaseMultiplier = _temperatureXcBaseMultiplier / (ST_TSENSE_CAL2_110C - ST_TSENSE_CAL1_30C);
  _temperatureXcBaseMultiplier = _temperatureXcBaseMultiplier + 30000 + (_temperatureAdjustment * cBaseMultiplier);
}


// Starts it all up
//
void initialize()
{
  uint32_t startupDisplayBitmap = cTargetHardwareVersion << 16;
  RgbLed   startupDarkWhite(144, 114, 118, 0),
           startupBrightWhite(512, 380, 480, 0),
           startupOff(0, 0, 0, 0);
  Display  startupDisplay(startupDarkWhite, startupBrightWhite, startupDisplayBitmap);

  // First, set up all the hardware things
  _clockSetup();
  _iwdgSetup();
  _gpioSetup();
  _timerSetup();
  _dmaSetup();
  _adcSetup();
  _spiSetup();
  _usartSetup();
  _tscSetup();

  _systickSetup(1);   // tick every 1 mS

  if (cTargetHardwareVersion != 3)
  {
    _i2cSetup();
  }

  Dmx512Rx::initialize();

  // Let's initialize some memory/data structures
  for (_adcSampleCounter = 0; _adcSampleCounter < cAdcSamplesToAverage; _adcSampleCounter++)
  {
    _adcSampleSetLight[_adcSampleCounter] = _bufferADC[cAdcPhototransistor];
    _adcSampleSetTemp[_adcSampleCounter] = _bufferADC[cAdcTemperature];
    _adcSampleSetVoltage[_adcSampleCounter] = _bufferADC[cAdcVref];
  }

  for (_tscChannelCounter = 0; _tscChannelCounter < cTscChannelCount; _tscChannelCounter++)
  {
    _tscAcquisitionValues[_tscChannelCounter] = 0;
    _tscMinimums[_tscChannelCounter] = 0xffff;
    _tscMaximums[_tscChannelCounter] = 0;
  }

  // for v3+ hardware, we check for SPI peripherals
  if (cTargetHardwareVersion >= 3)
  {
    // Here we check for other temperature sensors
    if (LM74::isConnected() == true)
    {
      _externalTemperatureSensor = TempSensorType::LM74;
      startupDisplayBitmap |= 0x01;   // this will appear on the startup display
    }
    else
    {
      _invertCsForTemperatureSensor = true;

      if (DS1722::isConnected() == true)
      {
        _externalTemperatureSensor = TempSensorType::DS1722;
        startupDisplayBitmap |= 0x02;   // this will appear on the startup display
      }
      else
      {
        _invertCsForTemperatureSensor = false;
      }
    }

    _externalRtcConnected = DS3234::isConnected();

    if (_externalRtcConnected == true)
    {
      while (_spiState != SpiState::SpiIdle);
      // isConnected() refreshes the status register so this will work without a refresh()
      _rtcIsSet = DS3234::isValid();
      startupDisplayBitmap |= 0x10;   // this will appear on the startup display
      if (_externalTemperatureSensor == TempSensorType::None)
      {
        _externalTemperatureSensor = TempSensorType::DS3234;
      }
    }
    else
    {
      _rtcIsSet = RTC_ISR & RTC_ISR_INITS;
    }
  }
  // for pre-v3 hardware, we check for I2C peripherals
  else
  {
    _externalRtcConnected = DS3231::isConnected();

    if (_externalRtcConnected == true)
    {
      // isConnected() refreshes the status register so this will work without a refresh()
      _rtcIsSet = DS3231::isValid();
      DS3231::set32kHzOut(true);
      _externalTemperatureSensor = TempSensorType::DS3231;
      startupDisplayBitmap |= 0x01;   // this will appear on the startup display
    }
    else
    {
      _rtcIsSet = RTC_ISR & RTC_ISR_INITS;
    }

    // Here we check for other temperature sensors
    if (LM75::isConnected() == true)
    {
      _externalTemperatureSensor = TempSensorType::LM75;
      startupDisplayBitmap |= 0x20;   // this will appear on the startup display
    }

    if (MCP9808::isConnected() == true)
    {
      _externalTemperatureSensor = TempSensorType::MCP9808;
      startupDisplayBitmap |= 0x10;   // this will appear on the startup display
    }
  }

  _rtcSetup();

  DisplayManager::initialize();
  DisplayManager::setDisplayBlanking(false);

  startupDisplay.setDisplayFromBitmap(startupDisplayBitmap);
  DisplayManager::writeDisplay(startupDisplay, startupDarkWhite);
  delay(250);
  startupDisplayBitmap = 0;
  startupDisplay.setDisplayColor0(startupOff);
  startupDisplay.setDisplayColor1(startupOff);
  startupDisplay.setDisplayFromBitmap(startupDisplayBitmap);
  DisplayManager::writeDisplay(startupDisplay, startupOff);
  delay(250);

  refresh();
  // set status LED off
  setStatusLed(startupOff);
}


// Should be called frequently from the main loop
//
void refresh()
{
  uint32_t totalLight = 0;

  iwdg_reset();

  // Add up some values so we can compute some averages...
  for (uint8_t i = 0; i < cAdcSamplesToAverage; i++)
  {
    totalLight += _adcSampleSetLight[i];
  }
  // Compute average light level and save it for access later
  if (cTargetHardwareVersion >= 2)
  {
    _lightLevel = totalLight / cAdcSamplesToAverage;
  }
  else
  {
    _lightLevel = 4095 - (totalLight / cAdcSamplesToAverage);
  }

  // Update time/date if it's the DS323x's turn
  if (_externalRtcConnected == true)
  {
    if (_lastRefreshWasRTC == false)
    {
      if (cTargetHardwareVersion >= 3)
      {
        _lastRefreshWasRTC = DS3234::refresh();

        _currentDateTime = DS3234::getDateTime();
        // _rtcIsSet = DS3234::isValid();
      }
      else
      {
        if (_externalTemperatureSensor == TempSensorType::DS3231)
        {
          _lastRefreshWasRTC = DS3231::refreshTimeDateTemp();
        }
        else
        {
          _lastRefreshWasRTC = DS3231::refreshTimeDate();
        }
        _currentDateTime = DS3231::getDateTime();
        // _rtcIsSet = DS3231::isValid();
      }
    }
  }
  else
  {
    uint32_t dr = RTC_DR, tr = RTC_TR;
    uint16_t year = 2000;
    uint8_t month, day, hour, minute, second;

    year += (10 * ((dr >> RTC_DR_YT_SHIFT) & RTC_DR_YT_MASK));
    year += ((dr >> RTC_DR_YU_SHIFT) & RTC_DR_YU_MASK);

    month = 10 * ((dr >> RTC_DR_MT_SHIFT) & RTC_DR_MT_MASK);
    month += ((dr >> RTC_DR_MU_SHIFT) & RTC_DR_MU_MASK);

    day = 10 * ((dr >> RTC_DR_DT_SHIFT) & RTC_DR_DT_MASK);
    day += ((dr >> RTC_DR_DU_SHIFT) & RTC_DR_DU_MASK);

    hour = 10 * ((tr >> RTC_TR_HT_SHIFT) & RTC_TR_HT_MASK);
    hour += ((tr >> RTC_TR_HU_SHIFT) & RTC_TR_HU_MASK);

    minute = 10 * ((tr >> RTC_TR_MNT_SHIFT) & RTC_TR_MNT_MASK);
    minute += ((tr >> RTC_TR_MNU_SHIFT) & RTC_TR_MNU_MASK);

    second = 10 * ((tr >> RTC_TR_ST_SHIFT) & RTC_TR_ST_MASK);
    second += ((tr >> RTC_TR_SU_SHIFT) & RTC_TR_SU_MASK);

    _currentDateTime.setDate(year, month, day);
    _currentDateTime.setTime(hour, minute, second);
    // _rtcIsSet = RTC_ISR & RTC_ISR_INITS;
    _lastRefreshWasRTC = true;  // this doesn't matter with the internal RTC
  }

  // Update temperature reading if it's the temperature sensor's turn
  if (_lastRefreshWasRTC == true)
  {
    if (cTargetHardwareVersion >= 3)
    {
      // Determine where to get the temperature from and get it
      switch (_externalTemperatureSensor)
      {
        case TempSensorType::DS3234:
          // Registers will have been refreshed above
          _lastRefreshWasRTC = false;
          _temperatureXcBaseMultiplier =  DS3234::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((DS3234::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        case TempSensorType::LM74:
          _lastRefreshWasRTC = !LM74::refresh();
          _temperatureXcBaseMultiplier =  LM74::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((LM74::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        case TempSensorType::DS1722:
          _lastRefreshWasRTC = !DS1722::refresh();
          _temperatureXcBaseMultiplier =  DS1722::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((DS1722::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        default:
          _lastRefreshWasRTC = false;
          _refreshInternalTemp();
          // break;
      }
    }
    else
    {
      // Determine where to get the temperature from and get it
      switch (_externalTemperatureSensor)
      {
        case TempSensorType::DS3231:
          // Registers will have been refreshed above
          _lastRefreshWasRTC = false;
          _temperatureXcBaseMultiplier =  DS3231::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((DS3231::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        case TempSensorType::LM75:
          _lastRefreshWasRTC = !LM75::refreshTemp();
          _temperatureXcBaseMultiplier =  LM75::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((LM75::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        case TempSensorType::MCP9808:
          _lastRefreshWasRTC = !MCP9808::refreshTemp();
          _temperatureXcBaseMultiplier =  MCP9808::getTemperatureWholePart() * cBaseMultiplier;
          _temperatureXcBaseMultiplier += ((MCP9808::getTemperatureFractionalPart() >> 1) * cTempFracMultiplier);
          break;

        default:
          _lastRefreshWasRTC = false;
          _refreshInternalTemp();
          // break;
      }
    }
  }

  // we do this here to give DMA time to complete before refreshing the display
  if (_adcSampleRefreshNow == true)
  {
    if (++_adcSampleCounter >= cAdcSamplesToAverage)
    {
      _adcSampleCounter = 0;
    }

    _adcSampleSetLight[_adcSampleCounter] = _bufferADC[cAdcPhototransistor];
    _adcSampleSetTemp[_adcSampleCounter] = _bufferADC[cAdcTemperature];
    _adcSampleSetVoltage[_adcSampleCounter] = _bufferADC[cAdcVref];

    _adcSampleRefreshNow = false;
  }

  DisplayManager::refresh();
}


bool tick()
{
  return tone(cToneFrequencyMinimum * 8, 2);
}


bool tone(const uint16_t frequency, const uint16_t duration)
{
  if (cTargetHardwareVersion >= 2)
  {
    // period = (48000000 / 8 (prescaler)) / frequency
    // oc_value = period / 2
    if (_toneTimer == 0)
    {
      if ((frequency >= cToneFrequencyMinimum) && (_toneVolume < cToneVolumeMaximum))
      {
        // proc speed divided by prescaler...
        uint32_t period = (48000000 / 8) / frequency,
                 ocValue = period / (2 << _toneVolume);
        timer_set_period(TIM1, period);
        timer_set_oc_value(TIM1, TIM_OC1, ocValue);
      }
      else
      {
        // timer_set_period(TIM1, 1);
        timer_set_oc_value(TIM1, TIM_OC1, 0);
      }

      _toneTimer = duration;

      return true;
    }
    else if (_toneTimerNext == 0)
    {
      _toneTimerNext = duration;
      _toneFrequencyNext = frequency;

      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (_toneTimer == 0)
    {
      if ((frequency >= cToneFrequencyMinimum) && (_toneVolume < cToneVolumeMaximum))
      {
        gpio_set(cBuzzerPort, cBuzzerPin);
      }
      else
      {
        gpio_clear(cBuzzerPort, cBuzzerPin);
      }

      _toneTimer = duration;

      return true;
    }
    else if (_toneTimerNext == 0)
    {
      _toneTimerNext = duration;
      _toneFrequencyNext = frequency;

      return true;
    }
    else
    {
      return false;
    }
  }
}


bool alarmInput(const uint8_t alarmInputNumber)
{
  if (alarmInputNumber < cAlarmInputPinCount)
  {
    // if the pin is low, return true (alarm is active if pin is pulled down)
    return (gpio_get(cAlarmInputPort, cAlarmInputPins[alarmInputNumber]) == 0);
  }

  return false;
}


bool button(const uint8_t button)
{
  if (_buttonStates & (1 << button))
  {
    return true;
  }

  return false;
}


uint8_t buttons()
{
  return _buttonStates;
}


uint8_t buttonsPressed()
{
  return _buttonStates & _buttonsChanged;
}


uint8_t buttonsReleased()
{
  return ~_buttonStates & _buttonsChanged;
}


void buttonsRefresh()
{
  uint32_t avg = 0;
  uint16_t mid = 0;
  uint8_t  button = 0, sample = 0;

  for (button = 0; button < cTscChannelCount; button++)
  {
    avg = 0;
    mid = (_tscMaximums[button] - _tscMinimums[button]) / 3;

    for (sample = 0; sample < cTscSamplesToAverage; sample++)
    {
      avg += _tscSampleSets[button][sample];
    }
    avg = avg / cTscSamplesToAverage;

    if ((avg < _tscMinimums[button] + mid) && (_tscMaximums[button] - _tscMinimums[button] > cTscMinimumDelta) && _tscMinimums[button] < cTscMaximumMinimum)
    {
      _buttonStates |= (1 << button);
    }
    else
    {
      _buttonStates &= ~(1 << button);
    }
  }

  _buttonsChanged = _buttonStates ^ _previousButtonStates;
  _previousButtonStates = _buttonStates;
}


void setLedCurrentDrive(const uint8_t drive)
{
  if (cTargetHardwareVersion >= 4)
  {
    DisplayManager::setDotCorrectionRange(drive > 1);
  }
  else
  {
    // check low bit
    if (drive & 1)
    {
      gpio_set(cCurrentDrivePort, cCurrentDriveLowPin);
    }
    else
    {
      gpio_clear(cCurrentDrivePort, cCurrentDriveLowPin);
    }

    // check high bit
    if (drive & 2)
    {
      gpio_set(cCurrentDrivePort, cCurrentDriveHighPin);
    }
    else
    {
      gpio_clear(cCurrentDrivePort, cCurrentDriveHighPin);
    }
  }
}


void setDisplayHardwareBlanking(const bool blankingState)
{
  if (_displayBlankingState != blankingState)
  {
    _displayBlankingState = blankingState;

    if (cTargetHardwareVersion >= 4)
    {
      if (blankingState == true)
      {
        // blank the display -- for TLC5951 XBLNK means NOT-BLANK, hence 'clear'
        gpio_clear(cBlankDisplayPort, cBlankDisplayPin);
      }
      else
      {
        // reset the timer's counter as PWM counters will start at 0 once XBLNK is set
        timer_generate_event(TIM2, TIM_EGR_UG);
        // unblank the display (the ISR, triggered by the above line, will do this)
        // gpio_set(cBlankDisplayPort, cBlankDisplayPin);
      }
    }
    else
    {
      if (blankingState == true)
      {
        // blank the display
        gpio_set(cBlankDisplayPort, cBlankDisplayPin);
      }
      else if (_displayBlankForWrite == false)
      {
        gpio_clear(cBlankDisplayPort, cBlankDisplayPin);
      }
    }
  }
}


bool dstState()
{
  return RTC_CR & RTC_CR_BKP;
}


DateTime getDateTime()
{
  return _currentDateTime;
}


void setDateTime(const DateTime &dateTime)
{
  _currentDateTime = dateTime;
  _rtcIsSet = true;

  if (_externalRtcConnected == true)
  {
    if (cTargetHardwareVersion >= 3)
    {
      DS3234::setDateTime(_currentDateTime);
    }
    else
    {
      DS3231::setDateTime(_currentDateTime);
    }
  }
  // we'll set the internal RTC regardless
  uint8_t dayOfWeek = _currentDateTime.dayOfWeek();

  // STM32 RTC does not accept 0; it uses 7 for Sunday, instead
  if (dayOfWeek == 0)
  {
    dayOfWeek = 7;
  }

  uint32_t dr = ((_currentDateTime.yearShort(true) & 0xff) << RTC_DR_YU_SHIFT) |
                ((_currentDateTime.month(true) & 0x1f) << RTC_DR_MU_SHIFT) |
                ((_currentDateTime.day(true) & 0x3f) << RTC_DR_DU_SHIFT) |
                ((dayOfWeek) << RTC_DR_WDU_SHIFT);

  uint32_t tr = ((_currentDateTime.hour(true, false) & 0x3f) << RTC_TR_HU_SHIFT) |
                ((_currentDateTime.minute(true) & 0x7f) << RTC_TR_MNU_SHIFT) |
                ((_currentDateTime.second(true) & 0x7f) << RTC_TR_SU_SHIFT);

  pwr_disable_backup_domain_write_protect();
  rtc_unlock();

  // enter init mode
  RTC_ISR |= RTC_ISR_INIT;
  while ((RTC_ISR & RTC_ISR_INITF) == 0);

  RTC_DR = dr;
  RTC_TR = tr;

  // exit init mode
  RTC_ISR &= ~(RTC_ISR_INIT);

  rtc_lock();
  rtc_wait_for_synchro();
  pwr_enable_backup_domain_write_protect();

  return;
}


bool rtcIsSet()
{
  return _rtcIsSet;
}


int32_t temperature(const bool fahrenheit, const bool bcd)
{
  if (fahrenheit)
  {
    if (bcd)
    {
      return uint32ToBcd((uint16_t)((_temperatureXcBaseMultiplier * 18) / 10000) + 32);
    }
    return ((_temperatureXcBaseMultiplier * 18) / 10000) + 32;
  }
  else
  {
    if (bcd)
    {
      return uint32ToBcd((uint16_t)_temperatureXcBaseMultiplier / cBaseMultiplier);
    }
    return _temperatureXcBaseMultiplier / cBaseMultiplier;
  }
}


uint16_t lightLevel()
{
  return _lightLevel;
}


void setDstState(const bool enableDst, const bool adjustRtcHardware)
{
  // number of seconds in an hour
  int16_t rtcAdjustment = 3600;
  // bit to set in RTC_CR to adjust the STM32's internal RTC
  uint32_t rtcDstAdjustBit = 0;
  // get the current state of the hardware into dstHwState
  bool dstHwState = RTC_CR & RTC_CR_BKP;
  // if the new state and the hardware state are not the same...
  if (enableDst != dstHwState)
  {
    // get ready to toggle the BKP bit...
    pwr_disable_backup_domain_write_protect();
    rtc_unlock();

    if (enableDst == true)
    {
      RTC_CR |= RTC_CR_BKP;

      rtcDstAdjustBit = RTC_CR_ADD1H;
    }
    else
    {
      RTC_CR &= ~RTC_CR_BKP;

      rtcDstAdjustBit = RTC_CR_SUB1H;

      rtcAdjustment *= -1;
    }

    if (adjustRtcHardware == true)
    {
      RTC_CR |= rtcDstAdjustBit;

      if (_externalRtcConnected == true)
      {
        if (cTargetHardwareVersion >= 3)
        {
          DS3234::setDateTime(_currentDateTime.addSeconds(rtcAdjustment));
        }
        else
        {
          DS3231::setDateTime(_currentDateTime.addSeconds(rtcAdjustment));
        }
      }

      DisplayManager::doubleBlink();
    }

    rtc_lock();
    rtc_wait_for_synchro();
    pwr_enable_backup_domain_write_protect();
  }
}


void setFlickerReduction(const uint16_t value)
{
  _blankingThreshold = value;
}


void setTemperatureCalibration(const int8_t value)
{
  _temperatureAdjustment = value;
}


void setVolume(const uint8_t volumeLevel)
{
  if (volumeLevel > cToneVolumeMaximum)
  {
    _toneVolume = 0;
  }
  else
  {
    _toneVolume = cToneVolumeMaximum - volumeLevel;
  }
}


uint32_t eraseFlash(uint32_t startAddress)
{
	uint32_t pageAddress = startAddress,
	         flashStatus = 0;

	// check if startAddress is in proper range
	if ((startAddress - FLASH_BASE) >= (cFlashPageSize * (cFlashPageNumberMaximum + 1)))
  {
    return 0xff;
  }

	// calculate current page address
  if ((startAddress % cFlashPageSize) != 0)
  {
    pageAddress -= (startAddress % cFlashPageSize);
  }

	flash_unlock();

	// Erase page(s)
	flash_erase_page(pageAddress);
	flashStatus = flash_get_status_flags();
	if (flashStatus == FLASH_SR_EOP)
  {
    flashStatus = 0;
  }

  flash_lock();

	return flashStatus;
}


void readFlash(uint32_t startAddress, uint16_t numElements, uint8_t *outputData)
{
	uint16_t i;
	uint32_t *memoryPtr = (uint32_t*)startAddress;

	for (i = 0; i < numElements / 4; i++)
	{
		*(uint32_t*)outputData = *(memoryPtr + i);
		outputData += 4;
	}
}


uint32_t writeFlash(uint32_t startAddress, uint8_t *inputData, uint16_t numElements)
{
	uint16_t i;
  const uint32_t erasedValue = 0xffffffff;
	uint32_t currentAddress = startAddress,
	         pageAddress = startAddress,
	         flashStatus = 0,
           returnStatus = 0;

	// check if startAddress is in proper range
	if ((startAddress - FLASH_BASE) >= (cFlashPageSize * (cFlashPageNumberMaximum + 1)))
  {
    return 0xff;
  }

	// calculate current page address
	if ((startAddress % cFlashPageSize) != 0)
  {
    pageAddress -= (startAddress % cFlashPageSize);
  }

	flash_unlock();

	// Erasing page
	flash_erase_page(pageAddress);
	flashStatus = flash_get_status_flags();
	if (flashStatus != FLASH_SR_EOP)
  {
    returnStatus = flashStatus;
  }
  else
  {
    // verify flash memory was completely erased
  	for (i = 0; i < numElements; i += 4)
  	{
      // verify if address was erased properly
  		if (*((uint32_t*)(currentAddress + i)) != erasedValue)
      {
        returnStatus = 0x40;
        break;
      }
    }

    if (returnStatus == 0)
    {
      // programming flash memory
    	for (i = 0; i < numElements; i += 4)
    	{
    		// programming word data
    		flash_program_word(currentAddress + i, *((uint32_t*)(inputData + i)));
    		flashStatus = flash_get_status_flags();
    		if (flashStatus != FLASH_SR_EOP)
        {
          returnStatus = 0x80 | flashStatus;
          break;
        }

    		// verify if correct data is programmed
    		if (*((uint32_t*)(currentAddress + i)) != *((uint32_t*)(inputData + i)))
        {
          returnStatus = 0x80 | FLASH_SR_PGERR;
          break;
        }
    	}
    }
  }

  flash_lock();

	return returnStatus;
}


bool i2cTransfer(const uint8_t addr, const uint8_t *bufferTx, size_t numberTx, uint8_t *bufferRx, size_t numberRx)
{
  // this could be arranged a little better but this way prevents the DMA
  //  complete interrupt from being called before the receive data is populated
  if (_i2cState != I2cState::I2cIdle)
  {
    if (_i2cBusyFailCount++ > cI2cMaxFailBusyCount)
    {
      _i2cRecover();
    }
    // Let the caller know it was busy if so
    return false;
  }
  else
  {
    if (numberTx > 0)
    {
      _i2cAddr = addr;
      _i2cBufferRx = bufferRx;
      _i2cNumberRx = numberRx;

      return i2cTransmit(addr, bufferTx, numberTx, (numberRx == 0));
    }
    else if (numberRx > 0)
    {
      return i2cReceive(addr, bufferRx, numberRx, true);
    }
  }
}


// Transfers the given buffers to/from the given peripheral through the SPI via DMA
//
bool i2cReceive(const uint8_t addr, uint8_t *bufferRx, const size_t numberRx, const bool autoEndXfer)
{
  if (_i2cState != I2cState::I2cIdle)
  {
    if (_i2cBusyFailCount++ > cI2cMaxFailBusyCount)
    {
      _i2cRecover();
    }
    // Let the caller know it was busy if so
    return false;
  }

  _i2cBusyFailCount = 0;

  if (numberRx > 0)
  {
    _i2cState = I2cState::I2cBusy;
    i2c_set_7bit_address(I2C1, addr);
    i2c_set_read_transfer_dir(I2C1);
    i2c_set_bytes_to_transfer(I2C1, numberRx);

    // Reset DMA channel
    dma_channel_reset(DMA1, DMA_CHANNEL7);

    // Set up rx dma, note it has higher priority to avoid overrun
    dma_set_peripheral_address(DMA1, DMA_CHANNEL7, (uint32_t)&I2C1_RXDR);
    dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)bufferRx);
    dma_set_number_of_data(DMA1, DMA_CHANNEL7, numberRx);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL7);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL7);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL7, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
    dma_set_priority(DMA1, DMA_CHANNEL7, DMA_CCR_PL_VERY_HIGH);

    // Enable dma transfer complete interrupt
  	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL7);

  	// Activate dma channel
  	dma_enable_channel(DMA1, DMA_CHANNEL7);

    i2c_send_start(I2C1);

    if (autoEndXfer == true)
    {
      /* important to do it afterwards to do a proper repeated start! */
      i2c_enable_autoend(I2C1);
    }
    else
    {
      i2c_disable_autoend(I2C1);
    }

  	/* Enable the I2C transfer via DMA
  	 * This will immediately start the transmission, after which when the receive
     * is complete, the receive DMA will activate
  	 */
  	i2c_enable_rxdma(I2C1);
  }

  return true;
}


// Transfers the given buffers to/from the given peripheral through the SPI via DMA
//
bool i2cTransmit(const uint8_t addr, const uint8_t *bufferTx, const size_t numberTx, const bool autoEndXfer)
{
  if (_i2cState != I2cState::I2cIdle)
  {
    if (_i2cBusyFailCount++ > cI2cMaxFailBusyCount)
    {
      _i2cRecover();
    }
    // Let the caller know it was busy if so
    return false;
  }

  _i2cBusyFailCount = 0;

  if (numberTx > 0)
  {
    _i2cState = I2cState::I2cBusy;
    /* Setting transfer properties */
    i2c_set_7bit_address(I2C1, addr);
    i2c_set_write_transfer_dir(I2C1);
    i2c_set_bytes_to_transfer(I2C1, numberTx);

    // Reset DMA channel
    dma_channel_reset(DMA1, DMA_CHANNEL6);

    // Set up tx dma
    dma_set_peripheral_address(DMA1, DMA_CHANNEL6, (uint32_t)&I2C1_TXDR);
    dma_set_memory_address(DMA1, DMA_CHANNEL6, (uint32_t)bufferTx);
    dma_set_number_of_data(DMA1, DMA_CHANNEL6, numberTx);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL6);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL6);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL6, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);
    dma_set_priority(DMA1, DMA_CHANNEL6, DMA_CCR_PL_HIGH);

    // Enable dma transfer complete interrupt
  	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL6);

  	// Activate dma channel
  	dma_enable_channel(DMA1, DMA_CHANNEL6);

    if (autoEndXfer == true)
    {
      i2c_enable_autoend(I2C1);
    }
    else
    {
      i2c_disable_autoend(I2C1);
    }

    /* start transfer */
    i2c_send_start(I2C1);

  	/* Enable the I2C transfer via DMA
  	 * This will immediately start the transmission, after which when the receive
     * is complete, the receive DMA will activate
  	 */
  	i2c_enable_txdma(I2C1);
  }

  return true;
}


void i2cAbort()
{
  _i2cRecover();
}


bool i2cIsBusy()
{
  return (_i2cState != I2cState::I2cIdle);
}


bool readSerial(const uint32_t usart, const uint32_t length, char* data)
{
  switch (usart)
  {
    case USART2:
    if ((DMA1_ISR & DMA_ISR_TCIF6) || !(USART2_CR3 & USART_CR3_DMAR))
    {
      dma_channel_reset(DMA1, DMA_CHANNEL6);
      // Set up rx dma, note it has higher priority to avoid overrun
      dma_set_peripheral_address(DMA1, DMA_CHANNEL6, (uint32_t)&USART2_RDR);
      dma_set_memory_address(DMA1, DMA_CHANNEL6, (uint32_t)data);
      dma_set_number_of_data(DMA1, DMA_CHANNEL6, length);
      dma_set_read_from_peripheral(DMA1, DMA_CHANNEL6);
      dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL6);
      dma_set_peripheral_size(DMA1, DMA_CHANNEL6, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(DMA1, DMA_CHANNEL6, DMA_CCR_PL_VERY_HIGH);

      // Enable dma transfer complete interrupt
    	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL6);

    	// Activate dma channel
    	dma_enable_channel(DMA1, DMA_CHANNEL6);

      usart_enable_rx_dma(USART2);
    }
    else
    {
      return false;
    }
    break;

    default:
    if ((DMA1_ISR & DMA_ISR_TCIF5) || !(USART1_CR3 & USART_CR3_DMAR))
    {
      dma_channel_reset(DMA1, DMA_CHANNEL5);
      // Set up rx dma, note it has higher priority to avoid overrun
      dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t)&USART1_RDR);
      dma_set_memory_address(DMA1, DMA_CHANNEL5, (uint32_t)data);
      dma_set_number_of_data(DMA1, DMA_CHANNEL5, length);
      dma_set_read_from_peripheral(DMA1, DMA_CHANNEL5);
      dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);
      dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_VERY_HIGH);

      // Enable dma transfer complete interrupt
    	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);

    	// Activate dma channel
    	dma_enable_channel(DMA1, DMA_CHANNEL5);

      usart_enable_rx_dma(USART1);
    }
    else
    {
      return false;
    }
    // break;
  }
  return true;
}


bool writeSerial(const uint32_t usart, const uint32_t length, const char* data)
{
  switch (usart)
  {
    case USART2:
    if (USART2_ISR & USART_ISR_TC)
    {
      dma_channel_reset(DMA1, DMA_CHANNEL7);
      // Set up tx dma
      dma_set_peripheral_address(DMA1, DMA_CHANNEL7, (uint32_t)&USART1_TDR);
      dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)data);
      dma_set_number_of_data(DMA1, DMA_CHANNEL7, length);
      dma_set_read_from_memory(DMA1, DMA_CHANNEL7);
      dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL7);
      dma_set_peripheral_size(DMA1, DMA_CHANNEL7, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(DMA1, DMA_CHANNEL7, DMA_CCR_PL_HIGH);

      // Enable dma transfer complete interrupt
    	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL7);

    	// Activate dma channel
    	dma_enable_channel(DMA1, DMA_CHANNEL7);

      usart_enable_tx_dma(USART2);
    }
    else
    {
      return false;
    }
    break;

    default:
    if (USART1_ISR & USART_ISR_TC)
    {
      dma_channel_reset(DMA1, DMA_CHANNEL4);
      // Set up tx dma
      dma_set_peripheral_address(DMA1, DMA_CHANNEL4, (uint32_t)&USART1_TDR);
      dma_set_memory_address(DMA1, DMA_CHANNEL4, (uint32_t)data);
      dma_set_number_of_data(DMA1, DMA_CHANNEL4, length);
      dma_set_read_from_memory(DMA1, DMA_CHANNEL4);
      dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL4);
      dma_set_peripheral_size(DMA1, DMA_CHANNEL4, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(DMA1, DMA_CHANNEL4, DMA_CCR_PL_HIGH);

      // Enable dma transfer complete interrupt
    	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL4);

      USART1_ICR |= USART_ICR_TCCF;

    	// Activate dma channel
    	dma_enable_channel(DMA1, DMA_CHANNEL4);

      usart_enable_tx_dma(USART1);
    }
    else
    {
      return false;
    }
    // break;
  }
  return true;
}


// Transfers the given buffers to/from the given peripheral through the SPI via DMA
//
bool spiTransfer(const SpiPeripheral peripheral, uint8_t *bufferIn, uint8_t *bufferOut, const uint16_t length, const bool use16BitXfers)
{
  uint16_t mSize = DMA_CCR_MSIZE_8BIT, pSize = DMA_CCR_PSIZE_8BIT;
  volatile uint8_t temp_data __attribute__ ((unused));

  if ((_spiState != SpiState::SpiIdle) ||
      (_ledDriverSrState == DriverShiftRegState::SrContainsGsData))
  {
    // Let the caller know it was busy if so
    return false;
  }

  _spiState = SpiState::SpiBusyPeripheral;

  if (use16BitXfers == true)
  {
    mSize = DMA_CCR_MSIZE_16BIT;
    pSize = DMA_CCR_PSIZE_16BIT;
  }

	// Reset SPI data and status registers
	// First, ensure the SPI is not busy...
  while (SPI_SR(SPI1) & (SPI_SR_BSY));
  // ...now we purge the FIFO to ensure it's empty for the new inbound bits.
	while (SPI_SR(SPI1) & (SPI_SR_RXNE | SPI_SR_OVR))
  {
		temp_data = SPI_DR(SPI1);
	}

  // Configure SPI1 for use with the appropriate peripheral
  _spiSelectPeripheral(peripheral);

  // Reset DMA channels
  dma_channel_reset(DMA1, DMA_CHANNEL2);
  dma_channel_reset(DMA1, DMA_CHANNEL3);

  // Set up rx dma, note it has higher priority to avoid overrun
  dma_set_peripheral_address(DMA1, DMA_CHANNEL2, (uint32_t)&SPI1_DR);
  dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t)bufferIn);
  dma_set_number_of_data(DMA1, DMA_CHANNEL2, length);
  dma_set_read_from_peripheral(DMA1, DMA_CHANNEL2);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL2);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL2, pSize);
  dma_set_memory_size(DMA1, DMA_CHANNEL2, mSize);
  dma_set_priority(DMA1, DMA_CHANNEL2, DMA_CCR_PL_VERY_HIGH);

  // Set up tx dma
  dma_set_peripheral_address(DMA1, DMA_CHANNEL3, (uint32_t)&SPI1_DR);
  dma_set_memory_address(DMA1, DMA_CHANNEL3, (uint32_t)bufferOut);
  dma_set_number_of_data(DMA1, DMA_CHANNEL3, length);
  dma_set_read_from_memory(DMA1, DMA_CHANNEL3);
  dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL3);
  dma_set_peripheral_size(DMA1, DMA_CHANNEL3, pSize);
  dma_set_memory_size(DMA1, DMA_CHANNEL3, mSize);
  dma_set_priority(DMA1, DMA_CHANNEL3, DMA_CCR_PL_HIGH);

	// Enable dma transfer complete interrupts
	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);
	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);

	// Activate dma channels
	dma_enable_channel(DMA1, DMA_CHANNEL2);
	dma_enable_channel(DMA1, DMA_CHANNEL3);

  switch (peripheral)
  {
    case SpiPeripheral::LedDriversGs:
      gpio_clear(cNssPort, cNssDisplayPin);
      _spiState = SpiState::SpiBusyDisplay;
      _ledDriverSrState = DriverShiftRegState::SrLoadingGsData;

      if (cTargetHardwareVersion <= 3)
      {
        // Attempt to reduce flicker at very low intensities
        if ((Application::getIntensityAutoAdjust() == false)
            || (DisplayManager::getMasterIntensity() > _blankingThreshold))
        {
          _displayBlankForWrite = true;
          gpio_set(cBlankDisplayPort, cBlankDisplayPin);
        }
      }
      break;

    case SpiPeripheral::LedDriversOther:
      gpio_set(cNssPort, cNssDisplayPin);
      _spiState = SpiState::SpiBusyDisplay;
      _ledDriverSrState = DriverShiftRegState::SrLoadingOtherData;
      break;

    case SpiPeripheral::Rtc:
      gpio_clear(cNssPort, cNssRtcPin);
      break;

    case SpiPeripheral::TempSensor:
      if (_invertCsForTemperatureSensor == true)
      {
        gpio_set(cNssPort, cNssTemperaturePin);
      }
      else
      {
        gpio_clear(cNssPort, cNssTemperaturePin);
      }
      break;

    default:
      break;
  }

	/* Enable the SPI transfer via DMA
	 * This will immediately start the transmission, after which when the receive
   * is complete, the receive DMA will activate
	 */
	spi_enable_rx_dma(SPI1);
	spi_enable_tx_dma(SPI1);

  return true;
}


bool spiIsBusy()
{
  return _spiState != SpiState::SpiIdle;
}


void setBlueLed(const uint32_t intensity)
{
  if (intensity <= Display::cLedMaxIntensity)
  {
    timer_set_oc_value(cLedPwmTimer, cLed2PwmOc, intensity);
  }
  else
  {
    timer_set_oc_value(cLedPwmTimer, cLed2PwmOc, Display::cLedMaxIntensity);
  }
}


void setGreenLed(const uint32_t intensity)
{
  if (intensity <= Display::cLedMaxIntensity)
  {
    timer_set_oc_value(cLedPwmTimer, cLed1PwmOc, intensity);
  }
  else
  {
    timer_set_oc_value(cLedPwmTimer, cLed1PwmOc, Display::cLedMaxIntensity);
  }
}


void setRedLed(const uint32_t intensity)
{
  if (intensity <= Display::cLedMaxIntensity)
  {
    timer_set_oc_value(cLedPwmTimer, cLed0PwmOc, intensity);
  }
  else
  {
    timer_set_oc_value(cLedPwmTimer, cLed0PwmOc, Display::cLedMaxIntensity);
  }
}


void setStatusLed(const RgbLed led)
{
  setBlueLed(led.getBlue());
  setGreenLed(led.getGreen());
  setRedLed(led.getRed());
}


void blinkStatusLed(const RgbLed led1, const RgbLed led2, uint32_t numberOfBlinks, const uint32_t delayLength)
{
  // be sure this is off so blinking is visible
  DisplayManager::setStatusLedAutoRefreshing(false);

  while (numberOfBlinks-- > 0)
  {
    setStatusLed(led1);
    delay(delayLength);
    setStatusLed(led2);
    delay(delayLength);
  }
}


void delay(const uint32_t length)
{
  _delayCounter = 0;

  while (_delayCounter < length)
  {
    DisplayManager::refresh();
  }
}


uint32_t bcdToUint32(uint32_t bcdValue)
{
  uint32_t multiplier = 1, result = 0;

  while (bcdValue != 0)
  {
    result += multiplier * (bcdValue & 0xf);
    multiplier *= 10;
    bcdValue = bcdValue >> 4;
  }
  return result;
}


uint32_t uint32ToBcd(uint32_t uint32Value)
{
    uint32_t result = 0;
    uint8_t shift = 0;

    if (uint32Value > 99999999)
    {
      uint32Value = 0;
    }

    while (uint32Value != 0)
    {
        result +=  (uint32Value % 10) << shift;
        uint32Value = uint32Value / 10;
        shift += 4;
    }
    return result;
}


void dmaIsr()
{
  if ((DMA1_ISR & DMA_ISR_TCIF2) || (DMA1_ISR & DMA_ISR_TCIF3))
  {
    if (DMA1_ISR & DMA_ISR_TCIF2)
  	{
  		DMA1_IFCR |= DMA_IFCR_CTCIF2;

  		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);

  		spi_disable_rx_dma(SPI1);

  		dma_disable_channel(DMA1, DMA_CHANNEL2);
  	}

  	if (DMA1_ISR & DMA_ISR_TCIF3)
  	{
  		DMA1_IFCR |= DMA_IFCR_CTCIF3;

  		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);

  		spi_disable_tx_dma(SPI1);

  		dma_disable_channel(DMA1, DMA_CHANNEL3);
  	}

    // wait for the last bits to roll out
    while (SPI_SR(SPI1) & SPI_SR_BSY);

    switch (_spiState)
    {
      case SpiState::SpiBusyPeripheral:
        // release the NSS line(s)
        if (_invertCsForTemperatureSensor == true)
        {
          gpio_set(cNssPort, cNssRtcPin);
          gpio_clear(cNssPort, cNssTemperaturePin);
        }
        else
        {
          gpio_set(cNssPort, cNssRtcPin | cNssTemperaturePin);
        }

        _spiState = SpiState::SpiIdle;

        break;

      case SpiState::SpiBusyDisplay:
        if (_ledDriverSrState == DriverShiftRegState::SrLoadingOtherData)
        {
          gpio_clear(cNssPort, cNssDisplayPin);
          _ledDriverSrState = DriverShiftRegState::SrAvailable;
          _spiState = SpiState::SpiIdle;
          gpio_set(cNssPort, cNssDisplayPin);
        }
        else if (_ledDriverSrState == DriverShiftRegState::SrLoadingGsData)
        {
          // for version 4+, the timer interrupt takes care of latching GS data
          if (cTargetHardwareVersion >= 4)
          {
            _ledDriverSrState = DriverShiftRegState::SrContainsGsData;
          }
          // ...for pre-v4 hardware, we do it here
          else
          {
            gpio_set(cNssPort, cNssDisplayPin);
            _ledDriverSrState = DriverShiftRegState::SrAvailable;
            if (_displayBlankingState == false)
            {
              gpio_clear(cBlankDisplayPort, cBlankDisplayPin);
            }
            _displayBlankForWrite = false;
          }
        }

        _spiState = SpiState::SpiIdle;

        break;
    }
  }

  if (DMA1_ISR & DMA_ISR_TCIF4)
	{
		DMA1_IFCR |= DMA_IFCR_CTCIF4;

		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL4);

    if (cTargetHardwareVersion >= 3)
    {
      usart_disable_tx_dma(USART1);
    }
    else
    {
      usart_disable_tx_dma(USART2);
    }

		dma_disable_channel(DMA1, DMA_CHANNEL4);
	}

  if (DMA1_ISR & DMA_ISR_TCIF5)
	{
		DMA1_IFCR |= DMA_IFCR_CTCIF5;

		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL5);

    if (cTargetHardwareVersion >= 3)
    {
      usart_disable_rx_dma(USART1);
    }
    else
    {
      usart_disable_rx_dma(USART2);

      Dmx512Rx::rxCompleteIsr();
    }

		dma_disable_channel(DMA1, DMA_CHANNEL5);
	}

  if (DMA1_ISR & DMA_ISR_TCIF6)
	{
		DMA1_IFCR |= DMA_IFCR_CTCIF6;

		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL6);

    if (cTargetHardwareVersion >= 3)
    {
      usart_disable_rx_dma(USART2);

      Dmx512Rx::rxCompleteIsr();
    }
    else
    {
      i2c_disable_txdma(I2C1);

      _i2cState = I2cState::I2cIdle;

      if (_i2cNumberRx > 0)
      {
        i2cReceive(_i2cAddr, _i2cBufferRx, _i2cNumberRx, true);
      }
    }

		dma_disable_channel(DMA1, DMA_CHANNEL6);
	}

  if (DMA1_ISR & DMA_ISR_TCIF7)
	{
		DMA1_IFCR |= DMA_IFCR_CTCIF7;

		dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL7);

    if (cTargetHardwareVersion >= 3)
    {
      usart_disable_tx_dma(USART2);
    }
    else
    {
      i2c_disable_rxdma(I2C1);

      _i2cState = I2cState::I2cIdle;

      _i2cBufferRx = nullptr;
      _i2cNumberRx = 0;
    }

		dma_disable_channel(DMA1, DMA_CHANNEL7);
	}
}


void systickIsr()
{
  // used by delay()
  _delayCounter++;

  // update samples
  if (_adcSampleTimer++ >= cAdcSampleInterval)
  {
    _adcSampleTimer = 0;
    _adcSampleRefreshNow = true;
  }

  // update the tone timer and turn off the tone if the timer has expired
  if (_toneTimer > 0)
  {
    if (--_toneTimer == 0)
    {
      if (_toneTimerNext > 0)
      {
        tone(_toneFrequencyNext, _toneTimerNext);
        _toneFrequencyNext = 0;
        _toneTimerNext = 0;
      }
      else
      {
        if (cTargetHardwareVersion >= 2)
        {
          timer_set_oc_value(TIM1, TIM_OC1, 0);
        }
        else
        {
          gpio_clear(cBuzzerPort, cBuzzerPin);
        }
      }
    }
  }
}


void tim2Isr()
{
	if (TIM2_SR & TIM_SR_UIF)
	{
    // clear the event flag
		TIM2_SR &= ~TIM_SR_UIF;
    // first, blank the display, resetting PWM counters
    gpio_clear(cBlankDisplayPort, cBlankDisplayPin);
    // if new data is waiting to be latched, latch it in now
    if (_ledDriverSrState == DriverShiftRegState::SrContainsGsData)
    {
      gpio_set(cNssPort, cNssDisplayPin);
      _ledDriverSrState = DriverShiftRegState::SrAvailable;
    }
    // if the display is not blanked right now, unblank it
    if (DisplayManager::getDisplayBlanking() == false)
    {
      gpio_set(cBlankDisplayPort, cBlankDisplayPin);
    }
	}
}


void tim15Isr()
{

}


void tim16Isr()
{

}


void tscIsr()
{
  // process acquisition, advance to next channels, and trigger another acquisition
  if (TSC_ISR & TSC_ISR_EOAF)
  {
    // ensure the counter is in bounds -- should already be as initialized above
    // if (_tscChannelCounter >= cTscChannelsPerGroup)
    // {
    //   _tscChannelCounter = 0;
    // }

    if (cTargetHardwareVersion == 4)
    {
      _tscAcquisitionValues[_tscChannelCounter + cTscChannelsPerGroup] = TSC_IOGxCR(6);
      _tscAcquisitionValues[_tscChannelCounter] = TSC_IOGxCR(3);
    }
    else if ((cTargetHardwareVersion == 3) || (cTargetHardwareVersion == 2))
    {
      _tscAcquisitionValues[_tscChannelCounter + cTscChannelsPerGroup * 2] = TSC_IOGxCR(6);
      _tscAcquisitionValues[_tscChannelCounter + cTscChannelsPerGroup] = TSC_IOGxCR(5);
      _tscAcquisitionValues[_tscChannelCounter] = TSC_IOGxCR(3);
    }
    else if (cTargetHardwareVersion == 1)
    {
      _tscAcquisitionValues[_tscChannelCounter + cTscChannelsPerGroup] = TSC_IOGxCR(4);
      _tscAcquisitionValues[_tscChannelCounter] = TSC_IOGxCR(2);
    }


    for (uint8_t i = _tscChannelCounter; i < cTscChannelCount; i += cTscChannelsPerGroup)
    {
      // if the value we just got is less than the known minimum...
      if (_tscAcquisitionValues[i] < _tscMinimums[i])
      {
        // ...update the minimum with the new minimum
        _tscMinimums[i] = _tscAcquisitionValues[i];
      }

      // if the value we just got is greater than the known maximum...
      if (_tscAcquisitionValues[i] > _tscMaximums[i])
      {
        // ...update the maximum with the new value
        _tscMaximums[i] = _tscAcquisitionValues[i];
      }

      // update the sample set with the value we just got
      _tscSampleSets[i][_tscSampleCounter] = _tscAcquisitionValues[i];
    }

    if (++_tscChannelCounter >= cTscChannelsPerGroup)
    {
      _tscChannelCounter = 0;

      if (++_tscSampleCounter >= cTscSamplesToAverage)
      {
        _tscSampleCounter = 0;
      }
    }

    // select the next pair of channels to sample
    TSC_IOCCR = cTscChannelControlBits[_tscChannelCounter];

    TSC_ICR = TSC_ICR_EOAIC;

    /* kick off the next acquisition cycle */
    TSC_CR |= TSC_CR_START;
  }

  // deal with max count error if it happened
  if (TSC_ISR & TSC_ISR_MCEF)
  {
    TSC_ICR = TSC_ICR_MCEIC;
  }
}


void usart1Isr()
{

}


void usart2Isr()
{

}


}

}
