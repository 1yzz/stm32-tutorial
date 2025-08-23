#define Button_Pin GPIO_PIN_0
#define Button_GPIO_Port GPIOA

void Button_Init(void);
uint8_t Button_Pressed(void);