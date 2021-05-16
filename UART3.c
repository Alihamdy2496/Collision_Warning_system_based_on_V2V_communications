# include "UART3.h"
#include "TM4C123GH6PM.h"
extern void UART3_INIT(void)
{
   // float temp;
    SYSCTL_RCGCUART_R|=(1<<3);   // Activate Clock for Uart 3
    SYSCTL_RCGCGPIO_R|=(1<<2);   // Activate Clock for port c
    GPIO_PORTC_AFSEL_R |= 0xC0;          //alternative
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)+0x11000000;
    UART3_CTL_R &= ~UART_CTL_UARTEN;

    UART3_IBRD_R = 43;                     // IBRD = int(80,000,000 / (16 * 9600)) = int(520.83333333)
    UART3_FBRD_R = 26;                        // FBRD = int(0.83333333 * 64 + 0.5) = 53.8333333
                                          // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART3_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
    UART3_CTL_R |= UART_CTL_UARTEN;
    GPIO_PORTC_DEN_R |= 0xC0;
    GPIO_PORTC_AMSEL_R &= ~0xC0;
}
extern void UART3_sendByte(char c)
 {
   //UART0_ECR_R=0X00;
   while((UART3_FR_R&UART_FR_TXFF)!=0); //
   UART3_DR_R=c;

}

extern uint8 UART3_recieveByte(void)
{
   uint8 c;
   while((UART3_FR_R&UART_FR_RXFE)!=0);
   c=(char)(UART3_DR_R&0xff);
   return c;

}

void UART3_sendString(const uint8 *Str)
{
   uint8 i = 0;
   while(Str[i] != '\0')
      {
         UART3_sendByte(Str[i]);
         i++;
       }
}

void UART3_receiveString(uint8 *Str)
{
    uint8 i = 0;
    Str[i] = UART3_recieveByte();
    while(Str[i] != '\n')
    {
      i++;
      Str[i] = UART3_recieveByte();
    }
    Str[i] = '\0';
}



