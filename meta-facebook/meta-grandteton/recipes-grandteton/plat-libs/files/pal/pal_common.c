#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <openbmc/obmc-pal.h>
#include <openbmc/libgpio.h>
#include <syslog.h>
#include "pal_common.h"
#include "pal_def.h"
#include "pal_gpio.h"

// TODO Remove after updating inspiration point pal_gpio.h
#ifndef BIC_READY
#define BIC_READY "FM_SWB_BIC_READY_ISO_R_N"
#endif

bool
is_cpu_socket_occupy(uint8_t cpu_idx) {
  static bool cached = false;
  static unsigned int cached_id = 0;

  if (!cached) {
    const char *shadows[] = {
      "FM_CPU0_SKTOCC_LVT3_PLD_N",
      "FM_CPU1_SKTOCC_LVT3_PLD_N"
    };
    if (gpio_get_value_by_shadow_list(shadows, ARRAY_SIZE(shadows), &cached_id)) {
      return false;
    }
    cached = true;
  }

  // bit == 1 implies CPU is absent.
  if (cached_id & (1 << cpu_idx)) {
    return false;
  }
  return true;
}

bool
pal_skip_access_me(void) {
  if (!access("/tmp/fin_bios_upd", F_OK)) {
    return true;
  }

  return false;
}

int
read_device(const char *device, int *value) {
  FILE *fp = NULL;

  if (device == NULL || value == NULL) {
    syslog(LOG_ERR, "%s: Invalid parameter", __func__);
    return -1;
  }

  fp = fopen(device, "r");
  if (fp == NULL) {
    syslog(LOG_INFO, "%s failed to open device %s error: %s", __func__, device, strerror(errno));
    return -1;
  }

  if (fscanf(fp, "%d", value) != 1) {
    syslog(LOG_INFO, "%s failed to read device %s error: %s", __func__, device, strerror(errno));
    fclose(fp);
    return -1;
  }
  fclose(fp);

  return 0;
}

bool get_bic_ready(void) {
  gpio_value_t value;

  value = gpio_get_value_by_shadow(BIC_READY);
  if(value != GPIO_VALUE_INVALID)
    return value ? false : true;

  return false;
}
