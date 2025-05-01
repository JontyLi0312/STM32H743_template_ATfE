#include "lcd_show.h"
#include "sdram.h"
#include <stdint.h>
#include <sys/_intsup.h>

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

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

static pFONT *LCD_Fonts;
static pFONT *LCD_CHFonts;

struct {
    uint32_t Color;
    uint32_t BackColor;
    uint32_t ColorMode;
    uint32_t LayerMemoryAdd;
    uint8_t Layer;
    uint8_t Direction;
    uint8_t BytesPerPixel;
    uint8_t ShowNum_Mode;
} LCD;

void LCD_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_LDC_Backlight_CLK_ENABLE;

    GPIO_InitStruct.Pin   = LCD_Backlight_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_Backlight_PORT, &GPIO_InitStruct);

    LCD_Backlight_OFF;
}

void LCD_RGB_Init(void)
{
    LTDC_LayerCfgTypeDef pLayerCfg = {0}; // layer0 ��ز���

    pLayerCfg.WindowX0    = 0;
    pLayerCfg.WindowX1    = LCD_Width;
    pLayerCfg.WindowY0    = 0;
    pLayerCfg.WindowY1    = LCD_Height;
    pLayerCfg.ImageWidth  = LCD_Width;
    pLayerCfg.ImageHeight = LCD_Height;
    pLayerCfg.PixelFormat = ColorMode_0;

    pLayerCfg.Alpha = 255;

    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

    pLayerCfg.FBStartAdress = LCD_MemoryAdd;

    pLayerCfg.Alpha0          = 0;
    pLayerCfg.Backcolor.Blue  = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red   = 0;

    HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0);

#if ((ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888) ||                              \
     (ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB8888))

    HAL_LTDC_EnableDither(&hltdc);

#endif

#if (LCD_NUM_LAYERS == 2)

    LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

    pLayerCfg1.WindowX0    = 0;
    pLayerCfg1.WindowX1    = LCD_Width;
    pLayerCfg1.WindowY0    = 0;
    pLayerCfg1.WindowY1    = LCD_Height;
    pLayerCfg1.ImageWidth  = LCD_Width;
    pLayerCfg1.ImageHeight = LCD_Height;
    pLayerCfg1.PixelFormat = ColorMode_1;
    pLayerCfg1.Alpha       = 255;

    pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

    pLayerCfg1.FBStartAdress = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;

    pLayerCfg1.Alpha0          = 0;
    pLayerCfg1.Backcolor.Red   = 0;
    pLayerCfg1.Backcolor.Green = 0;
    pLayerCfg1.Backcolor.Blue  = 0;

    HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1);

#if ((ColorMode_1 == LTDC_PIXEL_FORMAT_RGB888) ||                              \
     (ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB8888))
    HAL_LTDC_EnableDither(&hltdc); // ������ɫ����
#endif

#endif

    LCD_GPIO_Init();

    LCD_DisplayDirection(Direction_H);
    LCD_SetFont(&Font24);
    LCD_ShowNumMode(Fill_Space);

    LCD_SetLayer(0);
    LCD_SetBackColor(LCD_BLACK);
    LCD_SetColor(LCD_WHITE);
    LCD_Clear();

#if LCD_NUM_LAYERS == 2

    LCD_SetLayer(1);
    LCD_SetBackColor(LCD_BLACK);
    LCD_SetColor(LCD_WHITE);
    LCD_Clear();

#endif

    LCD_Backlight_ON;
}

void LCD_SetLayer(uint8_t layer)
{
#if LCD_NUM_LAYERS == 2

    if (layer == 0) {
        LCD.LayerMemoryAdd = LCD_MemoryAdd;
        LCD.ColorMode      = ColorMode_0;
        LCD.BytesPerPixel  = BytesPerPixel_0;
    } else if (layer == 1) {
        LCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;
        LCD.ColorMode      = ColorMode_1;
        LCD.BytesPerPixel  = BytesPerPixel_1;
    }
    LCD.Layer = layer;

#else

    LCD.LayerMemoryAdd = LCD_MemoryAdd;
    LCD.ColorMode      = ColorMode_0;
    LCD.BytesPerPixel  = BytesPerPixel_0;
    LCD.Layer          = 0;

#endif
}

void LCD_SetColor(uint32_t Color)
{
    uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565) {
        Red_Value   = (uint16_t)((Color & 0x00F80000) >> 8);
        Green_Value = (uint16_t)((Color & 0x0000FC00) >> 5);
        Blue_Value  = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.Color   = (uint16_t)(Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555) {
        if ((Color & 0xFF000000) == 0)
            Alpha_Value = 0x0000;
        else
            Alpha_Value = 0x8000;

        Red_Value   = (uint16_t)((Color & 0x00F80000) >> 9);
        Green_Value = (uint16_t)((Color & 0x0000F800) >> 6);
        Blue_Value  = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.Color =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444) {

        Alpha_Value = (uint16_t)((Color & 0xf0000000) >> 16);
        Red_Value   = (uint16_t)((Color & 0x00F00000) >> 12);
        Green_Value = (uint16_t)((Color & 0x0000F000) >> 8);
        Blue_Value  = (uint16_t)((Color & 0x000000F8) >> 4);
        LCD.Color =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else
        LCD.Color = Color;
}

void LCD_SetBackColor(uint32_t Color)
{
    uint16_t Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB565) {
        Red_Value     = (uint16_t)((Color & 0x00F80000) >> 8);
        Green_Value   = (uint16_t)((Color & 0x0000FC00) >> 5);
        Blue_Value    = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB1555) {
        if ((Color & 0xFF000000) == 0)
            Alpha_Value = 0x0000;
        else
            Alpha_Value = 0x8000;

        Red_Value   = (uint16_t)((Color & 0x00F80000) >> 9);
        Green_Value = (uint16_t)((Color & 0x0000F800) >> 6);
        Blue_Value  = (uint16_t)((Color & 0x000000F8) >> 3);
        LCD.BackColor =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    } else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB4444) {

        Alpha_Value = (uint16_t)((Color & 0xf0000000) >> 16);
        Red_Value   = (uint16_t)((Color & 0x00F00000) >> 12);
        Green_Value = (uint16_t)((Color & 0x0000F000) >> 8);
        Blue_Value  = (uint16_t)((Color & 0x000000F8) >> 4);
        LCD.BackColor =
            (uint16_t)(Alpha_Value | Red_Value | Green_Value | Blue_Value);
    }

    else
        LCD.BackColor = Color;
}

void LCD_SetFont(pFONT *fonts)
{
    LCD_Fonts = fonts;
}

void LCD_DisplayDirection(uint8_t direction)
{
    LCD.Direction = direction;
}

void LCD_Clear(void)
{

    DMA2D->CR &= ~(DMA2D_CR_START);
    DMA2D->CR     = DMA2D_R2M;
    DMA2D->OPFCCR = LCD.ColorMode;
    DMA2D->OOR    = 0;
    DMA2D->OMAR   = LCD.LayerMemoryAdd;
    DMA2D->NLR    = (LCD_Width << 16) | (LCD_Height);
    DMA2D->OCOLR  = LCD.BackColor;

    while (LTDC->CDSR != 0X00000001);

    DMA2D->CR |= DMA2D_CR_START;

    while (DMA2D->CR & DMA2D_CR_START);
}

void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

    DMA2D->CR &= ~(DMA2D_CR_START);
    DMA2D->CR     = DMA2D_R2M;
    DMA2D->OPFCCR = LCD.ColorMode;
    DMA2D->OCOLR  = LCD.BackColor;

    if (LCD.Direction == Direction_H) {
        DMA2D->OOR = LCD_Width - width;
        DMA2D->OMAR =
            LCD.LayerMemoryAdd + LCD.BytesPerPixel * (LCD_Width * y + x);
        DMA2D->NLR = (width << 16) | (height);
    } else {
        DMA2D->OOR = LCD_Width - height;
        DMA2D->OMAR =
            LCD.LayerMemoryAdd +
            LCD.BytesPerPixel * ((LCD_Height - x - 1 - width) * LCD_Width + y);
        DMA2D->NLR = (width) | (height << 16);
    }

    DMA2D->CR |= DMA2D_CR_START;

    while (DMA2D->CR & DMA2D_CR_START);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{

    /*----------------------- 32位色 ARGB8888 模式 ----------------------*/

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888) {
        if (LCD.Direction == Direction_H) // 水平方向
        {
            *(__IO uint32_t *)(LCD.LayerMemoryAdd + 4 * (x + y * LCD_Width)) =
                color;
        } else if (LCD.Direction == Direction_V) // 垂直方向
        {
            *(__IO uint32_t *)(LCD.LayerMemoryAdd +
                               4 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
        }
    }
    /*----------------------------- 24位色 RGB888 模式
       -------------------------*/

    else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888) {
        if (LCD.Direction == Direction_H) // 水平方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAdd + 3 * (x + y * LCD_Width)) =
                color;
            *(__IO uint8_t *)(LCD.LayerMemoryAdd + 3 * (x + y * LCD_Width) +
                              2) = color >> 16;
        } else if (LCD.Direction == Direction_V) // 垂直方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAdd +
                               3 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
            *(__IO uint8_t *)(LCD.LayerMemoryAdd +
                              3 * ((LCD_Height - x - 1) * LCD_Width + y) + 2) =
                color >> 16;
        }
    }

    /*----------------------- 16位色 ARGB1555、RGB565或者ARGB4444 模式
       ----------------------*/
    else {
        if (LCD.Direction == Direction_H) // 水平方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAdd + 2 * (x + y * LCD_Width)) =
                color;
        } else if (LCD.Direction == Direction_V) // 垂直方向
        {
            *(__IO uint16_t *)(LCD.LayerMemoryAdd +
                               2 * ((LCD_Height - x - 1) * LCD_Width + y)) =
                color;
        }
    }
}

uint32_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
    uint32_t color = 0;

    if (LCD.ColorMode == LTDC_PIXEL_FORMAT_ARGB8888) {
        if (LCD.Direction == Direction_H) {
            color = *(__IO uint32_t *)(LCD.LayerMemoryAdd +
                                       4 * (x + y * LCD_Width));
        } else if (LCD.Direction == Direction_V) {
            color =
                *(__IO uint32_t *)(LCD.LayerMemoryAdd +
                                   4 * ((LCD_Height - x - 1) * LCD_Width + y));
        }
    }

    else if (LCD.ColorMode == LTDC_PIXEL_FORMAT_RGB888) {
        if (LCD.Direction == Direction_H) {
            color = *(__IO uint32_t *)(LCD.LayerMemoryAdd +
                                       3 * (x + y * LCD_Width)) &
                    0x00ffffff;
        } else if (LCD.Direction == Direction_V) {
            color =
                *(__IO uint32_t *)(LCD.LayerMemoryAdd +
                                   3 * ((LCD_Height - x - 1) * LCD_Width + y)) &
                0x00ffffff;
        }
    }

    else {
        if (LCD.Direction == Direction_H) {
            color = *(__IO uint16_t *)(LCD.LayerMemoryAdd +
                                       2 * (x + y * LCD_Width));
        } else if (LCD.Direction == Direction_V) {
            color =
                *(__IO uint16_t *)(LCD.LayerMemoryAdd +
                                   2 * ((LCD_Height - x - 1) * LCD_Width + y));
        }
    }
    return color;
}

void LCD_DisplayChar(uint16_t x, uint16_t y, uint8_t c)
{
    uint16_t index = 0, counter = 0; // 计数变量
    uint8_t disChar;                 // 存储字符的地址
    uint16_t Xaddress = x;           // 水平坐标

    c = c - 32; // 计算ASCII字符的偏移

    for (index = 0; index < LCD_Fonts->Sizes; index++) {
        disChar =
            LCD_Fonts->pTable[c * LCD_Fonts->Sizes + index]; // 获取字符的模值
        for (counter = 0; counter < 8; counter++) {
            if (disChar & 0x01) {
                LCD_DrawPoint(Xaddress, y,
                              LCD.Color); // 当前模值不为0时，使用画笔色绘点
            } else {
                LCD_DrawPoint(Xaddress, y,
                              LCD.BackColor); // 否则使用背景色绘制点
            }
            disChar >>= 1;
            Xaddress++; // 水平坐标自加

            if ((Xaddress - x) ==
                LCD_Fonts->Width) // 如果水平坐标达到了字符宽度，则退出当前循环
            {                     // 进入下一行的绘制
                Xaddress = x;
                y++;
                break;
            }
        }
    }
}

void LCD_DisplayString(uint16_t x, uint16_t y, char *p)
{
    while ((x < LCD_Width) &&
           (*p != 0)) // 判断显示坐标是否超出显示区域并且字符是否为空字符
    {
        LCD_DisplayChar(x, y, *p);
        x += LCD_Fonts->Width; // 显示下一个字符
        p++;                   // 取下一个字符地址
    }
}

void LCD_SetTextFont(pFONT *fonts)
{
    LCD_CHFonts = fonts;
    switch (fonts->Width) {
        case 12:
            LCD_Fonts = &Font12;
            break;
        case 16:
            LCD_Fonts = &Font16;
            break;
        case 20:
            LCD_Fonts = &Font20;
            break;
        case 24:
            LCD_Fonts = &Font24;
            break;
        case 32:
            LCD_Fonts = &Font32;
            break;
        default:
            break;
    }
}

void LCD_DisplayChinese(uint16_t x, uint16_t y, char *pText)
{
    uint16_t i = 0, index = 0, counter = 0;
    uint16_t addr;
    uint8_t disChar;
    uint16_t Xaddress = x;

    while (1) {

        if (*(LCD_CHFonts->pTable + (i + 1) * LCD_CHFonts->Sizes + 0) ==
                *pText &&
            *(LCD_CHFonts->pTable + (i + 1) * LCD_CHFonts->Sizes + 1) ==
                *(pText + 1)) {
            addr = i;
            break;
        }
        i += 2;

        if (i >= LCD_CHFonts->Table_Rows) break;
    }

    for (index = 0; index < LCD_CHFonts->Sizes; index++) {
        disChar = *(LCD_CHFonts->pTable + (addr)*LCD_CHFonts->Sizes + index);

        for (counter = 0; counter < 8; counter++) {
            if (disChar & 0x01) {
                LCD_DrawPoint(Xaddress, y, LCD.Color);
            } else {
                LCD_DrawPoint(Xaddress, y, LCD.BackColor);
            }
            disChar >>= 1;
            Xaddress++;

            if ((Xaddress - x) == LCD_CHFonts->Width) {
                Xaddress = x;
                y++;
                break;
            }
        }
    }
}

void LCD_DisplayText(uint16_t x, uint16_t y, char *pText)
{

    while (*pText != 0) {
        if (*pText <= 0x7F) {
            LCD_DisplayChar(x, y, *pText);
            x += LCD_Fonts->Width;
            pText++;
        } else {
            LCD_DisplayChinese(x, y, pText);
            x += LCD_CHFonts->Width;
            pText += 2;
        }
    }
}

void LCD_ShowNumMode(uint8_t mode)
{
    LCD.ShowNum_Mode = mode;
}

void LCD_DisplayNumber(uint16_t x, uint16_t y, int32_t number, uint8_t len)
{
    char Number_Buffer[15];

    if (LCD.ShowNum_Mode == Fill_Zero) {
        sprintf(Number_Buffer, "%.*ld", len, number);
    } else {
        sprintf(Number_Buffer, "%*ld", len, number);
    }

    LCD_DisplayString(x, y, (char *)Number_Buffer);
}

void LCD_DisplayDecimals(uint16_t x, uint16_t y, double decimals, uint8_t len,
                         uint8_t decs)
{
    char Number_Buffer[20]; // 用于存储转换后的字符串

    int32_t whole = (int32_t)decimals; // 整数部分

    double decimal_part = decimals - whole;
    if (decimal_part < 0) { decimal_part = -decimal_part; }

    int32_t decimal_int = (int32_t)(decimal_part * pow(10, decs) + 0.5);

    if (decimal_int >= pow(10, decs)) {
        decimal_int = 0;
        whole += (decimals < 0) ? -1 : 1;
    }
    // 格式化输出
    if (LCD.ShowNum_Mode == Fill_Zero) {
        // 多余位填充0模式
        char format[32];
        sprintf(format, "%%0%dld.%%0%dld", len - decs - 1, decs);
        sprintf(Number_Buffer, format, whole, decimal_int);
    } else {
        // 多余位填充空格
        char format[16];
        sprintf(format, "%%%dld.%%0%dld", len - decs - 1, decs);
        sprintf(Number_Buffer, format, whole, decimal_int);
    }

    LCD_DisplayString(x, y, (char *)Number_Buffer);

    /* if (LCD.ShowNum_Mode == Fill_Zero) // 多余位填充0模式
    {
        sprintf(Number_Buffer, "%0*.*lf", len, decs,
                decimals); // 将 number 转换成字符串，便于显示
    } else                 // 多余位填充空格
    {
        sprintf(Number_Buffer, "%*.*lf", len, decs,
                decimals); // 将 number 转换成字符串，便于显示
    }

    LCD_DisplayString(x, y,
                      (char *)Number_Buffer); // 将转换得到的字符串显示出来 */
}

void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                   const uint8_t *pImage)
{
    uint8_t disChar;
    uint16_t Xaddress = x;
    uint16_t i = 0, j = 0, m = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < (float)width / 8; j++) {
            disChar = *pImage;

            for (m = 0; m < 8; m++) {
                if (disChar & 0x01) {
                    LCD_DrawPoint(Xaddress, y, LCD.Color);
                } else {
                    LCD_DrawPoint(Xaddress, y, LCD.BackColor);
                }
                disChar >>= 1;
                Xaddress++;

                if ((Xaddress - x) == width) {
                    Xaddress = x;
                    y++;
                    break;
                }
            }
            pImage++;
        }
    }
}

#define ABS(X) ((X) > 0 ? (X) : -(X))

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1); /* The difference between the x's */
    deltay = ABS(y2 - y1); /* The difference between the y's */
    x      = x1;           /* Start x off at the first pixel */
    y      = y1;           /* Start y off at the first pixel */

    if (x2 >= x1) /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    } else /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    } else /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) /* There is at least one x-value for every y-value */
    {
        xinc1     = 0; /* Don't change the x when numerator >= denominator */
        yinc2     = 0; /* Don't change the y for every iteration */
        den       = deltax;
        num       = deltax / 2;
        numadd    = deltay;
        numpixels = deltax; /* There are more x-values than y-values */
    } else /* There is at least one y-value for every x-value */
    {
        xinc2     = 0; /* Don't change the x for every iteration */
        yinc1     = 0; /* Don't change the y when numerator >= denominator */
        den       = deltay;
        num       = deltay / 2;
        numadd    = deltax;
        numpixels = deltay; /* There are more y-values than x-values */
    }
    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        LCD_DrawPoint(x, y, LCD.Color); /* Draw the current pixel */
        num += numadd;  /* Increase the numerator by the top of the fraction */
        if (num >= den) /* Check if numerator >= denominator */
        {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}

void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    /* draw horizontal lines */
    LCD_DrawLine(x, y, x + width, y);
    LCD_DrawLine(x, y + height, x + width, y + height);

    /* draw vertical lines */
    LCD_DrawLine(x, y, x, y + height);
    LCD_DrawLine(x + width, y, x + width, y + height);
}

void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r)
{
    int Xadd = -r, Yadd = 0, err = 2 - 2 * r, e2;
    do {

        LCD_DrawPoint(x - Xadd, y + Yadd, LCD.Color);
        LCD_DrawPoint(x + Xadd, y + Yadd, LCD.Color);
        LCD_DrawPoint(x + Xadd, y - Yadd, LCD.Color);
        LCD_DrawPoint(x - Xadd, y - Yadd, LCD.Color);

        e2 = err;
        if (e2 <= Yadd) {
            err += ++Yadd * 2 + 1;
            if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
        }
        if (e2 > Xadd) err += ++Xadd * 2 + 1;
    } while (Xadd <= 0);
}

void LCD_DrawEllipse(int x, int y, int r1, int r2)
{
    int Xadd = -r1, Yadd = 0, err = 2 - 2 * r1, e2;
    float K = 0, rad1 = 0, rad2 = 0;

    rad1 = r1;
    rad2 = r2;

    if (r1 > r2) {
        do {
            K = (float)(rad1 / rad2);

            LCD_DrawPoint(x - Xadd, y + (uint16_t)(Yadd / K), LCD.Color);
            LCD_DrawPoint(x + Xadd, y + (uint16_t)(Yadd / K), LCD.Color);
            LCD_DrawPoint(x + Xadd, y - (uint16_t)(Yadd / K), LCD.Color);
            LCD_DrawPoint(x - Xadd, y - (uint16_t)(Yadd / K), LCD.Color);

            e2 = err;
            if (e2 <= Yadd) {
                err += ++Yadd * 2 + 1;
                if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
            }
            if (e2 > Xadd) err += ++Xadd * 2 + 1;
        } while (Xadd <= 0);
    } else {
        Yadd = -r2;
        Xadd = 0;
        do {
            K = (float)(rad2 / rad1);

            LCD_DrawPoint(x - (uint16_t)(Xadd / K), y + Yadd, LCD.Color);
            LCD_DrawPoint(x + (uint16_t)(Xadd / K), y + Yadd, LCD.Color);
            LCD_DrawPoint(x + (uint16_t)(Xadd / K), y - Yadd, LCD.Color);
            LCD_DrawPoint(x - (uint16_t)(Xadd / K), y - Yadd, LCD.Color);

            e2 = err;
            if (e2 <= Xadd) {
                err += ++Xadd * 3 + 1;
                if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
            }
            if (e2 > Yadd) err += ++Yadd * 3 + 1;
        } while (Yadd <= 0);
    }
}

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

    DMA2D->CR &= ~(DMA2D_CR_START); //	ֹͣDMA2D
    DMA2D->CR     = DMA2D_R2M;      //	�Ĵ�����SDRAM
    DMA2D->OPFCCR = LCD.ColorMode;  //	������ɫ��ʽ
    DMA2D->OCOLR  = LCD.Color;      //	��ɫ

    if (LCD.Direction == Direction_H) // �������
    {
        DMA2D->OOR = LCD_Width - width; //	������ƫ��
        DMA2D->OMAR =
            LCD.LayerMemoryAdd + LCD.BytesPerPixel * (LCD_Width * y + x); // ��ַ;
        DMA2D->NLR = (width << 16) | (height); //	�趨���ȺͿ��
    } else                                     // �������
    {
        DMA2D->OOR  = LCD_Width - height; //	������ƫ��
        DMA2D->OMAR = LCD.LayerMemoryAdd +
                      LCD.BytesPerPixel *
                          ((LCD_Height - x - 1 - width) * LCD_Width + y); // ��ַ
        DMA2D->NLR = (width) | (height << 16); //	�趨���ȺͿ��
    }

    DMA2D->CR |= DMA2D_CR_START; //	����DMA2D

    while (DMA2D->CR & DMA2D_CR_START); //	�ȴ��������
}

void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r)
{
    int32_t D;     /* Decision Variable */
    uint32_t CurX; /* Current X Value */
    uint32_t CurY; /* Current Y Value */

    D = 3 - (r << 1);

    CurX = 0;
    CurY = r;

    while (CurX <= CurY) {
        if (CurY > 0) {
            LCD_DrawLine(x - CurX, y - CurY, x - CurX, y - CurY + 2 * CurY);
            LCD_DrawLine(x + CurX, y - CurY, x + CurX, y - CurY + 2 * CurY);
        }

        if (CurX > 0) {
            LCD_DrawLine(x - CurY, y - CurX, x - CurY, y - CurX + 2 * CurX);
            LCD_DrawLine(x + CurY, y - CurX, x + CurY, y - CurX + 2 * CurX);
        }
        if (D < 0) {
            D += (CurX << 2) + 6;
        } else {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }

    LCD_DrawCircle(x, y, r);
}
