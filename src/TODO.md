# kbx's RGB Binary Clock

## TODO List and Issue Tracking

### Known issues

1. Clock is advanced one hour if set during DST after a completely cold-start
 when DST auto-adjusting is enabled. _DST adjustments should perhaps not be
 handled by TimeDateTempView. Determine a better way to manage this._
1. DMX-512 packet hand-off is dumb. Requests for received data should return an
 object that is a single DMX-512 packet, not return values from an array that
 might be changing.
1. In version 2.x hardware, DMA channels are shared between USART1 and USART2;
 calls to send and receive functions may squash the other's DMA. This works with
 the code as-is at the time of this writing, but will likely present problems
 and create confusion for others using the code. This should be handled better.
