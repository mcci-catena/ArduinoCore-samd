#!/usr/bin/env bash

##############################################################################
#
# File: ci.sh
#
# Function:
#     Travis script for test-building this library.
#
# Copyright Notice:
#     See LICENSE file accompanying this project.
#
# Author:
#     frazar     October 2020
#
##############################################################################

# Treat unset variables and parameters as an error
set -o nounset

# Exit immediately if a command fails
set -o errexit

# If set, the return value of a pipeline is the value of the last (rightmost)
# command to exit with a non-zero status, or zero if all commands in the
# pipeline exit successfully
set -o pipefail

# Configure bash to output debug information
set -o verbose

##############################################################################
# Run tests

# Install the Arduino-LMIC library
arduino-cli lib install "MCCI LoRaWAN LMIC library"

# Compile a simple example sketch from the Arduino-LMIC library
arduino-cli compile --verbose --fqbn mcci:samd:mcci_catena_4450:lorawan_region=us915 tests/raw/raw.ino
