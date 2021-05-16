//=========================================================================
//  PROJECT     : UART7.h
//  Created on  : 25 nov 2018
//  Author      : GP team
//  Description : uart7 of tiva c
//====================s=====================================================
#ifndef UART7_H_
#define UART7_H_

#include "standard_types.h"
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

void uart7_init(void);
unsigned char UART7_InChar(void);
void uart7_interrupt_enable(void);
void uart7_interrupt_disable(void);
void UART7_OutChar(unsigned char data);
void UART7_OutString(char *pt);

#endif /* UART7_H_ */
