#! /bin/sh
#
# Copyright 2015-present Facebook. All Rights Reserved.
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
# Provides:          server-init
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description: Provides a service to reinitialize the server
#
### END INIT INFO

. /usr/local/fbpackages/utils/ast-functions

# stop the service first
sv stop gpiod
sv stop sensord

slot_num=$1
PID=$$
PID_FILE="/var/run/server-init.${slot_num}.pid"
bus=$((slot_num-1))

# check if the script is running
[ -r "$PID_FILE" ] && OLDPID=$(cat "$PID_FILE") || OLDPID=''

# Set the current pid
echo "$PID" > "$PID_FILE"

# kill the previous running script if it is existed
if [ -n "$OLDPID" ] && (grep "server-init" /proc/$OLDPID/cmdline 1> /dev/null 2>&1) ; then
  echo "kill pid $OLDPID..."
  kill -s 9 "$OLDPID"
  ps | grep 'bic-cached' | grep "slot${slot_num}" | awk '{print $1}'| xargs kill -s 9 1> /dev/null 2>&1
  ps | grep 'power-util' | grep "slot${slot_num}" | awk '{print $1}'| xargs kill -s 9 1> /dev/null 2>&1
  ps | grep 'setup-fan' | awk '{print $1}'| xargs kill -s 9 1> /dev/null 2>&1
fi
unset OLDPID

if [ "$(is_server_prsnt "$slot_num")" = "0" ]; then
  disable_server_12V_power "$slot_num"
  gpio_set "FM_BMC_SLOT${slot_num}_ISOLATED_EN_R" 0
  rm -f "/tmp/*fruid_slot${slot_num}*"
  rm -f "/tmp/*sdr_slot${slot_num}*"
  kv del "slot${slot_num}_vr_c0h_crc"
  kv del "slot${slot_num}_vr_c4h_crc"
  kv del "slot${slot_num}_vr_ech_crc"
  kv del "slot${slot_num}_vr_c0h_new_crc" persistent
  kv del "slot${slot_num}_vr_c4h_new_crc" persistent
  kv del "slot${slot_num}_vr_ech_new_crc" persistent
  kv del "slot${slot_num}_cpld_new_ver"
  kv del "slot${slot_num}_is_m2_exp_prsnt"
  kv del "fru${slot_num}_2ou_board_type"
  kv del "fru${slot_num}_sb_type"
  kv del "fru${slot_num}_sb_board_rev_id"
  set_nic_power
else
  /usr/bin/sv start ipmbd_${bus} > /dev/null 2>&1
  /usr/local/bin/power-util "slot${slot_num}" 12V-on
  /usr/local/bin/bic-cached -s "slot${slot_num}"
  /usr/local/bin/bic-cached -f "slot${slot_num}"
  /usr/bin/fw-util "slot${slot_num}" --version > /dev/null
  /etc/init.d/setup-sic.sh "slot${slot_num}" > /dev/null
fi

# reload fscd
/etc/init.d/setup-fan.sh reload

# start the services again
sv start gpiod
sv start sensord
