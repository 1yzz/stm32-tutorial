#include "stm32f1xx_hal.h"
#include "button.h"
#include "led.h"

void EXTI0_IRQHandler(void);

int main(void) {
    HAL_Init();
    Button_EXTI_Init();
    LED_Init();

    while (1) {
        // Main loop - interrupt handling happens in ISR
    }
}

void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        // 防抖检查：只有通过防抖检查才执行LED切换
        if (Button_Debounce_Check()) {
            LED_Toggle();
        }
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}