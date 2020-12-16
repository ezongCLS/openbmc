#ifndef __PAL_SENSORS_H__
#define __PAL_SENSORS_H__

#include <openbmc/pmbus.h>
#include <openbmc/obmc_pal_sensors.h>

#define MAX_SENSOR_NUM         (0xFF)
#define MAX_DEVICE_NAME_SIZE   (128)
#define MB_TEMP_DEVICE  "/sys/class/i2c-dev/i2c-%d/device/%d-00%x/hwmon/hwmon*/temp1_input"
#define MB_ADC_VOLTAGE_DEVICE "/sys/devices/platform/iio-hwmon/hwmon/hwmon*/in%d_input"

//PECI CMD INFO
#define PECI_RETRY_TIMES                        (10)
#define PECI_CMD_RD_PKG_CONFIG                  (0xA1)
#define PECI_INDEX_PKG_TEMP                     (2)
#define PECI_INDEX_ACCUMULATED_ENERGY_STATUS    (3)
#define PECI_INDEX_THERMAL_MARGIN               (10)
#define PECI_INDEX_DIMM_THERMAL_RW              (14)
#define PECI_INDEX_TEMP_TARGET                  (16)
#define PECI_INDEX_TOTAL_TIME                   (31)
#define PECI_INDEX_PKG_IDENTIFIER               (0)
#define PECI_THERMAL_DIMM0_BYTE                 (1)
#define PECI_THERMAL_DIMM1_BYTE                 (2)
#define INTERNAL_CPU_ERROR_MASK                 (0x1C)
#define EXTERNAL_CPU_ERROR_MASK                 (0xE0)
#define BOTH_CPU_ERROR_MASK                     (0xFC)
#define PECI_SUCCESS                            (0x40)

//NM DEVICE INFO
#define NM_PSU_MUX_ADDR  (0xE0)

//CPU INFO
#define PECI_CPU0_ADDR    (0x30)
#define PECI_CPU1_ADDR    (0x31)
#define PECI_CPU2_ADDR    (0x32)
#define PECI_CPU3_ADDR    (0x33)
#define PECI_CPU4_ADDR    (0x34)
#define PECI_CPU5_ADDR    (0x35)
#define PECI_CPU6_ADDR    (0x36)
#define PECI_CPU7_ADDR    (0x37)

//TI VR NVM INFO
#define TI_ONLY_CONFIG2   (0xCE)

enum {
  CFG_SAMPLE_DIABLE = 0,
  CFG_SAMPLE_2,
  CFG_SAMPLE_4,
  CFG_SAMPLE_8,
  CFG_SAMPLE_16,
  CFG_SAMPLE_32,
  CFG_SAMPLE_64,
  CFG_SAMPLE_128,
};

//AMD1278 CMD INFO
#define ADM1278_SLAVE_ADDR  (0x22)
#define ADM1278_RSENSE      (0.15)

#define ADM1278_PMON_CONFIG         (0xD4)
#define ADM1278_PEAK_PIN            (0xDA)
#define PMON_CFG_VIN_EN             (1 << 2)
#define PMON_CFG_TEPM1_EN           (1 << 3)
#define PMON_CFG_CONTINUOUS_SAMPLE  (1 << 4)
#define PMON_CFG_VI_AVG(x)          (x << 8)
#define PMON_CFG_AVG_PWR(x)         (x << 11)

#define ADM1278_ALTER_CONFIG        (0xD5)
#define IOUT_OC_WARN_EN1            (1 << 10) 
 
//INA260 CMD INFO
#define INA260_CURRENT   (0x01)
#define INA260_VOLTAGE   (0x02)
#define INA260_POWER     (0x03)

#define FAN_PWM_ALL_NUM        (4)
#define FAN_TACH_ALL_NUM       (8)

//Sensor Table
enum {
  MB_SNR_PCH_TEMP = 0x08,
//NIC
  NIC_MEZZ0_SNR_TEMP = 0x10,
  NIC_MEZZ1_SNR_TEMP = 0x11,
//HARD DISK TEMP
  MB_SNR_BOOT_DRIVE_TEMP = 0x12,
  MB_SNR_DATA0_DRIVE_TEMP = 0x13,
  MB_SNR_DATA1_DRIVE_TEMP = 0x14,
  MB_SNR_POWER_FAIL = 0x1A,
//PDB HSC
  PDB_SNR_HSC_VIN = 0x20,
  PDB_SNR_HSC_VOUT = 0x21,
  PDB_SNR_HSC_TEMP = 0x22,
  PDB_SNR_HSC_PIN = 0x23,
  PDB_SNR_HSC_PEAK_IOUT = 0x24,
  PDB_SNR_HSC_PEAK_PIN = 0x25,
  PDB_SNR_HSC_P12V = 0x26,
  PDB_SNR_HSC_P3V = 0x27,

  MB_SNR_CPU0_TJMAX = 0x28,
  MB_SNR_CPU1_TJMAX = 0x29,
  MB_SNR_CPU2_TJMAX = 0x2A,
  MB_SNR_CPU3_TJMAX = 0x2B,
  MB_SNR_CPU4_TJMAX = 0x2C,
  MB_SNR_CPU5_TJMAX = 0x2D,
  MB_SNR_CPU6_TJMAX = 0x2E,
  MB_SNR_CPU7_TJMAX = 0x2F,

  MB_SNR_CPU0_PKG_POWER = 0x30,
  MB_SNR_CPU1_PKG_POWER = 0x31,
  MB_SNR_CPU2_PKG_POWER = 0x32,
  MB_SNR_CPU3_PKG_POWER = 0x33,
  MB_SNR_CPU4_PKG_POWER = 0x34,
  MB_SNR_CPU5_PKG_POWER = 0x35,
  MB_SNR_CPU6_PKG_POWER = 0x36,
  MB_SNR_CPU7_PKG_POWER = 0x37,

  MB_SNR_CPU0_THERM_MARGIN = 0x38,
  MB_SNR_CPU1_THERM_MARGIN = 0x39,
  MB_SNR_CPU2_THERM_MARGIN = 0x3A,
  MB_SNR_CPU3_THERM_MARGIN = 0x3B,
  MB_SNR_CPU4_THERM_MARGIN = 0x3C,
  MB_SNR_CPU5_THERM_MARGIN = 0x3D,
  MB_SNR_CPU6_THERM_MARGIN = 0x3E,
  MB_SNR_CPU7_THERM_MARGIN = 0x3F,

//HSC
  MB_SNR_HSC_VIN = 0x40,
  MB_SNR_HSC_IOUT = 0x41,
  MB_SNR_HSC_PIN = 0x42,
  MB_SNR_HSC_TEMP = 0x43,
  MB1_SNR_HSC_VIN = 0x44,
  MB1_SNR_HSC_IOUT = 0x45,
  MB1_SNR_HSC_PIN = 0x46,
  MB1_SNR_HSC_TEMP = 0x47,
  MB2_SNR_HSC_VIN = 0x48,
  MB2_SNR_HSC_IOUT = 0x49,
  MB2_SNR_HSC_PIN = 0x4A,
  MB2_SNR_HSC_TEMP = 0x4B,
  MB3_SNR_HSC_VIN = 0x4C,
  MB3_SNR_HSC_IOUT = 0x4D,
  MB3_SNR_HSC_PIN = 0x4E,
  MB3_SNR_HSC_TEMP = 0x4F,
//HCS PEAK PIN
  MB_SNR_HSC_PEAK_PIN = 0x50,
  MB1_SNR_HSC_PEAK_PIN = 0x51,
  MB2_SNR_HSC_PEAK_PIN = 0x52,
  MB3_SNR_HSC_PEAK_PIN = 0x53,  

//FAN POWER
  PDB_SNR_FAN0_POWER = 0x54,
  PDB_SNR_FAN1_POWER = 0x55,
  PDB_SNR_FAN2_POWER = 0x56,
  PDB_SNR_FAN3_POWER = 0x57,

//FAN DUTY
  PDB_SNR_FAN0_DUTY = 0x58,
  PDB_SNR_FAN1_DUTY = 0x59,
  PDB_SNR_FAN2_DUTY = 0x5A,
  PDB_SNR_FAN3_DUTY = 0x5B,

//INA260
  MB_SNR_P12V_STBY_INA260_VOL = 0x5C,
  MB_SNR_P3V3_M2_1_INA260_VOL = 0x5D,
  MB_SNR_P3V3_M2_2_INA260_VOL = 0x5E,
  MB_SNR_P3V3_M2_3_INA260_VOL = 0x5F,
//FAN
  PDB_SNR_FAN0_INLET_SPEED = 0x60,
  PDB_SNR_FAN1_INLET_SPEED = 0x61,
  PDB_SNR_FAN2_INLET_SPEED = 0x62,
  PDB_SNR_FAN3_INLET_SPEED = 0x63,
  PDB_SNR_FAN0_OUTLET_SPEED = 0x64,
  PDB_SNR_FAN1_OUTLET_SPEED = 0x65,
  PDB_SNR_FAN2_OUTLET_SPEED = 0x66,
  PDB_SNR_FAN3_OUTLET_SPEED = 0x67,
  PDB_SNR_FAN0_VOLTAGE = 0x68,
  PDB_SNR_FAN1_VOLTAGE = 0x69,
  PDB_SNR_FAN2_VOLTAGE = 0x6A,
  PDB_SNR_FAN3_VOLTAGE = 0x6B,
  PDB_SNR_FAN0_CURRENT = 0x6C,
  PDB_SNR_FAN1_CURRENT = 0x6D,
  PDB_SNR_FAN2_CURRENT = 0x6E,
  PDB_SNR_FAN3_CURRENT = 0x6F,

//CPU DIMM TEMP
  MB_SNR_CPU0_DIMM_GRPA_TEMP = 0x70,
  MB_SNR_CPU0_DIMM_GRPB_TEMP = 0x71,
  MB_SNR_CPU0_DIMM_GRPC_TEMP = 0x72,
  MB_SNR_CPU0_DIMM_GRPD_TEMP = 0x73,
  MB_SNR_CPU0_DIMM_GRPE_TEMP = 0x74,
  MB_SNR_CPU0_DIMM_GRPF_TEMP = 0x75,

  MB_SNR_CPU1_DIMM_GRPA_TEMP = 0x76,
  MB_SNR_CPU1_DIMM_GRPB_TEMP = 0x77,
  MB_SNR_CPU1_DIMM_GRPC_TEMP = 0x78,
  MB_SNR_CPU1_DIMM_GRPD_TEMP = 0x79,
  MB_SNR_CPU1_DIMM_GRPE_TEMP = 0x7A,
  MB_SNR_CPU1_DIMM_GRPF_TEMP = 0x7B,

  MB_SNR_CPU2_DIMM_GRPA_TEMP = 0x7C,
  MB_SNR_CPU2_DIMM_GRPB_TEMP = 0x7D,
  MB_SNR_CPU2_DIMM_GRPC_TEMP = 0x7E,
  MB_SNR_CPU2_DIMM_GRPD_TEMP = 0x7F,
  MB_SNR_CPU2_DIMM_GRPE_TEMP = 0x80,
  MB_SNR_CPU2_DIMM_GRPF_TEMP = 0x81,

  MB_SNR_CPU3_DIMM_GRPA_TEMP = 0x82,
  MB_SNR_CPU3_DIMM_GRPB_TEMP = 0x83,
  MB_SNR_CPU3_DIMM_GRPC_TEMP = 0x84,
  MB_SNR_CPU3_DIMM_GRPD_TEMP = 0x85,
  MB_SNR_CPU3_DIMM_GRPE_TEMP = 0x86,
  MB_SNR_CPU3_DIMM_GRPF_TEMP = 0x87,

  MB_SNR_CPU4_DIMM_GRPA_TEMP = 0x88,
  MB_SNR_CPU4_DIMM_GRPB_TEMP = 0x89,
  MB_SNR_CPU4_DIMM_GRPC_TEMP = 0x8A,
  MB_SNR_CPU4_DIMM_GRPD_TEMP = 0x8B,
  MB_SNR_CPU4_DIMM_GRPE_TEMP = 0x8C,
  MB_SNR_CPU4_DIMM_GRPF_TEMP = 0x8D,

  MB_SNR_CPU5_DIMM_GRPA_TEMP = 0x8E,
  MB_SNR_CPU5_DIMM_GRPB_TEMP = 0x8F,
  MB_SNR_CPU5_DIMM_GRPC_TEMP = 0x90,
  MB_SNR_CPU5_DIMM_GRPD_TEMP = 0x91,
  MB_SNR_CPU5_DIMM_GRPE_TEMP = 0x92,
  MB_SNR_CPU5_DIMM_GRPF_TEMP = 0x93,

  MB_SNR_CPU6_DIMM_GRPA_TEMP = 0x94,
  MB_SNR_CPU6_DIMM_GRPB_TEMP = 0x95,
  MB_SNR_CPU6_DIMM_GRPC_TEMP = 0x96,
  MB_SNR_CPU6_DIMM_GRPD_TEMP = 0x97,
  MB_SNR_CPU6_DIMM_GRPE_TEMP = 0x98,
  MB_SNR_CPU6_DIMM_GRPF_TEMP = 0x99,

  MB_SNR_CPU7_DIMM_GRPA_TEMP = 0x9A,
  MB_SNR_CPU7_DIMM_GRPB_TEMP = 0x9B,
  MB_SNR_CPU7_DIMM_GRPC_TEMP = 0x9C,
  MB_SNR_CPU7_DIMM_GRPD_TEMP = 0x9D,
  MB_SNR_CPU7_DIMM_GRPE_TEMP = 0x9E,
  MB_SNR_CPU7_DIMM_GRPF_TEMP = 0x9F,

//Board TEMP
  MB_SNR_INLET_TEMP = 0xA0,
  MB_SNR_OUTLET_TEMP_R = 0xA1,
  MB_SNR_OUTLET_TEMP_L = 0xA2,
  MB_SNR_INLET_REMOTE_TEMP = 0xA3,
  MB_SNR_OUTLET_REMOTE_TEMP_R = 0xA4,
  MB_SNR_OUTLET_REMOTE_TEMP_L = 0xA5,
//CPU TEMP
  MB_SNR_CPU0_TEMP = 0xA8,
  MB_SNR_CPU1_TEMP = 0xA9,
  MB_SNR_CPU2_TEMP = 0xAA,
  MB_SNR_CPU3_TEMP = 0xAB,
  MB_SNR_CPU4_TEMP = 0xAC,
  MB_SNR_CPU5_TEMP = 0xAD,
  MB_SNR_CPU6_TEMP = 0xAE,
  MB_SNR_CPU7_TEMP = 0xAF,

//CPU VR
  MB_SNR_VR_CPU0_VCCIN_VOLT = 0xB0,
  MB_SNR_VR_CPU0_VCCIN_TEMP = 0xB1,
  MB_SNR_VR_CPU0_VCCIN_CURR = 0xB2,
  MB_SNR_VR_CPU0_VCCIN_POWER = 0xB3,
  MB_SNR_VR_CPU0_VSA_VOLT = 0xB4,
  MB_SNR_VR_CPU0_VSA_TEMP = 0xB5,
  MB_SNR_VR_CPU0_VSA_CURR = 0xB6,
  MB_SNR_VR_CPU0_VSA_POWER = 0xB7,
  MB_SNR_VR_CPU0_VCCIO_VOLT = 0xB8,
  MB_SNR_VR_CPU0_VCCIO_TEMP = 0xB9,
  MB_SNR_VR_CPU0_VCCIO_CURR = 0xBA,
  MB_SNR_VR_CPU0_VCCIO_POWER = 0xBB,
  MB_SNR_VR_CPU0_VDDQ_GRPABC_VOLT = 0xBC,
  MB_SNR_VR_CPU0_VDDQ_GRPABC_TEMP = 0xBD,
  MB_SNR_VR_CPU0_VDDQ_GRPABC_CURR = 0xBE,
  MB_SNR_VR_CPU0_VDDQ_GRPABC_POWER = 0xBF,
  MB_SNR_VR_CPU0_VDDQ_GRPDEF_VOLT = 0xC0,
  MB_SNR_VR_CPU0_VDDQ_GRPDEF_TEMP = 0xC1,
  MB_SNR_VR_CPU0_VDDQ_GRPDEF_CURR = 0xC2,
  MB_SNR_VR_CPU0_VDDQ_GRPDEF_POWER = 0xC3,
//PCH VR
  MB_SNR_VR_PCH_P1V05_VOLT = 0xC4,
  MB_SNR_VR_PCH_P1V05_TEMP = 0xC5,
  MB_SNR_VR_PCH_P1V05_CURR = 0xC6,
  MB_SNR_VR_PCH_P1V05_POWER = 0xC7,
  MB_SNR_VR_PCH_PVNN_VOLT = 0xC8,
  MB_SNR_VR_PCH_PVNN_TEMP = 0xC9,
  MB_SNR_VR_PCH_PVNN_CURR = 0xCA,
  MB_SNR_VR_PCH_PVNN_POWER = 0xCB,
//ADC
  MB_SNR_P5V = 0xD0,
  MB_SNR_P5V_STBY = 0xD1,
  MB_SNR_P3V3_STBY = 0xD2,
  MB_SNR_P3V3 = 0xD3,
  MB_SNR_P3V_BAT = 0xD4,
  MB_SNR_CPU_P1V8 = 0xD5,
  MB_SNR_PCH_P1V8 = 0xD6,
  MB_SNR_CPU0_PVPP_ABC= 0xD7,
  MB_SNR_CPU1_PVPP_ABC= 0xD8,
  MB_SNR_CPU0_PVPP_DEF= 0xD9,
  MB_SNR_CPU1_PVPP_DEF= 0xDA,
  MB_SNR_CPU0_PVTT_ABC= 0xDB,
  MB_SNR_CPU1_PVTT_ABC= 0xDC,
  MB_SNR_CPU0_PVTT_DEF= 0xDD,
  MB_SNR_CPU1_PVTT_DEF= 0xDE,
  MB_SNR_PROCESSOR_FAIL = 0xDF,
//VR CPU1
  MB_SNR_VR_CPU1_VCCIN_VOLT = 0xE0,
  MB_SNR_VR_CPU1_VCCIN_TEMP = 0xE1,
  MB_SNR_VR_CPU1_VCCIN_CURR = 0xE2,
  MB_SNR_VR_CPU1_VCCIN_POWER = 0xE3,
  MB_SNR_VR_CPU1_VSA_VOLT = 0xE4,
  MB_SNR_VR_CPU1_VSA_TEMP = 0xE5,
  MB_SNR_VR_CPU1_VSA_CURR = 0xE6,
  MB_SNR_VR_CPU1_VSA_POWER = 0xE7,
  MB_SNR_VR_CPU1_VCCIO_VOLT = 0xE8,
  MB_SNR_VR_CPU1_VCCIO_TEMP = 0xE9,
  MB_SNR_VR_CPU1_VCCIO_CURR = 0xEA,
  MB_SNR_VR_CPU1_VCCIO_POWER = 0xEB,
  MB_SNR_VR_CPU1_VDDQ_GRPABC_VOLT = 0xEC,
  MB_SNR_VR_CPU1_VDDQ_GRPABC_TEMP = 0xED,
  MB_SNR_VR_CPU1_VDDQ_GRPABC_CURR = 0xEE,
  MB_SNR_VR_CPU1_VDDQ_GRPABC_POWER = 0xEF,
  MB_SNR_VR_CPU1_VDDQ_GRPDEF_VOLT = 0xF0,
  MB_SNR_VR_CPU1_VDDQ_GRPDEF_TEMP = 0xF1,
  MB_SNR_VR_CPU1_VDDQ_GRPDEF_CURR = 0XF2,
  MB_SNR_VR_CPU1_VDDQ_GRPDEF_POWER = 0XF3,
};

typedef struct {
  float ucr_thresh;
  float unc_thresh;
  float unr_thresh;
  float lcr_thresh;
  float lnc_thresh;
  float lnr_thresh;
  float pos_hyst;
  float neg_hyst;
} PAL_SENSOR_THRESHOLD;

enum {
  TEMP = 1,
  CURR,
  VOLT,
  FAN,
  POWER,
  STATE,
};

typedef struct {
  char* snr_name;
  uint8_t id;
  int (*read_sensor) (uint8_t id, float *value);
  uint8_t stby_read;
  PAL_SENSOR_THRESHOLD snr_thresh;
  uint8_t units;
} PAL_SENSOR_MAP;

//ADC INFO
enum {
  ADC0 = 0,
  ADC1,
  ADC2,
  ADC3,
  ADC4,
  ADC5,
  ADC6,
  ADC7,
  ADC8,
  ADC9,
  ADC10,
  ADC11,
  ADC12,
  ADC13,
  ADC14,
  ADC_NUM_CNT,
};

//INA260 INFO
enum {
  INA260_ID0,
  INA260_ID1,
  INA260_ID2,
  INA260_ID3,
};

//GENERIC I2C Sensors
enum {
  TEMP_INLET = 0,
  TEMP_OUTLET_L,
  TEMP_OUTLET_R,
  TEMP_REMOTE_INLET,
  TEMP_REMOTE_OUTLET_R,
  TEMP_REMOTE_OUTLET_L, 
};

//NIC INFO
enum {
  MEZZ0 = 0,
  MEZZ1,
};

//HARD DISK INFO
enum {
  DISK_BOOT = 0,
  DISK_DATA0,
  DISK_DATA1,
};

//PECI CPU INFO
enum {
  CPU_ID0 = 0,
  CPU_ID1,
  CPU_ID2,
  CPU_ID3,
  CPU_ID4,
  CPU_ID5,
  CPU_ID6,
  CPU_ID7,
  CPU_ID_NUM,
};

typedef struct {
  uint8_t cpu_id;
  uint8_t cpu_addr;
}PAL_CPU_INFO;

enum {
  DIMM_CRPA = 0,
  DIMM_CRPB,
  DIMM_CRPC,
  DIMM_CRPD,
  DIMM_CRPE,
  DIMM_CRPF,
  DIMM_CNT,
};

typedef struct {
  uint8_t cpu_addr;
  uint8_t index;
  uint8_t dev_info;
  uint8_t para_l;
  uint8_t para_h;
} PECI_RD_PKG_CONFIG_INFO;

//ADM1278 INFO
enum {
  ADM1278_VOLTAGE = 0,
  ADM1278_CURRENT,
  ADM1278_POWER,
  ADM1278_TEMP,
};

typedef struct {
  uint8_t type;
  float m;
  float b;
  float r;
} PAL_ADM1278_INFO;

//HSC INFO
enum {
  HSC_ID0,
  HSC_ID1,
  HSC_ID2,
  HSC_ID3, 
};

typedef struct {
  uint8_t id;
  uint8_t slv_addr;
  uint8_t ch;
  PAL_ADM1278_INFO* info;
} PAL_HSC_INFO;

//NM ID
enum {
  NM_ID0,
};

//PCH SENSOR INFO
enum {
  NM_PCH_TEMP = 8,
};

typedef struct {
  uint8_t id;
  uint8_t bus;
  uint8_t slv_addr;
} PAL_I2C_BUS_INFO;

//CM SENSOR INFO
enum {
  CM_FAN0_INLET_SPEED,
  CM_FAN1_INLET_SPEED,
  CM_FAN2_INLET_SPEED,
  CM_FAN3_INLET_SPEED,
  CM_FAN0_OUTLET_SPEED,
  CM_FAN1_OUTLET_SPEED,
  CM_FAN2_OUTLET_SPEED,
  CM_FAN3_OUTLET_SPEED,
  CM_HSC_VIN,
  CM_HSC_IOUT,
  CM_HSC_TEMP,
  CM_HSC_PIN,
  CM_HSC_PEAK_IOUT,
  CM_HSC_PEAK_PIN,
  CM_P12V,
  CM_P3V,
  CM_FAN0_VOLT,
  CM_FAN1_VOLT,
  CM_FAN2_VOLT,
  CM_FAN3_VOLT,
  CM_FAN0_CURR,
  CM_FAN1_CURR,
  CM_FAN2_CURR,
  CM_FAN3_CURR,
  CM_FAN0_POWER,
  CM_FAN1_POWER,
  CM_FAN2_POWER,
  CM_FAN3_POWER,
  CM_SNR_CNT,
};

typedef struct {
  uint8_t id;
  uint8_t num;
} PAL_CM_SENSOR_HEAD;

typedef struct {
  uint8_t snr_num;
  uint8_t available;
  uint8_t val_h;
  uint8_t val_l;
} PAL_CM_SENSOR_INFO;

//VR TPS53688 INFO
enum {
  VR_ID0 = 0,
  VR_ID1,
  VR_ID2,
  VR_ID3,
  VR_ID4,
  VR_ID5,
  VR_ID6,
  VR_ID7,
  VR_ID8,
  VR_ID9,
  VR_ID10,
  VR_ID11,
  VR_NUM_CNT,
};

enum {
  PCA9545_CH1 = 1,
  PCA9545_CH2 = 2,
  PCA9545_CH3 = 4,
  PCA9545_CH4 = 8,
  PCA9545_NOT_USE,
};

typedef struct {
  uint8_t mode;
  uint8_t position;
  uint8_t type;
} BMC_SYSTEM_INFO;

int cmd_peci_get_cpu_err_num(int* num, uint8_t is_caterr);
int pal_sensor_monitor_initial(void);
bool pal_check_nic_prsnt(uint8_t fru);
bool pal_check_dimm_prsnt(uint8_t snr_num);

extern size_t pal_pwm_cnt;
extern size_t pal_tach_cnt;
extern const char pal_pwm_list[];
extern const char pal_tach_list[];
extern const char pal_fru_list[];
extern const char pal_server_list[];
#endif
