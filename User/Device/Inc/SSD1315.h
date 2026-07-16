#ifndef __SD1315_H__
#define __SD1315_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_mspm0g_i2c.h"

/*I2C设备地址*/
typedef enum {
  SD1315_ADDR_1 = 0x3C,
  SD1315_ADDR_2 = 0x3D,
} EF_Device_SD1315_I2CID_e;

typedef enum {
  SSD1315_PUMP_0V,
  SSD1315_PUMP_7V5,
  SSD1315_PUMP_8V5,
  SSD1315_PUMP_9V0,
} EF_Device_SD1315_PUMP_e;

typedef struct EF_Device_SD1315_I2C_t {
  uint8_t i2c_addr;
  uint8_t height;
  uint8_t witdh;
  EF_Device_SD1315_PUMP_e pump_setting;
  EasyFrame_I2C_Typedef_t *i2c;

  _Bool (*WriteCMD)(struct EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                    _Bool CO, uint8_t buffer_len);

  _Bool (*WriteData)(struct EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                     uint8_t buffer_len);

  _Bool (*InitDevice)(struct EF_Device_SD1315_I2C_t *self);

  _Bool is_inited;
} EF_Device_SD1315_I2C_t;

_Bool EF_Device_SD1315_I2C_Init(EF_Device_SD1315_I2C_t *self, uint8_t addr,
                                uint8_t height, uint8_t witdh,
                                EasyFrame_I2C_Typedef_t *i2c);

#ifdef __cplusplus
}
#endif

#endif /* __SD1315_H__ */
