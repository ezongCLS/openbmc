#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openbmc/pal.h>
#include <openbmc/pal_common.h>
#include <libpldm/pldm.h>
#include <libpldm/platform.h>
#include <libpldm-oem/pldm.h>
#include "raa_gen3.h"
#include "xdpe12284c.h"
#include "xdpe152xx.h"

#define MB_VR_BUS_ID   (20)
#define SWB_VR_BUS_ID  (3)

enum {
  VR_MB_CPU0_VCCIN   = 0,
  VR_MB_CPU0_VCCFA   = 1,
  VR_MB_CPU0_VCCD    = 2,
  VR_MB_CPU1_VCCIN   = 3,
  VR_MB_CPU1_VCCFA   = 4,
  VR_MB_CPU1_VCCD    = 5,
  VR_SWB_PXE0_VCC    = 6,
  VR_SWB_PXE1_VCC    = 7,
  VR_CNT,
};

enum {
  ADDR_SWB_VR_PXE0 = 0xC0,
  ADDR_SWB_VR_PXE1 = 0xC4,
};

enum {
  ADDR_CPU0_VCCIN  = 0xC0,
  ADDR_CPU0_VCCFA  = 0xC2,
  ADDR_CPU0_VCCD   = 0xC6,
  ADDR_CPU1_VCCIN  = 0xE4,
  ADDR_CPU1_VCCFA  = 0xE8,
  ADDR_CPU1_VCCD   = 0xEC,
};

enum {
  ADDR_INF_CPU0_VCCIN  = 0xC0,
  ADDR_INF_CPU0_VCCD   = 0xC4,
  ADDR_INF_CPU0_VCCFA  = 0xC8,
  ADDR_INF_CPU1_VCCIN  = 0xE4,
  ADDR_INF_CPU1_VCCFA  = 0xE8,
  ADDR_INF_CPU1_VCCD   = 0xEC,
};

int swb_vr_map_id(uint8_t addr, uint8_t* id) {
  switch (addr) {
  case ADDR_SWB_VR_PXE0:
    *id = VR0_COMP;
    return 0;

  case ADDR_SWB_VR_PXE1:
    *id = VR1_COMP;
    return 0;
  }
  return -1;
}

static int
vr_pldm_wr(uint8_t bus, uint8_t addr,
           uint8_t *txbuf, uint8_t txlen,
           uint8_t *rxbuf, uint8_t rxlen) {
  uint8_t tbuf[255] = {0};
  uint8_t tlen=0;
  uint8_t vr_id;
  int ret;

  ret = swb_vr_map_id(addr, &vr_id);
  if (ret)
    return -1;

  tbuf[0] = vr_id;
  tbuf[1] = rxlen;
  memcpy(tbuf+2, txbuf, txlen);
  tlen = txlen + 2;

  size_t rlen = 0;
  ret = pldm_oem_ipmi_send_recv(bus, SWB_BIC_EID,
                               NETFN_OEM_1S_REQ, CMD_OEM_1S_BIC_BRIDGE,
                               tbuf, tlen,
                               rxbuf, &rlen);
  return ret;
}


struct vr_ops raa_gen2_3_ops = {
  .get_fw_ver = get_raa_ver,
  .parse_file = raa_parse_file,
  .validate_file = NULL,
  .fw_update = raa_fw_update,
  .fw_verify = NULL,
};

struct vr_info vr_list[] = {
  [VR_MB_CPU0_VCCIN] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU0_VCCIN,
    .dev_name = "VR_CPU0_VCCIN/VCCFA_FIVRA",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU0_VCCFA] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU0_VCCFA,
    .dev_name = "VR_CPU0_VCCFAEHV/FAON",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU0_VCCD] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU0_VCCD,
    .dev_name = "VR_CPU0_VCCD",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCIN] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU1_VCCIN,
    .dev_name = "VR_CPU1_VCCIN/VCCFA_FIVRA",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCFA] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU1_VCCFA,
    .dev_name = "VR_CPU1_VCCFAEHV/FAON",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCD] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_CPU1_VCCD,
    .dev_name = "VR_CPU1_VCCD",
    .ops = &raa_gen2_3_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_SWB_PXE0_VCC] = {
    .bus = SWB_VR_BUS_ID,
    .addr = ADDR_SWB_VR_PXE0,
    .dev_name = "VR_PEX0_VCC",
    .ops = &raa_gen2_3_ops,
    .private_data = "swb",
    .xfer = vr_pldm_wr,
  },
  [VR_SWB_PXE1_VCC] = {
    .bus = SWB_VR_BUS_ID,
    .addr = ADDR_SWB_VR_PXE1,
    .dev_name = "VR_PEX1_VCC",
    .ops = &raa_gen2_3_ops,
    .private_data = "swb",
    .xfer = vr_pldm_wr,
  },
};



//INFINEON
struct vr_ops xdpe152xx_ops = {
  .get_fw_ver = get_xdpe152xx_ver,
  .parse_file = xdpe152xx_parse_file,
  .validate_file = NULL,
  .fw_update = xdpe152xx_fw_update,
  .fw_verify = NULL,
};

struct vr_ops xdpe12284c_ops = {
  .get_fw_ver = get_xdpe_ver,
  .parse_file = xdpe_parse_file,
  .validate_file = NULL,
  .fw_update = xdpe_fw_update,
  .fw_verify = NULL,
};

struct vr_info mb_inf_vr_list[] = {
  [VR_MB_CPU0_VCCIN] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU0_VCCIN,
    .dev_name = "VR_CPU0_VCCIN/VCCFA_FIVRA",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU0_VCCFA] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU0_VCCFA,
    .dev_name = "VR_CPU0_VCCFAEHV/FAON",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU0_VCCD] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU0_VCCD,
    .dev_name = "VR_CPU0_VCCD",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCIN] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU1_VCCIN,
    .dev_name = "VR_CPU1_VCCIN/VCCFA_FIVRA",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCFA] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU1_VCCFA,
    .dev_name = "VR_CPU1_VCCFAEHV/FAON",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
  [VR_MB_CPU1_VCCD] = {
    .bus = MB_VR_BUS_ID,
    .addr = ADDR_INF_CPU1_VCCD,
    .dev_name = "VR_CPU1_VCCD",
    .ops = &xdpe152xx_ops,
    .private_data = "mb",
    .xfer = NULL,
  },
};

int plat_vr_init(void) {
  int ret, i, vr_cnt = sizeof(vr_list)/sizeof(vr_list[0]);
  uint8_t mb_sku_id = 0;
  uint8_t inf_devid[3] = { 0x02, 0x79, 0x02 };
  uint8_t tbuf[8], rbuf[8];

  pal_get_platform_id(&mb_sku_id);
  mb_sku_id = mb_sku_id & 0x03;

//MB
  if (mb_sku_id == 0x01) {
    for (i = 0; i < 6; i++) {
      vr_list[i].ops =  &xdpe152xx_ops;
      vr_list[i].addr = mb_inf_vr_list[i].addr;
    }
  }

//SWB
  if (get_bic_ready()) {
    tbuf[0] = 0xAD;
    vr_pldm_wr(SWB_VR_BUS_ID, ADDR_SWB_VR_PXE0, tbuf, 1, rbuf, 3);
    if(!memcmp(rbuf, inf_devid, 3)) {
      for (i = 0; i < 2; i++) {
        vr_list[i+6].ops = &xdpe12284c_ops;
      }
    }
  }
  ret = vr_device_register(vr_list, vr_cnt);
  if (ret < 0) {
    vr_device_unregister();
  }

  return ret;
}

void plat_vr_exit(void) {
  if (plat_configs) {
    free(plat_configs);
  }
  return;
}
