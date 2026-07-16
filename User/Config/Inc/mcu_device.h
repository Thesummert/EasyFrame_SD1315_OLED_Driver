#ifndef __MCU_DEVICE_H__
#define __MCU_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SSD1315.h"

_Bool EasyFrameDevice_Init();

EF_Device_SD1315_I2C_t *GetSSD1315();

#ifdef __cplusplus
}
#endif

#endif /* __MCU_DEVICE_H__ */
