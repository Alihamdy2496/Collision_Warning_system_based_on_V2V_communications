/*
 * PWM.h
 *
 *  Created on: Nov 30, 2018
 *      Author: user
 */

#ifndef PWM_H_
#define PWM_H_
#include "TM4C123GH6PM.h"
#include "std_types.h"

//void PWM_Timer0_Init(uint16 period);
//void Set_PWM_duty_cyle(unsigned char set_duty_cycle);

//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
//void Timer0A_PWM_Init(u32 period, u32 high);
//Timer0A PWM initialized
//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
//void Timer0A_PWM_Duty(u32 high);

void Timer0B_PWM_Init(u32 period, u32 high);

void Timer0B_PWM_Duty(u32 high);

void Timer0A_PWM_Init(u32 period, u32 high);

void Timer0A_PWM_Duty(u32 high);


#endif /* PWM_H_ */
