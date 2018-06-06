# kbx's RGB Binary Clock

## TODO List and Issue Tracking

### Known issues

1. Automatic display intensity adjustments sometimes cause a little flicker at
 intermediate intensities. _Fix it._
1. Some type of alarm for the Timer/Counter might be nice.
1. In version 2.x hardware, DMA channels are shared between USART1 and USART2;
 calls to send and receive functions may squash the other's DMA. This works with
 the code as-is at the time of this writing, but will likely present problems
 and create confusion for others using the code. This should be handled better.
