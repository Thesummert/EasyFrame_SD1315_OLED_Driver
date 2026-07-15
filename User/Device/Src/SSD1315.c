#include "SSD1315.h"
#include "SSD1315_regs.h"
#include "mcu_config.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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
    {2, SSD1315_CMD_SET_COLUMN_ADDR, SSD1315_COL_MIN,
     SSD1315_COL_MAX}, // 设定列范围
    {2, SSD1315_CMD_SET_PAGE_ADDR, SSD1315_PAGE_MIN,
     SSD1315_PAGE_MAX},                        // 设定页范围
    {1, SSD1315_CMD_FADE_OUT_BLINK, 0},        // 关闭BLINK
    {1, SSD1315_CMD_ZOOM_IN, 0},               // 关闭ZOOM
    {0, SSD1315_CMD_DEACTIVATE_SCROLL},        // 关闭滚动
    {1, SSD1315_CMD_SET_CONTRAST, 0xCF},       // 设定白平衡
    {0, SSD1315_CMD_SEG_REMAP_COL0},           // 设定列方向
    {0, SSD1315_CMD_COM_SCAN_NORMAL},          // 设定扫描方向
    {0, SSD1315_CMD_NORMAL_DISPLAY},           // 普通显示模式
    {1, SSD1315_CMD_SET_MULTIPLEX_RATIO, 63},  // 设定行数多路复用
    {1, SSD1315_CMD_SET_DISPLAY_OFFSET, 0},    // 设定偏移
    {1, SSD1315_CMD_SET_DISP_CLOCK_DIV, 0x80}, // 设定时钟分频
    {1, SSD1315_CMD_SET_PRECHARGE, 0xF1},      // 设定充电相位设定
    {1, SSD1315_CMD_SET_IREF, 0x10},           // 启用内部参考电流
    {1, SSD1315_CMD_SET_COM_PINS, 0x12},       // 设定COM引脚
    {1, SSD1315_CMD_SET_VCOMH, 0x20},          // 设定取消选择电平
    {0, SSD1315_CMD_ENTIRE_DISPLAY_RESUME},    // 恢复内存屏幕控制
    {0, SSD1315_CMD_DISPLAY_ON},               // 恢复显示
};

static _Bool WriteCMD(EF_Device_SD1315_I2C_t *self, uint8_t *buffer);

static _Bool WriteCMD(EF_Device_SD1315_I2C_t *self, uint8_t *buffer) {
  if (self == NULL) {
    RTT_Print(0, "Null pointer error in ssd1315 write cmd \r\n");
    return false;
  }
  if (self->is_inited == false) {
    return false;
  }
  return self->i2c->transmit(self->i2c, self->i2c_addr, buffer + 1,
                             buffer[0] + 1, 0xFFFFF);
}
