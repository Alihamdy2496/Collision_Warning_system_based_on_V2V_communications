/*
 * Speed.h
 *
 *  Created on: Feb 28, 2019
 *      Author: hp
 */

#ifndef SPEED_H_
#define SPEED_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include"TM4C123GH6PM.h"


#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"


void PortFIntHandler(void);

void Configure_Speed_pin(void(*func)(void));

void Enable_Speed_Interrupt(void);

void Disable_Speed_Interrupt(void);

void TimerA1_Handler(void);



#endif /* SPEED_H_ */
