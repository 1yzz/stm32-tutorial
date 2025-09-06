#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"

// OLED I2C address (SSD1315: 0x78=0x3C<<1)
#define OLED_I2C_ADDR         0x78 

// OLED GPIO and Pin definitions
#define OLED_RST_PORT         GPIOB
#define OLED_RST_PIN          GPIO_PIN_0


// OLED display size
#define OLED_WIDTH            128
#define OLED_HEIGHT           64

// Function prototypes
void OLED_System_Init(void);  
void OLED_Init(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Write_Command(uint8_t cmd);
void OLED_Write_Data(uint8_t data);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);

#endif /* __OLED_H */
