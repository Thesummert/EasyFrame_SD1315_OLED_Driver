#ifndef __SD1315_REGS_H__
#define __SD1315_REGS_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Basic commands */
#define SSD1315_CMD_SET_LOWER_COLUMN(col)        (0x00 | ((col) & 0x0F))
#define SSD1315_CMD_SET_HIGHER_COLUMN(col)       (0x10 | ((col) & 0x0F))
#define SSD1315_CMD_SET_MEMORY_ADDR_MODE         0x20
#define SSD1315_CMD_SET_COLUMN_ADDR              0x21
#define SSD1315_CMD_SET_PAGE_ADDR                0x22
#define SSD1315_CMD_SET_DISPLAY_START_LINE(line) (0x40 | ((line) & 0x3F))

#define SSD1315_CMD_SET_CONTRAST                 0x81
#define SSD1315_CMD_SEG_REMAP_COL0               0xA0
#define SSD1315_CMD_SEG_REMAP_COL127             0xA1
#define SSD1315_CMD_ENTIRE_DISPLAY_RESUME        0xA4
#define SSD1315_CMD_ENTIRE_DISPLAY_ON            0xA5
#define SSD1315_CMD_NORMAL_DISPLAY               0xA6
#define SSD1315_CMD_INVERSE_DISPLAY              0xA7
#define SSD1315_CMD_SET_MULTIPLEX_RATIO          0xA8
#define SSD1315_CMD_SET_IREF                     0xAD
#define SSD1315_CMD_DISPLAY_OFF                  0xAE
#define SSD1315_CMD_DISPLAY_ON                   0xAF

#define SSD1315_CMD_SET_PAGE_START_ADDR(page)    (0xB0 | ((page) & 0x07))
#define SSD1315_CMD_COM_SCAN_NORMAL              0xC0
#define SSD1315_CMD_COM_SCAN_REMAP               0xC8
#define SSD1315_CMD_SET_DISPLAY_OFFSET           0xD3
#define SSD1315_CMD_SET_DISP_CLOCK_DIV           0xD5
#define SSD1315_CMD_SET_PRECHARGE                0xD9
#define SSD1315_CMD_SET_COM_PINS                 0xDA
#define SSD1315_CMD_SET_VCOMH                    0xDB
#define SSD1315_CMD_NOP                          0xE3

/* Charge pump */
#define SSD1315_CMD_SET_CHARGE_PUMP              0x8D

/* Scroll commands */
#define SSD1315_CMD_SCROLL_RIGHT                 0x26
#define SSD1315_CMD_SCROLL_LEFT                  0x27
#define SSD1315_CMD_SCROLL_VERT_RIGHT            0x29
#define SSD1315_CMD_SCROLL_VERT_LEFT             0x2A
#define SSD1315_CMD_DEACTIVATE_SCROLL            0x2E
#define SSD1315_CMD_ACTIVATE_SCROLL              0x2F
#define SSD1315_CMD_SET_VERTICAL_SCROLL_AREA     0xA3

/* Advanced graphic commands */
#define SSD1315_CMD_FADE_OUT_BLINK               0x23
#define SSD1315_CMD_ZOOM_IN                      0xD6

/* Memory addressing mode values */
#define SSD1315_MEM_ADDR_HORIZONTAL              0x00
#define SSD1315_MEM_ADDR_VERTICAL                0x01
#define SSD1315_MEM_ADDR_PAGE                    0x02

/* Seg remap */
#define SSD1315_SEG_REMAP_NORMAL                 0xA0
#define SSD1315_SEG_REMAP_MIRROR                 0xA1


/* Charge pump values */
#define SSD1315_CHARGE_PUMP_DISABLE              0x10
#define SSD1315_CHARGE_PUMP_ENABLE_7V5           0x14
#define SSD1315_CHARGE_PUMP_ENABLE_8V5           0x94
#define SSD1315_CHARGE_PUMP_ENABLE_9V0           0x95

/* Addressing helpers */
#define SSD1315_PAGE_MIN                         0x00
#define SSD1315_PAGE_MAX                         0x07
#define SSD1315_COL_MIN                          0x00
#define SSD1315_COL_MAX                          0x7F

/* Scroll page mapping helpers */
#define SSD1315_SCROLL_PAGE0                     0x00
#define SSD1315_SCROLL_PAGE1                     0x01
#define SSD1315_SCROLL_PAGE2                     0x02
#define SSD1315_SCROLL_PAGE3                     0x03
#define SSD1315_SCROLL_PAGE4                     0x04
#define SSD1315_SCROLL_PAGE5                     0x05
#define SSD1315_SCROLL_PAGE6                     0x06
#define SSD1315_SCROLL_PAGE7                     0x07


#ifdef __cplusplus
}
#endif

#endif /* __SD1315_REGS_H__ */
