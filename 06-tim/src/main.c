#include "stm32f1xx_hal.h"
#include "led.h"
#include "timer.h"

uint8_t count = 0;

int main(void) {
    HAL_Init();
    LED_Init();

    TIMER_Init();
    TIMER_Start();

    while (1) {
    }
}

// Timer interrupt callback function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        LED_Toggle();  // Toggle LED every timer period
        count++;
        // OLED_ShowNum(0, 16, count, 3, 16);
    }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
