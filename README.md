# Arduino Core for MCCI Catena&reg; IoT Nodes based on the SAMD21 CPU

This repository contains the source code and configuration files of the Arduino Core
for MCCI Catena IoT devices.  It is based on the Arduino core
for Atmel's SAMD21 processor (used on the Arduino/Genuino Zero, MKR1000 and MKRZero boards),
as extended by Adafruit for their SAMD Boards such as the Feather M0, and further extended by MCCI to support its MCCI Catena LoRaWAN&reg; technology boards.

[![GitHub release](https://img.shields.io/github/release/mcci-catena/ArduinoCore-samd.svg)](https://github.com/mcci-catena/ArduinoCore-samd/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/mcci-catena/ArduinoCore-samd/latest.svg)](https://github.com/mcci-catena/ArduinoCore-samd/compare/v2.2.0...master)

**Contents:**
<!-- markdownlint-disable MD033 -->
<!-- markdownlint-capture -->
<!-- markdownlint-disable -->
<!-- TOC depthFrom:2 updateOnSave:true -->

- [Installing this Package](#installing-this-package)
    - [Installing from the IDE](#installing-from-the-ide)
    - [Installing a Development Copy of this BSP](#installing-a-development-copy-of-this-bsp)
- [Bugs or Issues](#bugs-or-issues)
- [Release History](#release-history)
- [License and credits](#license-and-credits)
    - [LGPL License](#lgpl-license)
    - [Trademark Acknowledgements](#trademark-acknowledgements)
    - [Support Open-Source Software, Hardware, and Community IoT](#support-open-source-software-hardware-and-community-iot)

<!-- /TOC -->
<!-- markdownlint-restore -->
<!-- Due to a bug in Markdown TOC, the table is formatted incorrectly if tab indentation is set other than 4. Due to another bug, this comment must be *after* the TOC entry. -->

## Installing this Package

### Installing from the IDE

The easy approach is to install the package following the instructions given here:

[`https://github.com/mcci-catena/arduino-boards/blob/master/README.md`](https://github.com/mcci-catena/arduino-boards/blob/master/README.md)

However, if you need to development, it's more complicated. See the following section.

### Installing a Development Copy of this BSP

If you want to develop and test changes to this package, we suggest the following.

1. Install the current release from the standard location using the Arduino `Tools>Boards>Boards Manager...` menu. This installs tools and so forth.
2. Select a board supported by this package.
3. Create an empty sketch with Arduino `File>New`, and check that you can build.
4. Close the Arduino IDE.
5. Clone this repo to a convenient spot on your computer.
6. Open a command window and go to the Arduino board installation directory. It will be one of the following.

   | System | Location |
   |:------:|:---------|
   | Windows 7 through 10 | <code>c:\Users\\<em><strong>username</strong></em>\AppData\Local\Arduino15</code>. If you're using git bash, you can also use `~/AppData/Local/Arduino15` |
   | macOS | <code>~/Library/Arduino15</code> |
   | Linux | <code>~/.arduino15</code> |

7. From this location, change directory to `packages/mcci/samd`.
8. Look at the directory contents with `ls`. You'll see a directory named like a version number, for example `1.2.0`.
9. Move the directory you just found away from the `samd` directory. It's convenient to have it around, so we suggest that you not delete it.
10. Create a symbolic link to the sandbox you created in step (3) above. On macOS and Linux, this is done using the `ln -s` command. On Windows 10, this is done with the `mklink /d` command. (On older versions of Windows, unless you're very experienced, you might want to use a Linux VM in VirtualBox -- we can't advise on the best way to do this.)

    - On any system you can also simply move the old directory out of the way, and then move your sandbox in. The old directory cannot simply be renamed `packages/mcci/samd`, but it can be put (for example) in `packages/mcci/hide-samd-1.2.0` (or whatever version it was). After moving the old directory away, you can move the repo that you cloned in step 5, above, into the `packages/mcci/samd` directory using a suitable version number.

11. Open the Arduino IDE.
12. Use the menu `File>New` to create an empty sketch (or reuse the sketch from step 3).
13. Make sure the board you want to develop for is selected in `Tools>Boards`.
14. Make changes and build.

Remember to restart the IDE whenever you change `platform.txt`, `boards.txt` or `programmers.txt`.

**Caution:** If you update your BSP from the IDE, your changes may be discarded.

## Bugs or Issues

If you find a bug you can submit an issue here on GitHub:

[`github.com/mcci-catena/ArduinoCore-samd/issues`](https://github.com/mcci-catena/ArduinoCore-samd/issues)

Before posting a new issue, please check if the same problem has been already reported by someone else
to avoid duplicates.

## Release History

- [v2.2.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v2.2.0) adds support for UI selection of network and subband.

- [v2.1.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v2.1.0) added support for the LoRaWAN Region KR-920 (issue [#34](https://github.com/mcci-catena/ArduinoCore-samd/issues/34)). Rename AU921 region to AU915 (retaining `CFG_au921` redundant symbol for older LMICs) ([#37](https://github.com/mcci-catena/ArduinoCore-samd/issues/37), [#38](https://github.com/mcci-catena/ArduinoCore-samd/issues/38)). Add support for new IDE features for injecting precompiled objects ([#39](https://github.com/mcci-catena/ArduinoCore-samd/issues/39)).

- [v2.0.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v2.0.0) makes a major version jump primarily to avoid reusing versions that are used by tags in the repo for Adafruit BSPs. It also adds `_mcci_arduino_version` (issue [#28](https://github.com/mcci-catena/ArduinoCore-samd/issues/28)).

- [v1.3.1](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v1.3.1) corrected the version in `platform.txt`, which was missed.

- [v1.3.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v1.3.0) increases the serial port ring-buffer size (issue [#26](https://github.com/mcci-catena/ArduinoCore-samd/issues/26))

- [v1.2.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v1.2.0) adds several features. It moves flags in the link script to accommodate the BME680 BSEC library. It adapts the JSON templating system from the [MCCI STM32 BSP](https://github.com/mcci-catena/Arduino_Core_STM32). The `millis()` routine was updated so that the clock value will advance even if interrupts are disabled (issue [#17](https://github.com/mcci-catena/ArduinoCore-samd/issues/16)). Properly define the country code for as923jp (issue [#13](https://github.com/mcci-catena/ArduinoCore-samd/issues/13)). Fix typo `au915` should have been `au921`(issue [#12](https://github.com/mcci-catena/ArduinoCore-samd/issues/12)).

- [v1.1.0](https://github.com/mcci-catena/ArduinoCore-samd/releases/tag/v1.1.0) adds support for the MCCI Catena 4420 and 4470. It also adds the ability to select the target LoRaWAN region from the IDE Tools menu (assuming that you're using the [MCCI Catena `arduino-lmic`](https://github.com/mcci-catena/arduino-lmic) library). It also fixes the USB product ID numbers to match the official numbers assigned by MCCI release engineering (issue [#9](https://github.com/mcci-catena/ArduinoCore-samd/issues/9)).

## License and credits

This core has been developed by Arduino LLC in collaboration with Atmel. The copyright notice doesn't
include Adafruit, but we think it probably should. Additional work was done by MCCI to incorporate better support LoRaWAN and the MCCI family of SAMD-based LoRa boards.

### LGPL License

```text
  Copyright (c) 2015 Arduino LLC.  All rights reserved.
  Portions copyright (c) Adafruit Industries.
  Portions copyright (c) 2017-2019 MCCI Corporation.

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

MCCI and MCCI Catena are registered trademarks of MCCI Corporation. LoRaWAN is a registered trademark of the LoRa Alliance. All other trademarks are the properties of their respective owners.

### Support Open-Source Software, Hardware, and Community IoT

Everyone at MCCI invests time and resources providing this open-source code and open-source hardware. MCCI is also the principal corporate sponsor of [The Things Network New York](https://thethings.nyc) and [Ithaca](https://ttni.tech). Please support our work by purchasing products from MCCI! Visit our on-line store at [store.mcci.com](https://store.mcci.com).
