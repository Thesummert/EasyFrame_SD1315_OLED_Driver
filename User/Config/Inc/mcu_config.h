#ifndef __MCU_CONFIG_H__
#define __MCU_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define BUS_CLK 80  // 主线时钟

// 是否使用RTT
#define USING_SEGGER_RTT 1

// 配合宏开关可快速启用rtt打印 关闭时几乎不消耗资源 开启优化下应该会优化掉此函数
#if USING_SEGGER_RTT == 1
#include "SEGGER_RTT.h"
#define RTT_Print(X, ...) SEGGER_RTT_printf(X, ##__VA_ARGS__)
#else
#define RTT_Print(X, ...)
#endif


// I2C FIFO BUFFER大小
#define I2C_FIFO_MAX_SIZE 8


#ifdef __cplusplus
}
#endif

#endif /* __MCU_CONFIG_H__ */
