# Copyright 2018-present Facebook. All Rights Reserved.
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

SUMMARY = "Watchdog Control Command Line Interface"
DESCRIPTION = "Watchdog Control Command Line Interface"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://wdtcli.c;beginline=7;endline=19;md5=da35978751a9d71b73679307c4d296ec"

LOCAL_URI = " \
    file://Makefile \
    file://wdtcli.c \
    "

pkgdir = "wdtcli"
binfiles = "wdtcli \
           "

CFLAGS += "-Wall -Werror "
LDFLAGS += " -lwatchdog"
RDEPENDS:${PN} += " libwatchdog"
DEPENDS:append = " libwatchdog"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin
  for f in ${binfiles}; do
    install -m 755 $f ${dst}/$f
    ln -snf ../fbpackages/${pkgdir}/$f ${bin}/$f
  done
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"
FILES:${PN} = "${FBPACKAGEDIR}/wdtcli ${prefix}/local/bin"
