/*
 * Timer1.h
 *
 *  Created on: Dec 5, 2018
 *      Author: user
 */

#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"
#include "TM4C123GH6PM.h"

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer1A_Init(unsigned long period,void (*func)(void));

#endif /* TIMER1_H_ */
