#include "TM4C123GH6PM.h"
#include <stdint.h>
#include "uart2.h"
static void (*handler)(void);
void UART2_interrupt_enable(void)
{
    UART2_IM_R|=(1<<4);
    NVIC_EN1_R|=(1<<1);
}
void UART2_interrupt_disable(void)
{
    UART2_IM_R&=~(1<<4);
    NVIC_EN1_R&=~(1<<1);
}
void UART2_Handler(void)
{
    handler();
}
void uart2_init(void (*func)(void))
{
    handler=func;
    SYSCTL_RCGCUART_R|=(1<<2);  // uart2

    SYSCTL_RCGCGPIO_R|=(1<<3);  // Port D

    UART2_CTL_R &= ~(UART_CTL_UARTEN|UART_CTL_LBE);

 //   UART2_IBRD_R = 104;  /**/

 //   UART2_FBRD_R = 11; /**/
    /*Frequency 80*/

    UART2_IBRD_R = 520;                     // IBRD = int(80,000,000 / (16 * 9600)) = int(520.83333333)
    UART2_FBRD_R = 53;                        // FBRD = int(0.83333333 * 64 + 0.5) = 53.8333333

/************************/

    UART2_LCRH_R=0X70; //_8-bit, no parity, 1-stop bit, FIFO /**/

    UART2_CTL_R |= (UART_CTL_UARTEN|UART_CTL_RXE|UART_CTL_TXE);

    GPIO_PORTD_AFSEL_R |= 0xc0;

    GPIO_PORTD_DEN_R |= 0xc0;

    GPIO_PORTD_PCTL_R |= 0x11000000;


    UART2_IFLS_R = 0x00; // UART Interrupt FIFO Level Select, FIFO level at which we get interrupt

    UART2_interrupt_enable();
}
unsigned char UART2_InChar(void)
{
    while((UART2_FR_R&UART_FR_RXFE) != 0);
    return((char)(UART2_DR_R&0x00FF));
}

void UART2_OutChar(unsigned char data)
{
  while((UART2_FR_R&UART_FR_TXFF) != 0);
  UART2_DR_R = data;
}
//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART2_OutString(char *pt)
{
        while(*pt)
            {
            UART2_OutChar(*pt);
            pt++;
            }

}
