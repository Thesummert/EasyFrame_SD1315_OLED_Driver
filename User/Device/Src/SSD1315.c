#include "SSD1315.h"
#include "SSD1315_regs.h"
#include "bsp_mspm0g_tim_base.h"
#include "mcu_config.h"
#include "ti/driverlib/dl_aesadv.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 初始化命令 规定第0位为数据长度 第1位为CMD 之后的为命令参数
 * 这里会配置一个默认设置 可以在之后的初始化中修改
 */
static uint8_t INIT_CMD[][8] = {
    {0, SSD1315_CMD_DISPLAY_OFF}, // 先关闭显示器
    {1, SSD1315_CMD_SET_MEMORY_ADDR_MODE,
     SSD1315_MEM_ADDR_PAGE}, // 重置显示地址模式 为PAGE模式
    {1, SSD1315_CMD_SET_CHARGE_PUMP,
     SSD1315_CHARGE_PUMP_ENABLE_7V5}, // 启用电荷泵到7.5v

    // 设置0为列起始地址
    {0, SSD1315_CMD_SET_LOWER_COLUMN(0)},
    {0, SSD1315_CMD_SET_HIGHER_COLUMN(0)},
    // {2, SSD1315_CMD_SET_COLUMN_ADDR, SSD1315_COL_MIN,
    //  SSD1315_COL_MAX}, // 设定列范围
    {2, SSD1315_CMD_SET_PAGE_ADDR, SSD1315_PAGE_MIN,
     SSD1315_PAGE_MAX},                  // 设定页范围
    {1, SSD1315_CMD_FADE_OUT_BLINK, 0},  // 关闭BLINK
    {1, SSD1315_CMD_ZOOM_IN, 0},         // 关闭ZOOM
    {0, SSD1315_CMD_DEACTIVATE_SCROLL},  // 关闭滚动
    {1, SSD1315_CMD_SET_CONTRAST, 0xCF}, // 设定白平衡
    {0, SSD1315_CMD_SEG_REMAP_COL0},     // 设定列方向
    {0, SSD1315_CMD_COM_SCAN_NORMAL},    // 设定扫描方向
    {0, SSD1315_CMD_NORMAL_DISPLAY},     // 普通显示模式
    // {1, SSD1315_CMD_SET_MULTIPLEX_RATIO, 63},  // 设定行数多路复用
    {1, SSD1315_CMD_SET_DISPLAY_OFFSET, 0},    // 设定偏移
    {1, SSD1315_CMD_SET_DISP_CLOCK_DIV, 0x80}, // 设定时钟分频
    {1, SSD1315_CMD_SET_PRECHARGE, 0xF1},      // 设定充电相位设定
    {1, SSD1315_CMD_SET_IREF, 0x10},           // 启用内部参考电流
    {1, SSD1315_CMD_SET_COM_PINS, 0x12},       // 设定COM引脚
    {1, SSD1315_CMD_SET_VCOMH, 0x20},          // 设定取消选择电平
    {0, SSD1315_CMD_ENTIRE_DISPLAY_RESUME},    // 恢复内存屏幕控制
    // {0, SSD1315_CMD_DISPLAY_ON},               // 恢复显示
};

static _Bool I2C_WriteCMD(EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                          _Bool CO, uint8_t buffer_len);

static _Bool I2C_WriteData(EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                           uint8_t buffer_len);

static _Bool I2C_InitDevice(EF_Device_SD1315_I2C_t *self);

_Bool EF_Device_SD1315_I2C_Init(EF_Device_SD1315_I2C_t *self, uint8_t addr,
                                uint8_t height, uint8_t witdh,
                                EasyFrame_I2C_Typedef_t *i2c) {
  if (self == NULL || i2c == NULL) {
    RTT_Print(0, "Null pointer error in ssd1315 init \r\n");
    return false;
  }
  // 判断地址是否正确
  if (addr != SD1315_ADDR_1 && addr != SD1315_ADDR_2) {
    return false;
  }
  memset(self, 0, sizeof(EF_Device_SD1315_I2C_t));
  self->i2c = i2c;
  self->height = height;
  self->witdh = witdh;
  self->i2c_addr = addr;

  self->WriteCMD = I2C_WriteCMD;
  self->WriteData = I2C_WriteData;
  self->InitDevice = I2C_InitDevice;

  self->is_inited = true;

  return true;
}

static _Bool I2C_WriteCMD(EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                          _Bool CO, uint8_t buffer_len) {
  if (self == NULL) {
    RTT_Print(0, "Null pointer error in ssd1315 write cmd \r\n");
    return false;
  }
  if (self->is_inited == false) {
    return false;
  }
  uint8_t send_buffer[buffer_len + 1];
  send_buffer[0] = CO << 7;
  memcpy(send_buffer + 1, buffer + 1, buffer_len);
  return self->i2c->transmit(self->i2c, self->i2c_addr, send_buffer, buffer[0] + 2,
                             0xFFFFF);
}

static _Bool I2C_WriteData(EF_Device_SD1315_I2C_t *self, uint8_t *buffer,
                           uint8_t buffer_len) {
  if (self == NULL) {
    RTT_Print(0, "Null pointer error in ssd1315 write data \r\n");
    return false;
  }
  if (self->is_inited == false) {
    return false;
  }
  uint8_t send_buffer[buffer_len + 1];
  send_buffer[0] = 0x40; // 设定为发送数据
  memcpy(send_buffer + 1, buffer + 1, buffer_len);
  return self->i2c->transmit(self->i2c, self->i2c_addr, send_buffer, buffer[0] + 2,
                             0xFFFFF);
}

static _Bool I2C_InitDevice(EF_Device_SD1315_I2C_t *self) {
  if (self == NULL || self->is_inited == false) {
    RTT_Print(0, "Null pointer error or not inited in ssd1315 init \r\n");
    return false;
  }
  _Bool flag = true;
  for (uint8_t i = 0; i < sizeof(INIT_CMD) / sizeof(INIT_CMD[0]); i++) {
    flag = self->WriteCMD(self, INIT_CMD[i], 0, 8);
    if (flag == false) {
      return false;
    }
    EasyFrameSysTime_Delay_us(1000);
  }

  uint8_t column_set[8] = {2, SSD1315_CMD_SET_COLUMN_ADDR, SSD1315_COL_MIN,
                           self->witdh}; // 设定列范围
  uint8_t multiradio_set[8] = {1, SSD1315_CMD_SET_MULTIPLEX_RATIO,
                               self->height - 1}; // 设定行数多路复用
  uint8_t start[8] = {0, SSD1315_CMD_DISPLAY_ON}; // 恢复显示
  if (self->WriteCMD(self, column_set, 0, 8) == false) {
    return false;
  }
  EasyFrameSysTime_Delay_us(1000);
  if (self->WriteCMD(self, multiradio_set, 0, 8) == false) {
    return false;
  }
  EasyFrameSysTime_Delay_us(1000);
  if (self->WriteCMD(self, multiradio_set, 0, 8) == false) {
    return false;
  }
  EasyFrameSysTime_Delay_us(1000);
  if (self->WriteCMD(self, start, 0, 8) == false) {
    return false;
  }

  return true;
}
