/*
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
 *
 * This file provides platform specific implementation of FRUID information
 *
 * FRUID specification can be found at
 * www.intel.com/content/dam/www/public/us/en/documents/product-briefs/platform-management-fru-document-rev-1-2-feb-2013.pdf
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <openbmc/pal.h>
#include <openbmc/hal_fruid.h>
#include "fruid.h"

#define BIN_MB        "/tmp/fruid_mb.bin"
#define FRUID_SIZE    512
/*
 * copy_eeprom_to_bin - copy the eeprom to binary file im /tmp directory
 *
 * @eeprom_file   : path for the eeprom of the device
 * @bin_file      : path for the binary file
 *
 * returns 0 on successful copy
 * returns non-zero on file operation errors
 */
int copy_eeprom_to_bin(const char * eeprom_file, const char * bin_file) {
  int eeprom;
  int bin;
  uint64_t tmp[FRUID_SIZE];
  ssize_t bytes_rd, bytes_wr;

  errno = 0;

  if (access(eeprom_file, F_OK) != -1) {
    eeprom = open(eeprom_file, O_RDONLY);
    if (eeprom == -1) {
      syslog(LOG_ERR, "%s: unable to open the %s file: %s",
          __func__, eeprom_file, strerror(errno));
      return errno;
    }

    bin = open(bin_file, O_WRONLY | O_CREAT, 0644);
    if (bin == -1) {
      syslog(LOG_ERR, "%s: unable to create %s file: %s",
         __func__, bin_file, strerror(errno));
      return errno;
    }

    bytes_rd = read(eeprom, tmp, FRUID_SIZE);
    if (bytes_rd > FRUID_SIZE) {
      syslog(LOG_ERR, "%s: read %s file failed: %s",
          __func__, eeprom_file, strerror(errno));
      syslog(LOG_ERR, "%s: Need to less than or equal to %d bytes", __func__, FRUID_SIZE);
      return errno;
    }

    bytes_wr = write(bin, tmp, bytes_rd);
    if (bytes_wr != bytes_rd) {
      syslog(LOG_ERR, "%s: write to %s file failed: %s",
          __func__, bin_file, strerror(errno));
      return errno;
    }

    close(bin);
    close(eeprom);
  }

  return 0;
}

static void *swb_fru_reader(void *arg) {
  int retry = 3;
  char bin[32];

  sleep(2);  // wait ipmbd ready

  if (pal_get_fruid_path(FRU_SWB, bin)) {
    syslog(LOG_WARNING, "%s: Get Fruid SWB Bin Path Failed", __func__);
  }

  printf("binpath1 = %s\n", bin);
  while (retry > 0) {
    if (hal_read_pldm_fruid(0, bin, FRUID_SIZE) == 0) {
      break;
    }

    retry--;
    sleep(1);
  }
  if (retry <= 0) {
    syslog(LOG_WARNING, "%s: Read PDB FRU Failed", __func__);
  }

  pthread_detach(pthread_self());
  pthread_exit(NULL);
}

static void *fio_fru_reader(void *arg) {
  int retry = 3;
  char bin[32];

  if (pal_get_fruid_path(FRU_FIO, bin)) {
    syslog(LOG_WARNING, "%s: Get Fruid FIO Bin Path Failed", __func__);
  }

  printf("binpath = %s\n", bin);
  sleep(2);  // wait ipmbd ready
  while (retry > 0) {
    if (hal_read_pldm_fruid(1, bin, FRUID_SIZE) == 0) {
      break;
    }

    retry--;
    sleep(1);
  }
  if (retry <= 0) {
    syslog(LOG_WARNING, "%s: Read PDB FRU Failed", __func__);
  }

  pthread_detach(pthread_self());
  pthread_exit(NULL);
}

/* Populate the platform specific eeprom for fruid info */
int plat_fruid_init(void) {
  pthread_t tid_swb;
  pthread_t tid_fio;
  char eeprom_path[64] = {0};
  char bin_path[32] = {0};

  for (int i=1; i<FRU_CNT; i++) { 
    if (pal_get_fruid_path(i, bin_path)) {
      syslog(LOG_WARNING, "%s: Get Fruid%d Bin Path Failed", __func__, i);
      continue;
    }

    if (pal_get_fruid_eeprom_path(i, eeprom_path)) {
      syslog(LOG_WARNING, "%s: Get Fruid%d EEPROM Path Failed", __func__, i);
      continue;
    }

    printf("%s %s\n", eeprom_path, bin_path);

    if (copy_eeprom_to_bin(eeprom_path, bin_path)) {
      syslog(LOG_WARNING, "%s: Copy FRU%d EEPROM Failed", __func__, i);
      continue;
    }
  }

  if (pthread_create(&tid_swb, NULL, swb_fru_reader, NULL) < 0) {
    syslog(LOG_WARNING, "%s: Create swb_fru_reader Failed", __func__);
  }

  if (pthread_create(&tid_fio, NULL, fio_fru_reader, NULL) < 0) {
    syslog(LOG_WARNING, "%s: Create fio_fru_reader Failed", __func__);
  }

  return 0;
}

int plat_fruid_size(unsigned char payload_id) {
  struct stat buf;
  int ret;
  char bin[32] = {0};

  if (pal_get_fruid_path(FRU_MB, bin)) {
    syslog(LOG_WARNING, "%s: Get Fruid MB Bin Path Failed", __func__);
    return -1;
  }

  // check the size of the file and return size
  ret = stat(bin, &buf);
  if (ret) {
    return 0;
  }

  return buf.st_size;
}

int plat_fruid_data(unsigned char payload_id, int fru_id, int offset, int count, unsigned char *data) {
  int fd;
  int ret;
  char bin[32] = {0};

  if (pal_get_fruid_path(FRU_MB, bin)) {
    syslog(LOG_WARNING, "%s: Get Fruid MB Bin Path Failed", __func__);
    return -1;
  }

  // open file for read purpose
  fd = open(bin, O_RDONLY);
  if (fd < 0) {
    return fd;
  }

  // seek position based on given offset
  ret = lseek(fd, offset, SEEK_SET);
  if (ret < 0) {
    close(fd);
    return ret;
  }

  // read the file content
  ret = read(fd, data, count);
  if (ret != count) {
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}
