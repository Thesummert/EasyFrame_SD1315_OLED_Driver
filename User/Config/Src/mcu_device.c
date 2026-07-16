#include "mcu_device.h"
#include "bsp_mspm0g_i2c.h"
#include "bsp_mspm0g_tim_base.h"
#include "ti_msp_dl_config.h"
#include "SEGGER_RTT.h"
#include <stdint.h>
#include <stdlib.h>

static EasyFrame_I2C_Typedef_t i2c;
static EF_Device_SD1315_I2C_t ssd1315;
static uint8_t sd1315_buffer[1024];

_Bool EasyFrameDevice_Init() {
  EasyFrameSysTime_Init(4);
  SEGGER_RTT_Init();

  EasyFrame_GPIO_Typedef_t scl, sda;
  EasyFrame_GPIO_Init(&scl, GPIOA, GPIO_I2C_0_SCL_PIN);
  EasyFrame_GPIO_Init(&sda, GPIOA, GPIO_I2C_0_SDA_PIN);
  EasyFrame_GPIO_InitIOMux(&sda, GPIO_I2C_0_IOMUX_SDA);
  EasyFrame_GPIO_InitIOMux(&scl, GPIO_I2C_0_IOMUX_SCL);
  EasyFrame_I2C_Init(&i2c, I2C_0_INST, 1000);
  EasyFrame_I2C_InitGPIO(&i2c, sda, scl);
  EF_Device_SD1315_I2C_Init(&ssd1315, SD1315_ADDR_1, 64, 128, &i2c, sd1315_buffer);
  // i2c.mspm0g.i2c_delay = 100;
  

  return true;
}

EF_Device_SD1315_I2C_t *GetSSD1315() { return &ssd1315; }
