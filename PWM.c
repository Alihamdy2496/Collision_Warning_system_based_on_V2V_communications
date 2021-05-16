/*
 * PWM.c
 *
 *  Created on: Nov 30, 2018
 *      Author: atrby
 */

#include "PWM.h"
/*
//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
void Timer0A_PWM_Init(u32 period, u32 high){
  volatile unsigned long delay;
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  SYSCTL_RCGCGPIO_R |= 0x0002;     // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
  GPIO_PORTB_AFSEL_R |= 0x40;      // enable alt funct on PB6
  GPIO_PORTB_DEN_R |= 0x40;        // enable digital I/O on PB6
                                   // configure PB6 as T0CCP0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xF0FFFFFF)+0x07000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;     // disable analog functionality on PB6
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
                                   // configure for alternate (PWM) mode
  TIMER0_TAMR_R = (TIMER_TAMR_TAAMS|TIMER_TAMR_TAMR_PERIOD);
  TIMER0_TAILR_R = period-1;       // timer start value
  TIMER0_TAMATCHR_R = period-high-1; // duty cycle = high/period
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, PWM
}

//Timer0A PWM initialized
//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
void Timer0A_PWM_Duty(u32 high)
{
  TIMER0_TAMATCHR_R = TIMER0_TAILR_R-high; // duty cycle = high/period
}

*/


void Timer0B_PWM_Init(u32 period, u32 high)
{
  volatile unsigned long delay;
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  SYSCTL_RCGCGPIO_R |= 0x0002;     // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
  GPIO_PORTB_AFSEL_R |= (1<<7);      // enable alt funct on PB7
  GPIO_PORTB_DEN_R |= (1<<7);;       // enable digital I/O on PB7

                                   // configure PB7 as T0CCP1

  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0x0FFFFFFF)+0x70000000;

  GPIO_PORTB_AMSEL_R &= ~(1<<7);     // disable analog functionality on PB6

  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable TimerA during setup

  TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode

                                   // configure for alternate (PWM) mode
  TIMER0_TBMR_R = (TIMER_TAMR_TAAMS|TIMER_TAMR_TAMR_PERIOD);
  TIMER0_TBILR_R = period-1;       // timer start value
  TIMER0_TBMATCHR_R = period-high-1; // duty cycle = high/period
  TIMER0_CTL_R |= TIMER_CTL_TBEN;  // enable TimerA 16-b, PWM
}

//Timer0A PWM initialized
//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
void Timer0B_PWM_Duty(u32 high)

{
  TIMER0_TBMATCHR_R = TIMER0_TBILR_R-high; // duty cycle = high/period
}


void Timer0A_PWM_Init(u32 period, u32 high){
  volatile unsigned long delay;
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  SYSCTL_RCGCGPIO_R |= 0x0002;     // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?
  GPIO_PORTB_AFSEL_R |= 0x40;      // enable alt funct on PB6
  GPIO_PORTB_DEN_R |= 0x40;        // enable digital I/O on PB6
                                   // configure PB6 as T0CCP0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xF0FFFFFF)+0x07000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;     // disable analog functionality on PB6
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
                                   // configure for alternate (PWM) mode
  TIMER0_TAMR_R = (TIMER_TAMR_TAAMS|TIMER_TAMR_TAMR_PERIOD);
  TIMER0_TAILR_R = period-1;       // timer start value
  TIMER0_TAMATCHR_R = period-high-1; // duty cycle = high/period
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, PWM
}


//Timer0A PWM initialized
//  pulse frequency = 16,000,000/period (Hz)
//  pulse duty cycle = high/period
void Timer0A_PWM_Duty(u32 high)
{
  TIMER0_TAMATCHR_R = TIMER0_TAILR_R-high; // duty cycle = high/period
}


