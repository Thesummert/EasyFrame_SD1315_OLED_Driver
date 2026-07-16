#include "SSD1315.h"
#include "mcu_config.h"
#include "ti_msp_dl_config.h"
//

#include "mcu_device.h"
#include <stdbool.h>

EF_Device_SD1315_I2C_t *ssd;
_Bool flag = false;
_Bool ans = false;

int main(void) {
  SYSCFG_DL_init();
  EasyFrameDevice_Init();
  ssd = GetSSD1315();
  
  while (1) {
          ans = ssd->InitDevice(ssd);
      if (flag == true) {
          ans = false;
          ans = ssd->InitDevice(ssd);
          flag = false;
      }
  }
}
