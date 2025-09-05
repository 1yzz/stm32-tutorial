#include "stm32f1xx_hal.h"
#include "button.h"
#include "led.h"

void EXTI15_10_IRQHandler(void);

int main(void) {
    HAL_Init();
    Button_Init();
    LED_Init();

    while (1) {
        // Main loop - interrupt handling happens in ISR
    }
}

void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        LED_Toggle();
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}