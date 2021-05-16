//=========================================================================
//  PROJECT     : UART2.h
//  Created on  : 25 nov 2018
//  Author      : GP team
//  Description : uart2 of tiva c
//====================s=====================================================
#ifndef UART2_H_
#define UART2_H_

#include "standard_types.h"
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

void uart2_init(void (*func)(void));
unsigned char UART2_InChar(void);
void UART2_OutChar(unsigned char data);
void UART2_OutString(char *pt);
void uart2_interrupt_enable(void);
void uart2_interrupt_disable(void);

#endif /* UART2_H_ */
