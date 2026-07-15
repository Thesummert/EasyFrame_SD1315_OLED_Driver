#ifndef __SD1315_H__
#define __SD1315_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_mspm0g_i2c.h"

/*I2C设备地址*/
typedef enum {
  SD1315_ADDR_1 = 0x78,
  SD1315_ADDR_2 = 0x7A,
} EF_Device_SD1315_I2CID_e;

typedef struct EF_Device_SD1315_I2C_t {
  uint8_t i2c_addr;
  uint16_t height;
  uint16_t witdh;
  EasyFrame_I2C_Typedef_t *i2c;
} EF_Device_SD1315_I2C_t;

#ifdef __cplusplus
}
#endif

#endif /* __SD1315_H__ */
