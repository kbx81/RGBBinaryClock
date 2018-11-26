# kbx's RGB Binary Clock

## TODO List and Issue Tracking

### Known issues

1. In version 2.x hardware, DMA channels are shared between USART1 and USART2;
 calls to send and receive functions may squash the other's DMA. This works with
 the code as-is at the time of this writing, but will likely present problems
 and create confusion for others using the code. This should be handled better.


## General TODO

1. Standardized display brightness control via DisplayManager
1. Implement option for MSbs to be off
1. Better/"learning" automatic brightness control


## v4 TODO

1. Display brightness adjustments are not smooth at very low levels
1. Better match status LED colors with main display colors
1. Color correction tweaks using DC?
