#ifndef __pwm_h_
#define __pwm_h_
#include "sys.h"

void TIM12_PWM_Init(u32 arr,u32 psc);
void Arm_Angle(double DU);
void Hand_Angle(double DU);
void AngleCtrl(void);
#endif


