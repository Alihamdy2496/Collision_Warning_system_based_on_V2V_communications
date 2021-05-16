//=========================================================================
//  PROJECT     : UART7.c
//  Created on  : 25 nov 2018
//  Author      : GP team
//  Description : uart7 of tiva c
//====================s=====================================================
#include "TM4C123GH6PM.h"
#include <stdint.h>
#include <UART7.h>
void uart7_init(void)
{
    SYSCTL_RCGCUART_R|=(1<<7);
    SYSCTL_RCGCGPIO_R|=(1<<4);
    UART7_CTL_R &= ~(UART_CTL_UARTEN|UART_CTL_LBE);
    UART7_IBRD_R = 43;             // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.40277778)
    UART7_FBRD_R =26;                        // FBRD = int(0.40277778 * 64 + 0.5) = 26

    UART7_LCRH_R=0X70; //_8-bit, no parity, 1-stop bit, FIFO
    UART7_CTL_R |= (UART_CTL_UARTEN|UART_CTL_RXE|UART_CTL_TXE);
    GPIO_PORTE_AFSEL_R |= 0x03;
    GPIO_PORTE_DEN_R |= 0x03;
    GPIO_PORTE_PCTL_R = 0x00000011;
}
void uart7_interrupt_enable(void)
{
    UART7_IM_R|=(1<<4);
    NVIC_EN1_R|=(1<<31);
}
void uart7_interrupt_disable(void)
{
    UART7_IM_R&=~(1<<4);
    NVIC_EN1_R&=~(1<<31);
}
unsigned char UART7_InChar(void)
{
	while((UART7_FR_R&UART_FR_RXFE) != 0 );

	return((char)(UART7_DR_R&0x00FF));
}
void UART7_OutChar(unsigned char data)
{
  while((UART7_FR_R&UART_FR_TXFF) != 0);
  UART7_DR_R = data;
}
//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART7_OutString(char *pt)
{
        while(*pt)
            {
            UART7_OutChar(*pt);
            pt++;
            }
}
