#ifndef BUZZER_H
#define BUZZER_H

#define Buzzer_Pin GPIO_PIN_12
#define Buzzer_GPIO_Port GPIOB

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);
void Buzzer_Beep(void);
void Buzzer_Beep_Short(void);
void Buzzer_Beep_Long(void);
void Buzzer_Beep_Short_Long(void);
void Buzzer_Beep_Long_Short(void);

#endif // BUZZER_H