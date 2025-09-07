#include "stm32f1xx_hal.h"
#define LED_PIN GPIO_PIN_10
#define LED_GPIO_PORT GPIOB

void LED_Init(void);
void LED_On(void); 
void LED_Off(void);
void LED_Toggle(void);