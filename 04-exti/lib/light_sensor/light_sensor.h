#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#define Light_Sensor_Pin GPIO_PIN_12
#define Light_Sensor_GPIO_Port GPIOB

void Light_Sensor_Init(void);
uint8_t Light_Sensor_Read(void);

#endif // LIGHT_SENSOR_H