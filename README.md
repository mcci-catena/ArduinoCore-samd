# Arduino Core for MCCI Catena&reg; IoT Nodes based on the SAMD21 CPU

This repository contains the source code and configuration files of the Arduino Core
for MCCI Catena IoT devices.  It is based on the Arduino core
for Atmel's SAMD21 processor (used on the Arduino/Genuino Zero, MKR1000 and MKRZero boards),
as extended by Adafruit for their SAMD Boards such as the Feather M0.

[![GitHub release](https://img.shields.io/github/release/mcci-catena/ArduinoCore-samd.svg)](https://github.com/mcci-catena/ArduinoCore-samd/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/mcci-catena/ArduinoCore-samd/latest.svg)](https://github.com/mcci-catena/ArduinoCore-samd/compare/v1.1.0...master)

**Contents:**
<!-- TOC depthFrom:2 updateOnSave:true -->

- [Installation on Arduino IDE](#installation-on-arduino-ide)
- [Bugs or Issues](#bugs-or-issues)
- [Release History](#release-history)
- [License and credits](#license-and-credits)
	- [LGPL License](#lgpl-license)
	- [Trademark Acknowledgements](#trademark-acknowledgements)

<!-- /TOC -->
## Installation on Arduino IDE

Simply install the package following the instructions given here:

https://github.com/mcci-catena/arduino-boards/blob/master/README.md

## Bugs or Issues

If you find a bug you can submit an issue here on github:

https://github.com/mcci-catena/ArduinoCore-samd/issues

or if it is an issue with the upstream:

https://github.com/adafruit/ArduinoCore-samd/issues

or

https://github.com/arduino/ArduinoCore-samd/issues

Before posting a new issue, please check if the same problem has been already reported by someone else
to avoid duplicates.

## Release History

- v1.1.0 adds support for the MCCI Catena 4420 and 4470. It also adds the ability to select the target LoRaWAN region from the IDE Tools menu (assuming that you're using the [MCCI Catena arduino-lmic](https://github.com/mcci-catena/arduino-lmic) library). It also fixes the USB product ID numbers to match the official numbers assigned by MCCI release engineering (issue [#9](https://github.com/mcci-catena/ArduinoCore-samd/issues/9)).

## License and credits

This core has been developed by Arduino LLC in collaboration with Atmel. The copyright notice doesn't
include Adafruit, but we think it probably should.

### LGPL License
```text
  Copyright (c) 2015 Arduino LLC.  All rights reserved.
  Portions copyright (c) Adafruit Industries.
  Portions copyright (c) 2017-2018 MCCI Corporation.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
```

### Trademark Acknowledgements

MCCI and MCCI Catena are registered trademarks of MCCI Corporation. LoRaWAN is a trademark of the LoRa Alliance. All other trademarks are the properties of their respective owners.