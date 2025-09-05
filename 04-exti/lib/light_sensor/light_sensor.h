#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#define Light_Sensor_Pin GPIO_PIN_12
#define Light_Sensor_GPIO_Port GPIOB
#define Light_Sensor_EXTI_IRQn EXTI15_10_IRQn

// 光传感器防抖设置
#define LIGHT_SENSOR_DEBOUNCE_TIME_MS 30  // 防抖时间30ms

void Light_Sensor_Init(void);
void Light_Sensor_EXTI_Init(void);
uint8_t Light_Sensor_Read(void);

#endif // LIGHT_SENSOR_H    