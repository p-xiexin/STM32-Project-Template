#ifndef TRIGGER_H
#define TRIGGER_H
#include "main.h"

#define Fric_UP 1400
#define Fric_DOWN 1300
#define Fric_OFF 1000

void trigger_PWM_Init(void);
void fric_off(void);
void fric1_on(uint16_t cmd);
void fric2_on(uint16_t cmd);

#endif
