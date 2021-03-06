# Privacy Seed - Lights controller

LED controller based on Arduino Nano, FastLED and DotStar
This component is part of the [Privacy Seed](https://privacy-seed.org/) project.

## Wiring:
Supports 2 separate LED strips.

Tree base lights (53 LEDs):
-  Data pin  (green):  4
-  Clock pin (yellow):  5

Tree top and heart lights (25 LEDs):
-  Data pin  (green):  6
-  Clock pin (yellow):  7


## TODO:
- Refactor read to event based
- Investigate strips stopping with a rainbow error
-

## Serial test inputs:

Test sensor not covered
```
 echo -en '\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' > /dev/ttyUSB0
```

Test sensor read/noise - repeat quick
```
 echo -en '\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17\x01\x12\x01\x13\x01\x14\x01\x15\x01\x16\x01\x17' > /dev/ttyUSB0
```

Test beat pattern followed by sensor read/noise
```
 echo -en '\x02\x42\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04' > /dev/ttyUSB0
```

Test 2 beat patterns
```
 echo -en '\x02\x42\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x02\x43\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03\x01\x04\x01\x05\x01\x01\x01\x02\x01\x03' > /dev/ttyUSB0
```

Test beat pattern followed by sensor not covered
```
 echo -en '\x02\x42\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' > /dev/ttyUSB0
```

Don't try! Seriously :)
```
 echo -en '\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03\x03' > /dev/ttyUSB0
```
