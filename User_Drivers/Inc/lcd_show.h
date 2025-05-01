#ifndef __LCD_SHOW_H__
#define __LCD_SHOW_H__

#include "dma2d.h"
#include "lcd_fonts.h"
#include "main.h"
#include <stdio.h>

#define LCD_NUM_LAYERS 2

#define ColorMode_0    LTDC_PIXEL_FORMAT_RGB565

#if LCD_NUM_LAYERS == 2

#define ColorMode_1 LTDC_PIXEL_FORMAT_ARGB1555

#endif

#define Direction_H   0
#define Direction_V   1

#define Fill_Zero     0
#define Fill_Space    1

#define LCD_WHITE     0xffFFFFFF
#define LCD_BLACK     0xff000000

#define LCD_BLUE      0xff0000FF
#define LCD_GREEN     0xff00FF00
#define LCD_RED       0xffFF0000
#define LCD_CYAN      0xff00FFFF
#define LCD_MAGENTA   0xffFF00FF
#define LCD_YELLOW    0xffFFFF00
#define LCD_GREY      0xff2C2C2C

#define LIGHT_BLUE    0xff8080FF
#define LIGHT_GREEN   0xff80FF80
#define LIGHT_RED     0xffFF8080
#define LIGHT_CYAN    0xff80FFFF
#define LIGHT_MAGENTA 0xffFF80FF
#define LIGHT_YELLOW  0xffFFFF80
#define LIGHT_GREY    0xffA3A3A3

#define DARK_BLUE     0xff000080
#define DARK_GREEN    0xff008000
#define DARK_RED      0xff800000
#define DARK_CYAN     0xff008080
#define DARK_MAGENTA  0xff800080
#define DARK_YELLOW   0xff808000
#define DARK_GREY     0xff404040

void LCD_RGB_Init(void);
void LCD_Clear(void);
void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void LCD_SetLayer(uint8_t Layerx);
void LCD_SetColor(uint32_t Color);
void LCD_SetBackColor(uint32_t Color);
void LCD_DisplayDirection(uint8_t direction);

void LCD_SetFont(pFONT *fonts);
void LCD_DisplayChar(uint16_t x, uint16_t y, uint8_t c);
void LCD_DisplayString(uint16_t x, uint16_t y, char *p);

void LCD_SetTextFont(pFONT *fonts);
void LCD_DisplayChinese(uint16_t x, uint16_t y, char *pText);
void LCD_DisplayText(uint16_t x, uint16_t y, char *pText);

void LCD_ShowNumMode(uint8_t mode);
void LCD_DisplayNumber(uint16_t x, uint16_t y, int32_t number, uint8_t len);
void LCD_DisplayDecimals(uint16_t x, uint16_t y, double number, uint8_t len,
                         uint8_t decs);

void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                   const uint8_t *pImage);

void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color);
uint32_t LCD_ReadPoint(uint16_t x, uint16_t y);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);
void LCD_DrawEllipse(int x, int y, int r1, int r2);

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r);

#define HBP           80
#define VBP           40
#define HSW           1
#define VSW           1
#define HFP           200
#define VFP           22

#define LCD_Width     800
#define LCD_Height    480
#define LCD_MemoryAdd SDRAM_BANK_ADDR

#if (ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 ||                                \
     ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 ||                              \
     ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB4444)
#define BytesPerPixel_0 2
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
#define BytesPerPixel_0 3
#else
#define BytesPerPixel_0 4
#endif

#if LCD_NUM_LAYERS == 2

#if (ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 ||                                \
     ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 ||                              \
     ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB4444)
#define BytesPerPixel_1 2
#elif ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888
#define BytesPerPixel_1 3
#else
#define BytesPerPixel_1 4
#endif

#define LCD_MemoryAdd_OFFSET LCD_Width *LCD_Height *BytesPerPixel_0

#endif

#define LCD_Backlight_PIN             GPIO_PIN_6
#define LCD_Backlight_PORT            GPIOH
#define GPIO_LDC_Backlight_CLK_ENABLE __HAL_RCC_GPIOH_CLK_ENABLE()

#define LCD_Backlight_OFF                                                      \
    HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);
#define LCD_Backlight_ON                                                       \
    HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);

#endif
