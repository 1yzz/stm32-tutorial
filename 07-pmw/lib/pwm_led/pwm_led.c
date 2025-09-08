#include "pwm_led.h"
#include "stm32f1xx_hal.h"

static TIM_HandleTypeDef htim2;
static uint8_t current_duty_cycle = 0;
static uint8_t led_state = 0; // 0 = off, 1 = on

void PWM_LED_Init(void) {
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    // Configure PA0 as alternate function (TIM2_CH1)
    // PA0默认就是TIM2_CH1，不需要重映射
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PWM_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Push-pull for PWM output
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PWM_LED_GPIO_PORT, &GPIO_InitStruct);
    
    // Configure TIM2 for PWM
    // For STM32F103: System clock is 72MHz, APB1 is 36MHz, TIM2 gets 72MHz
    uint32_t timer_freq = 72000000; // 72MHz
    uint32_t prescaler = (timer_freq / (PWM_FREQUENCY * PWM_RESOLUTION)) - 1;
    uint32_t period = PWM_RESOLUTION - 1;
    
    // Debug: prescaler = (72000000 / (1000 * 1000)) - 1 = 71
    // This should give us 1kHz PWM with 1000 steps (0-100%)
    // 实际频率 = 72MHz / (71+1) / (1000) = 1kHz
    
    htim2.Instance = PWM_LED_TIMER;
    htim2.Init.Prescaler = prescaler;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = period;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        // Error handling
        while(1);
    }
    
    // Configure PWM channel
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0; // Start with 0% duty cycle
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, PWM_LED_CHANNEL) != HAL_OK) {
        // Error handling
        while(1);
    }
    
    // Start PWM
    HAL_TIM_PWM_Start(&htim2, PWM_LED_CHANNEL);
}

void PWM_LED_SetDutyCycle(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        duty_cycle = 100;
    }
    
    // Convert percentage (0-100) to PWM value (0-1000)
    uint32_t pwm_value = (duty_cycle * PWM_RESOLUTION) / 100;
    
    current_duty_cycle = duty_cycle;
    __HAL_TIM_SET_COMPARE(&htim2, PWM_LED_CHANNEL, pwm_value);
}

void PWM_LED_On(void) {
    PWM_LED_SetDutyCycle(100); // 100% duty cycle
    led_state = 1;
}

void PWM_LED_Off(void) {
    PWM_LED_SetDutyCycle(0); // 0% duty cycle
    led_state = 0;
}

void PWM_LED_Toggle(void) {
    if (led_state) {
        PWM_LED_Off();
    } else {
        PWM_LED_On();
    }
}

void PWM_LED_FadeIn(void) {
    for (uint8_t i = 0; i <= 100; i++) {
        PWM_LED_SetDutyCycle(i);
        HAL_Delay(10); // 10ms delay for smooth fade
    }
    led_state = 1;
}

void PWM_LED_FadeOut(void) {
    for (uint8_t i = 100; i > 0; i--) {
        PWM_LED_SetDutyCycle(i);
        HAL_Delay(10); // 10ms delay for smooth fade
    }
    PWM_LED_SetDutyCycle(0);
    led_state = 0;
}

// 设置亮度 (0-100%)
void PWM_LED_SetBrightness(uint8_t brightness) {
    PWM_LED_SetDutyCycle(brightness);
}

// 获取当前占空比
uint8_t PWM_LED_GetCurrentDutyCycle(void) {
    return current_duty_cycle;
}

// 呼吸灯效果 (非阻塞) - 需要定期调用
static uint8_t breathing_brightness = 0;
static uint8_t breathing_direction = 0; // 0 = 增加, 1 = 减少
static uint32_t breathing_last_update = 0;

void PWM_LED_Breathing(void) {
    uint32_t current_time = HAL_GetTick();
    
    // 每20ms更新一次亮度
    if (current_time - breathing_last_update >= 20) {
        // 更新亮度
        if (breathing_direction == 0) {
            breathing_brightness++;
            if (breathing_brightness >= 100) {
                breathing_direction = 1; // 开始减少亮度
            }
        } else {
            breathing_brightness--;
            if (breathing_brightness <= 0) {
                breathing_direction = 0; // 开始增加亮度
            }
        }
        
        // 设置PWM占空比
        PWM_LED_SetDutyCycle(breathing_brightness);
        
        breathing_last_update = current_time;
    }
}

// 呼吸灯效果 (阻塞) - 完成一个呼吸周期
void PWM_LED_BreathingBlocking(uint16_t cycle_time_ms) {
    uint16_t steps = cycle_time_ms / 20; // 每20ms一步
    uint8_t brightness = 0;
    uint8_t direction = 0;
    
    for (uint16_t i = 0; i < steps; i++) {
        // 更新亮度
        if (direction == 0) {
            brightness = (i * 200) / steps; // 0到100
            if (brightness >= 100) {
                brightness = 100;
                direction = 1;
            }
        } else {
            brightness = 100 - ((i - steps/2) * 200) / steps; // 100到0
            if (brightness <= 0) {
                brightness = 0;
            }
        }
        
        // 设置PWM占空比
        PWM_LED_SetDutyCycle(brightness);
        
        HAL_Delay(20);
    }
}

