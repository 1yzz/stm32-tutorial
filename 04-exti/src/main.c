#include "stm32f1xx_hal.h"
#include "button.h"
#include "led.h"
#include "light_sensor.h"

void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

int main(void) {
    HAL_Init();
    Button_EXTI_Init();
    Light_Sensor_EXTI_Init();
    LED_Init();

    while (1) {
        // Main loop - interrupt handling happens in ISR
    }
}

void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}

void EXTI15_10_IRQHandler(void) {
    // 检查具体是哪个引脚触发了中断
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    }
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_PIN) {
        // 防抖检查：只有通过防抖检查才执行LED切换
        if (Button_Debounce_Check()) {
            LED_Toggle();
        }
    } else if (GPIO_Pin == Light_Sensor_Pin) {
         LED_Toggle();
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}