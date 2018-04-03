//
// kbx81's binary clock
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

#include <cstdint>
#include <stdio.h>
#include <errno.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/tsc.h>
#include <libopencm3/stm32/usart.h>
#include "Hardware.h"
#include "Application.h"


using namespace kbxBinaryClock;


/* ADC DMA */
void dma1_channel1_isr(void)
{
	Hardware::dmaIsr();
}


/* SPI receive completed with DMA */
void dma1_channel2_3_isr(void)
{
	Hardware::dmaIsr();
}


void dma1_channel4_5_isr()
{
	Hardware::dmaIsr();
}


/* Called when systick fires */
void sys_tick_handler(void)
{
	Hardware::systickIsr();
}


/* TSC */
void tsc_isr(void)
{
	Hardware::tscIsr();
}


/* USART1 */
void usart1_isr(void)
{
	Hardware::usart1Isr();
}


/* USART1 */
void usart2_isr(void)
{
	Hardware::usart2Isr();
}


int main()
{
	Hardware::initialize();

  Application::initialize();

  Application::loop();
}
