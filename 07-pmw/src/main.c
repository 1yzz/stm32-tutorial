#include "stm32f1xx_hal.h"
#include "pwm_led.h"

int main(void) {
    // Initialize HAL
    HAL_Init();
    
    // Initialize PWM LED on PA0 (高电平触发)
    PWM_LED_Init();
    
    // 使用封装的呼吸灯方法
    while (1) {
        // 非阻塞呼吸灯效果
        PWM_LED_Breathing();
        
        // 可以在这里添加其他任务
        // 呼吸灯会自动运行
    }
}

// SysTick interrupt handler
void SysTick_Handler(void) {
    HAL_IncTick();
}