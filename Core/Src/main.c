#include "SSD1315.h"
#include "bsp_mspm0g_tim_base.h"
#include "mcu_config.h"
#include "ti_msp_dl_config.h"
//

#include "mcu_device.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/select.h>

EF_Device_SD1315_I2C_t *ssd;
_Bool flag = false;
_Bool ans = false;
uint8_t str[] = "ABC";

int main(void) {
  SYSCFG_DL_init();
  EasyFrameDevice_Init();
  ssd = GetSSD1315();
      ans = ssd->InitDevice(ssd);

  while (1) {
      ssd->WriteString(ssd,0, 20, SSD1315_FONT_12, str, 1);
      ssd->WriteBuffer(ssd);
      EasyFrameSysTime_Delay(1);
      ssd->WriteString(ssd,0, 20, SSD1315_FONT_12, str, 0);
      ssd->WriteBuffer(ssd);
      EasyFrameSysTime_Delay(1);
  }
}
