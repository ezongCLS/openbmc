#!/bin/bash
#
# Copyright 2020-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#

### BEGIN INIT INFO
# Provides:          eth0_mac_fixup.sh
# Required-Start:
# Required-Stop:
# Default-Start:     S
# Default-Stop:
# Short-Description:  Fixup the MAC address for eth0 based on wedge EEPROM
### END INIT INFO

PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin

# get the MAC from FBDarwin BMC EEPROM
mac=$(weutil bmc | grep '^Extended MAC Base' | cut -d' ' -f4)

# get the MAC from u-boot environment
ethaddr=$(fw_printenv ethaddr | cut -d'=' -f2)

if [ -z "$mac" ]; then
    if [ -n "$ethaddr" ]; then
        echo "Using U-boot ethaddr for mac."
        mac="$ethaddr"
    fi
fi
if [ -n "$mac" ]; then
    ifconfig eth0 hw ether "$mac"
else
    # No MAC from either EEPROM or u-boot, use whatever was assigned by kernel
    mac=$(ifconfig eth0 | grep HWaddr | awk '{ print $5 }')
fi

if [ "$ethaddr" != "$mac" ]; then
    # Set the MAC from EEPROM or ifconfig back to u-boot environment
    # so that u-boot can use it
    fw_setenv "ethaddr" "$mac"
fi
