#include "stm32f1xx_hal.h"
#include "led.h"
#include "button.h"

int main(void) {
    HAL_Init(); // Initialize the HAL Library

    LED_Init(); // Initialize the LED

    Button_Init();

    while (1)
    {
      if (Button_Pressed()) {
        LED_Toggle();
      }
    }
    
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}