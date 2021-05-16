/*
 * PLL.c
 *
 *  Created on: Apr 17, 2019
 *      Author: user
 */
#include "PLL.h"

void PLL_Init(void){
  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;   // clear XTAL field
  SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;// configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;// clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;// configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;  // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) // clear system clock divider field
                  + (SYSDIV2<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){};
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

//change uarts to
/*
    //Initialize the UART for 115,200 baud rate (assuming 80 MHz UART clock),
    UART7_IBRD_R = 43;             // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.40277778)
    UART7_FBRD_R =26                        // FBRD = int(0.40277778 * 64 + 0.5) = 26

    //Initialize the UARTs for 9600 baud rate (assuming 80 MHz UART clock),

    UART0_IBRD_R = 520;                     // IBRD = int(80,000,000 / (16 * 9600)) = int(520.83333333)
    UART0_FBRD_R =53                        // FBRD = int(0.83333333 * 64 + 0.5) = 53.8333333

    UART2_IBRD_R = 520;                     // IBRD = int(80,000,000 / (16 * 9600)) = int(520.83333333)
    UART2_FBRD_R =53                        // FBRD = int(0.83333333 * 64 + 0.5) = 53.8333333


    UART3_IBRD_R = 520;                     // IBRD = int(80,000,000 / (16 * 9600)) = int(520.83333333)
    UART3_FBRD_R =53                        // FBRD = int(0.83333333 * 64 + 0.5) = 53.8333333



*/

