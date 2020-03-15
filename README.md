TM1637-no-delay
===============
Arduino library for TM1637 (LED Driver) forked from [https://github.com/avishorp/TM1637](avishorp/TM1637)

Description
-----------
An Arduino library for 7-segment display modules based on the TM1637 chip, such as Seeed Studio's [Grove 4 digit display](http://www.seeedstudio.com/depot/grove-4digit-display-p-1198.html). The TM1637 chip also has keyboard input capability, but it's not implemented in this library.

Implemented without any wait function calls (such as `delayMicroseconds()`). A new interface is provided to update the display without pausing your event loop. Designed to be compatible with the [https://sensorium.github.io/Mozzi/](Mozzi Sound Library). Interface changes are italicized below for quick reference. 

Hardware Connection
-------------------
The display modules has two signal connection (and two power connections) which are CLK and DIO. These pins can be connected to any pair of digital pins on the Arduino. When an object is created, the pins should be configured. There is no limitation on the number of instances used concurrently (as long as each instance has a pin pair of its own)

Installation
------------
The library is installed as any Arduino library, by copying the files into a directory on the library search path of the Arduino IDE

Usage
-----
The library provides a single class named TM1637Display. An instance of this class provides the following functions:

* `setSegments` - Set the raw value of the segments of each digit
* `showNumberDec` - Display a decimal number
* `showNumberDecEx` - Display a decimal number with decimal points or colon
* `setBrightness` - Sets the brightness of the display
* `update` - *Perform one step of the current data transmission*

*`update` will return true when no data is set for transmission. After calling `setSegments` or either `showNumber` functions, `update` will return false until that transmission has been completed (68 times to write all 4 cells). To replicate the library default transmission speed, call `update` at a rate of every 100 microseconds. Being called in Mozzi's `updateControl` with the default `CONTROL_RATE 64` (every 15625 microseconds) results in slow but usable operation. For more information on timing tolerance consult the [https://www.mcielectronics.cl/website_MCI/static/documents/Datasheet_TM1637.pdf](datasheet). * 
Please refer to TM1637Display.h for more information. Two examples are included, one using the Arduino `loop` function to call `update` and the other using Mozzi's `updateControl`. The Mozzi sound library (tested with version 1.0.3 but 1.1.0 should work) must be reachable by your build process for the second example.
