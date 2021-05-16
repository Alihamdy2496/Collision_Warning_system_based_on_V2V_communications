/*
 * Ultrasoinc_2.h
 *
 *  Created on: Feb 22, 2019
 *      Author: hp
 */
extern unsigned long pulse;

unsigned short int Trigger_pin;

extern unsigned short Echo;

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"

#include <string.h>
#include"TM4C123GH6PM.h"



#ifndef ULTRASOINC_2_H_
#define ULTRASOINC_2_H_

/*Timer2 Configuration*/
void Captureinit(void);

/* Interrupt Handler*/
void inputInt(void);

void configure_callback(void(*func)(void));


/* Configure Echo Input, Interrupts*/
void Configure_Echo(void);

/*Configure Trigger Pin Output*/
void Configure_Trigger(void);


/*Enable Interrupts*/
void Enable_Ultrasionc_Interrupts(void);


/*Disable Interrupts*/
void Disable_Ultrasionc_Interrupts(void);

/* Return Ultrasoinc Reading*/
unsigned int Ultrasonic_Reading(unsigned short int);

/*Send Trigger high 10 us*/
void Send_Trigger(unsigned short int);


void Delay_us(unsigned short);


//void Delay_ms (unsigned short);

#endif /* ULTRASOINC_2_H_ */
