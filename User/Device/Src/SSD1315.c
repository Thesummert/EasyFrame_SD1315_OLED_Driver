#include "SD1315.h"
#include <assert.h>
#include <stdint.h>
#include "SD1315_regs.h"

/*初始化命令 规定第0位为CMD 后一位为数据长度 之后的为命令*/
static const uint8_t INIT_CMD[][8] = {
    {SSD1315_CMD_DISPLAY_OFF, 0,}, // 先关闭显示器
    {SSD1315_CMD_SET_MEMORY_ADDR_MODE, 1, 0b10}， // 重置显示地址模式

};

