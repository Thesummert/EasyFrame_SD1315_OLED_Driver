#include "SSD1315.h"
#include "SSD1315_regs.h"
#include "bsp_mspm0g_tim_base.h"
#include "mcu_config.h"
#include "ti/driverlib/dl_aesadv.h"
#include <assert.h>
#include <inttypes.h>
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

static _Bool I2C_DrawPoint(EF_Device_SD1315_I2C_t *self, uint8_t x, uint8_t y,
                           _Bool set);

static _Bool I2C_WritePoint(EF_Device_SD1315_I2C_t *self, uint8_t x, uint8_t y,
                            _Bool set);

static _Bool I2C_Clear(EF_Device_SD1315_I2C_t *self);

static _Bool WriteBufferSet(EF_Device_SD1315_I2C_t *self, uint8_t column,
                            uint8_t page);

static _Bool I2C_WriteBuffer(EF_Device_SD1315_I2C_t *self);

_Bool EF_Device_SD1315_I2C_Init(EF_Device_SD1315_I2C_t *self, uint8_t addr,
                                uint8_t height, uint8_t witdh,
                                EasyFrame_I2C_Typedef_t *i2c, uint8_t *buffer) {
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
  self->buffer = buffer;

  self->WriteCMD = I2C_WriteCMD;
  self->WriteData = I2C_WriteData;
  self->InitDevice = I2C_InitDevice;
  self->DrawPoint = I2C_DrawPoint;
  self->Clear = I2C_Clear;
  self->WriteBuffer = I2C_WriteBuffer;
  self->WritePoint = I2C_WritePoint;

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
  return self->i2c->transmit(self->i2c, self->i2c_addr, send_buffer,
                             buffer[0] + 2, 0xFFFFF);
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
  return self->i2c->transmit(self->i2c, self->i2c_addr, send_buffer,
                             buffer[0] + 2, 0xFFFFF);
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
                           self->witdh - 1}; // 设定列范围
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
  // if (self->Clear(self) == false) {
  //   // return false;
  // }
  EasyFrameSysTime_Delay_us(1000);
  if (self->WriteCMD(self, start, 0, 8) == false) {
    return false;
  }

  return true;
}

static _Bool I2C_DrawPoint(EF_Device_SD1315_I2C_t *self, uint8_t x, uint8_t y,
                           _Bool set) {
  if (self == NULL || self->is_inited == false) {
    RTT_Print(0, "Null pointer error or not inited in ssd1315 init \r\n");
    return false;
  }

  if (x > SSD1315_COL_MAX || y > (SSD1315_PAGE_MAX + 1) * 8) {
    RTT_Print(0, "x or y is invalid in ssd1315 draw point \r\n");
    return false;
  }

  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  uint16_t index = page * (SSD1315_COL_MAX + 1) + x;
  uint8_t mask = 1U << bit;
  _Bool send_flag = true;

  if (set) {
    self->buffer[index] |= mask;
  } else {
    self->buffer[index] &= (uint8_t)~mask;
  }
  uint8_t send_buffer[][8] = {{0, SSD1315_CMD_SET_PAGE_START_ADDR(page)},
                              {0, SSD1315_CMD_SET_LOWER_COLUMN(x)},
                              {0, SSD1315_CMD_SET_HIGHER_COLUMN(x >> 4)},
                              {0, self->buffer[index]}};

  for (uint16_t i = 0; i < 3; i++) {
    send_flag = self->WriteCMD(self, send_buffer[i], false, 8);
  }
  send_flag = self->WriteData(self, send_buffer[3], 8);
  return send_flag;
}

static _Bool I2C_Clear(EF_Device_SD1315_I2C_t *self) {
  if (self == NULL || self->is_inited == false) {
    RTT_Print(0, "Null pointer error or not inited in ssd1315 clear \r\n");
    return false;
  }
  _Bool flag = true;
  for (uint8_t i = 0; i < SSD1315_COL_MAX + 1; i++) {

    uint8_t send_buffer[][8] = {{0, SSD1315_CMD_SET_LOWER_COLUMN(i)},
                                {0, SSD1315_CMD_SET_HIGHER_COLUMN(i >> 4)},
                                {0, SSD1315_CMD_SET_PAGE_START_ADDR(0)},
                                {0, 0}};
    for (uint16_t c = 0; c < 2; c++) {
      flag = self->WriteCMD(self, send_buffer[c], false, 8);
    }
    for (uint8_t j = 0; j < SSD1315_PAGE_MAX + 1; j++) {
      send_buffer[2][1] = SSD1315_CMD_SET_PAGE_START_ADDR(j);
      self->WriteCMD(self, send_buffer[2], false, 8);
      flag = self->WriteData(self, send_buffer[3], 8);
    }
  }
  return flag;
}

static _Bool I2C_WritePoint(EF_Device_SD1315_I2C_t *self, uint8_t x, uint8_t y,
                            _Bool set) {
  if (self == NULL || self->is_inited == false) {
    RTT_Print(0, "Null pointer error or not inited in ssd1315 write point\r\n");
    return false;
  }

  if (x > SSD1315_COL_MAX || y > (SSD1315_PAGE_MAX + 1) * 8) {
    RTT_Print(0, "x or y is invalid in ssd1315 draw point \r\n");
    return false;
  }

  // 将内存缓冲区中对应的部分写入数据
  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  uint16_t index = page * (SSD1315_COL_MAX + 1) + x;
  uint8_t mask = 1U << bit;

  if (set) {
    self->buffer[index] |= mask;
  } else {
    self->buffer[index] &= (uint8_t)~mask;
  }
  WriteBufferSet(self, x, page);

  return true;
}

static _Bool WriteBufferSet(EF_Device_SD1315_I2C_t *self, uint8_t column,
                            uint8_t page) {
  /*在这里标记需要刷新的内存区域*/
  if (self == NULL || self->is_inited == false) {
    RTT_Print(0, "Null pointer error or not inited in ssd1315 fresh set \r\n");
    return false;
  }
  /*检查是否需要刷新缓冲区*/
  if (self->need_fresh[column].pages_flag > 0) {
    if ((self->need_fresh[column].pages_flag & (1U << page)) == 0) {
      self->need_fresh[column].pages_flag |= (1 << page);
    }
  } else {
    self->need_fresh[column].pages_flag |= (1 << page);
    self->fresh_area[self->fresh_num] = column;
    self->fresh_num += 1; // 添加需要刷新的数量
  }

  return true;
}

static _Bool I2C_WriteBuffer(EF_Device_SD1315_I2C_t *self) {
  /*将内存缓冲区中的数据写入到屏幕中*/

  if (self == NULL || self->is_inited == false) {
    RTT_Print(0,
              "Null pointer error or not inited in ssd1315 write buffer \r\n");
    return false;
  }
  _Bool flag = true;
  for (uint8_t i = 0; i < self->fresh_num; i++) {
    uint8_t send_buffer[][8] = {
        {0, SSD1315_CMD_SET_LOWER_COLUMN(self->fresh_area[i])},
        {0, SSD1315_CMD_SET_HIGHER_COLUMN(self->fresh_area[i] >> 4)}};
    for (uint8_t j = 0; j < 8; j++) {
      // 设定列地址
      self->WriteCMD(self, send_buffer[0], false, 8);
      self->WriteCMD(self, send_buffer[1], false, 8);
      if (self->need_fresh[self->fresh_area[i]].pages_flag & (1U << j)) {
        uint8_t page = j;
        uint16_t index = page * (SSD1315_COL_MAX + 1) + self->fresh_area[i];
        uint8_t buffer[][8] = {{0, SSD1315_CMD_SET_PAGE_START_ADDR(page)},
                               {0, self->buffer[index]}};
        self->WriteCMD(self, buffer[0], false, 8);
        flag = self->WriteData(self, buffer[1], 8);
      }
    }
  }
  memset(&self->fresh_area, 0, 128);
  memset(&self->need_fresh, 0, sizeof(self->need_fresh));
  self->fresh_num = 0;
  return flag;
}
