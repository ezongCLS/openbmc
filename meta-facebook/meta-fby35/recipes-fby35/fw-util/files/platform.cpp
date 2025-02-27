#include <syslog.h>
#include <facebook/fby35_common.h>
#include "bic_fw.h"
#include "bic_cpld.h"
#include "bmc_cpld.h"
#include "nic_ext.h"
#include "bic_me.h"
#include "bic_bios.h"
#include "bic_vr.h"
#include "bic_pcie_sw.h"
#include "bic_m2_dev.h"
#include <facebook/bic.h>
#include <facebook/bic_fwupdate.h>
#include <facebook/bic_ipmi.h>
#include <openbmc/pal.h>
#include "usbdbg.h"
#include "mp5990.h"

NicExtComponent nic_fw("nic", "nic", "nic_fw_ver", FRU_NIC, 0x00);

//slot1 sb bic/cpld/bios/me/vr
BicFwComponent  bic_fw1("slot1", "bic", "sb", FW_SB_BIC);
BicFwComponent  bic_rcvy_fw1("slot1", "bic_rcvy", "sb", FW_BIC_RCVY);
BiosComponent   bios_fw1("slot1", "bios", FRU_SLOT1, FW_BIOS);
VrComponent     vr_fw1("slot1", "vr", FW_VR);

class ClassConfig {
  public:
    ClassConfig() {
      uint8_t bmc_location = 0;
      uint8_t hsc_type = HSC_UNKNOWN;
      uint8_t board_rev = UNKNOWN_REV;
      int slot1_config_status, slot3_config_status = 0;

      // Only slot1 and slot3 may be 2OU, so we need to get the config status of slot1 and slot3
      slot1_config_status = bic_is_exp_prsnt(FRU_SLOT1);
      if (slot1_config_status < 0) {
        slot1_config_status = 0;
      }

      slot3_config_status = bic_is_exp_prsnt(FRU_SLOT3);
      if (slot3_config_status < 0) {
        slot3_config_status = 0;
      }

      if (fby35_common_get_bmc_location(&bmc_location) < 0) {
        printf("Failed to initialize the fw-util\n");
        exit(EXIT_FAILURE);
      }

      if (bmc_location == NIC_BMC) {
        static BmcCpldComponent  cpld_bmc("bmc", "cpld", MAX10_10M04, 9, 0x40);

        //slot1 bb bic/cpld
        static BicFwComponent    bic_bb_fw1("slot1", "bb_bic", "bb", FW_BB_BIC);
        static CpldComponent     cpld_bb_fw1("slot1", "bb_cpld", "bb", FW_BB_CPLD, 0, 0);

        //slot1 2ou bic/cpld
        static BicFwComponent  bic_2ou_fw1("slot1", "2ou_bic", "2ou", FW_2OU_BIC);
        static CpldComponent   cpld_2ou_fw1("slot1", "2ou_cpld", "2ou", FW_2OU_CPLD, 0, 0);
      } else {
        // Register USB Debug Card components
        static UsbDbgComponent   usbdbg("ocpdbg", "mcu", "FBY35", 9, 0x60, false);
        static UsbDbgBlComponent usbdbgbl("ocpdbg", "mcubl", 9, 0x60, 0x02);  // target ID of bootloader = 0x02

        static BmcCpldComponent  cpld_bmc("bmc", "cpld", MAX10_10M04, 12, 0x40);

        if (fby35_common_get_bb_hsc_type(&hsc_type) == 0) {
          if (hsc_type == HSC_MP5990) {
            static MP5990Component  hsc_bb("bmc", "hsc", FRU_BMC, 11, 0x40);
          }
        }
        if (get_board_rev(FRU_SLOT1, BOARD_ID_SB, &board_rev) == 0) {
          if (IS_BOARD_REV_MPS(board_rev)) {
            static MP5990Component hsc_fw1("slot1", "hsc", FRU_SLOT1, 1, 0x16);
          }
        }
        if (get_board_rev(FRU_SLOT2, BOARD_ID_SB, &board_rev) == 0) {
          if (IS_BOARD_REV_MPS(board_rev)) {
            static MP5990Component hsc_fw2("slot2", "hsc", FRU_SLOT2, 1, 0x16);
          }
        }
        if (get_board_rev(FRU_SLOT3, BOARD_ID_SB, &board_rev) == 0) {
          if (IS_BOARD_REV_MPS(board_rev)) {
            static MP5990Component hsc_fw3("slot3", "hsc", FRU_SLOT3, 1, 0x16);
          }
        }
        if (get_board_rev(FRU_SLOT4, BOARD_ID_SB, &board_rev) == 0) {
          if (IS_BOARD_REV_MPS(board_rev)) {
            static MP5990Component hsc_fw4("slot4", "hsc", FRU_SLOT4, 1, 0x16);
          }
        }

        //slot1 1ou bic/cpld
        static BicFwComponent    bic_1ou_fw1("slot1", "1ou_bic", "1ou", FW_1OU_BIC);
        static CpldComponent     cpld_1ou_fw1("slot1", "1ou_cpld", "1ou", FW_1OU_CPLD, 0, 0);

        //slot1 vr/cpld/me
        if (fby35_common_get_slot_type(FRU_SLOT1) == SERVER_TYPE_HD) {
            static CpldComponent   cpld_fw1("slot1", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x44);
            static VrComponent     vr_vddcpu0_fw1("slot1", "vr_vddcrcpu0", FW_VR_VDDCRCPU0);
            static VrComponent     vr_vddcpu1_fw1("slot1", "vr_vddcrcpu1", FW_VR_VDDCRCPU1);
            static VrComponent     vr_vdd11s3_fw1("slot1", "vr_vdd11s3", FW_VR_VDD11S3);
        } else {
            static CpldComponent   cpld_fw1("slot1", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x40);
            static MeComponent     me_fw1("slot1", "me", FRU_SLOT1);
            static VrComponent     vr_vccin_fw1("slot1", "vr_vccin", FW_VR_VCCIN);
            static VrComponent     vr_vccd_fw1("slot1", "vr_vccd", FW_VR_VCCD);
            static VrComponent     vr_vccinfaon_fw1("slot1", "vr_vccinfaon", FW_VR_VCCINFAON);
        }

        //slot1 1ou vr
        if( (slot1_config_status & PRESENT_1OU) == PRESENT_1OU ) {
            if( isRainbowFalls(FRU_SLOT1) ){
                static VrComponent     vr_1ou_va0v8_fw1("slot1", "1ou_vr_v9asica", FW_1OU_VR_V9_ASICA);
                static VrComponent     vr_1ou_vddqab_fw1("slot1", "1ou_vr_vddqab", FW_1OU_VR_VDDQAB);
                static VrComponent     vr_1ou_vddqcd_fw1("slot1", "1ou_vr_vddqcd", FW_1OU_VR_VDDQCD);
            }
        }

        //slot2 sb bic/cpld/bios/me/vr
        static BicFwComponent    bic_fw2("slot2", "bic", "sb", FW_SB_BIC);
        static BicFwComponent    bic_rcvy_fw2("slot2", "bic_rcvy", "sb", FW_BIC_RCVY);
        static CpldComponent     cpld_fw2("slot2", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x40);
        static BiosComponent     bios_fw2("slot2", "bios", FRU_SLOT2, FW_BIOS);
        static MeComponent       me_fw2("slot2", "me", FRU_SLOT2);
        static VrComponent       vr_fw2("slot2", "vr", FW_VR);
        static VrComponent       vr_vccin_fw2("slot2", "vr_vccin", FW_VR_VCCIN);
        static VrComponent       vr_vccd_fw2("slot2", "vr_vccd", FW_VR_VCCD);
        static VrComponent       vr_vccinfaon_fw2("slot2", "vr_vccinfaon", FW_VR_VCCINFAON);

        //slot2 1ou bic/cpld
        static BicFwComponent    bic_1ou_fw2("slot2", "1ou_bic", "1ou", FW_1OU_BIC);
        static CpldComponent     cpld_1ou_fw2("slot2", "1ou_cpld", "1ou", FW_1OU_CPLD, 0, 0);

        //slot3 sb bic/bios/vr
        static BicFwComponent    bic_fw3("slot3", "bic", "sb", FW_SB_BIC);
        static BicFwComponent    bic_rcvy_fw3("slot3", "bic_rcvy", "sb", FW_BIC_RCVY);
        static BiosComponent     bios_fw3("slot3", "bios", FRU_SLOT3, FW_BIOS);
        static VrComponent       vr_fw3("slot3", "vr", FW_VR);

        //slot3 vr/cpld/me
        if (fby35_common_get_slot_type(FRU_SLOT3) == SERVER_TYPE_HD) {
            static CpldComponent   cpld_fw3("slot3", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x44);
            static VrComponent     vr_vddcpu0_fw3("slot3", "vr_vddcrcpu0", FW_VR_VDDCRCPU0);
            static VrComponent     vr_vddcpu1_fw3("slot3", "vr_vddcrcpu1", FW_VR_VDDCRCPU1);
            static VrComponent     vr_vdd11s3_fw3("slot3", "vr_vdd11s3", FW_VR_VDD11S3);
        } else {
            static CpldComponent   cpld_fw3("slot3", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x40);
            static MeComponent       me_fw3("slot3", "me", FRU_SLOT3);
            static VrComponent       vr_vccin_fw3("slot3", "vr_vccin", FW_VR_VCCIN);
            static VrComponent       vr_vccd_fw3("slot3", "vr_vccd", FW_VR_VCCD);
            static VrComponent       vr_vccinfaon_fw3("slot3", "vr_vccinfaon", FW_VR_VCCINFAON);
        }

        //slot3 1ou vr
        if( (slot3_config_status & PRESENT_1OU) == PRESENT_1OU ) {
            if(isRainbowFalls(FRU_SLOT3)){
                static VrComponent     vr_1ou_va0v8_fw3("slot3", "1ou_vr_v9asica", FW_1OU_VR_V9_ASICA);
                static VrComponent     vr_1ou_vddqab_fw3("slot3", "1ou_vr_vddqab", FW_1OU_VR_VDDQAB);
                static VrComponent     vr_1ou_vddqcd_fw3("slot3", "1ou_vr_vddqcd", FW_1OU_VR_VDDQCD);
            }
        }

        //slot3 1ou bic/cpld
        static BicFwComponent    bic_1ou_fw3("slot3", "1ou_bic", "1ou", FW_1OU_BIC);
        static CpldComponent     cpld_1ou_fw3("slot3", "1ou_cpld", "1ou", FW_1OU_CPLD, 0, 0);

        //slot4 sb bic/cpld/bios/me/vr
        static BicFwComponent    bic_fw4("slot4", "bic", "sb", FW_SB_BIC);
        static BicFwComponent    bic_rcvy_fw4("slot4", "bic_rcvy", "sb", FW_BIC_RCVY);
        static CpldComponent     cpld_fw4("slot4", "cpld", "sb", FW_CPLD, LCMXO3_4300C, 0x40);
        static BiosComponent     bios_fw4("slot4", "bios", FRU_SLOT4, FW_BIOS);
        static MeComponent       me_fw4("slot4", "me", FRU_SLOT4);
        static VrComponent       vr_fw4("slot4", "vr", FW_VR);
        static VrComponent       vr_vccin_fw4("slot4", "vr_vccin", FW_VR_VCCIN);
        static VrComponent       vr_vccd_fw4("slot4", "vr_vccd", FW_VR_VCCD);
        static VrComponent       vr_vccinfaon_fw4("slot4", "vr_vccinfaon", FW_VR_VCCINFAON);

        //slot4 1ou bic/cpld
        static BicFwComponent    bic_1ou_fw4("slot4", "1ou_bic", "1ou", FW_1OU_BIC);
        static CpldComponent     cpld_1ou_fw4("slot4", "1ou_cpld", "1ou", FW_1OU_CPLD, 0, 0);
      }
  }

  static bool isRainbowFalls(uint8_t slot_id) {
    int ret;
    uint8_t card_type = TYPE_1OU_UNKNOWN;

    ret = bic_get_1ou_type(slot_id, &card_type);
    if (!ret && card_type == TYPE_1OU_RAINBOW_FALLS) {
      return true;
    }
    return false;
  }
};

ClassConfig platform_config;
