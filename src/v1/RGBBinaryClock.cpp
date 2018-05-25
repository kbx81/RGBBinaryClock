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

#include <libopencm3/cm3/nvic.h>
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


/* I2C and USART transmit/receive completed with DMA */
void dma1_channel4_5_isr()
{
	Hardware::dmaIsr();
}


/* Called when systick fires */
void sys_tick_handler(void)
{
	Hardware::systickIsr();
}


/* Timer 15 interrupt -- used for DMX-512 break length measurement */
void tim15_isr()
{
  Hardware::tim15Isr();
}


/* Timer 16 interrupt -- used for DMX-512 signal monitoring */
void tim16_isr()
{
  Hardware::tim16Isr();
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


/* Where all life begins...more or less */
int main()
{
	Hardware::initialize();

  Application::initialize();

  Application::loop();
}
