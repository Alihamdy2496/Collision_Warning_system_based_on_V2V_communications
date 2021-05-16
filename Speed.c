#include"Speed.h"
#include"UART.h"
extern unsigned short Counts;
extern unsigned short Car_Speed;

static void (*handler)(void);
void PortFIntHandler(void)
{
    handler();
}

void Configure_Speed_pin(void(*func)(void))
{
    handler=func;
    volatile unsigned long delay;

    /*Enable clock for port F*/
    SYSCTL_RCGCGPIO_R|=(1<<5);

    /*Allow time for clock to stabilize*/
    delay = SYSCTL_RCGC2_R;


    /*Configure PF1 as GPIO*/
    GPIO_PORTF_PCTL_R &=~(1<<1);

    /*Set 0 to make PF1 input*/
    GPIO_PORTF_DIR_R &=(~(1<<1));

    /*Set 0 to disable alt funct on PF1*/
    GPIO_PORTF_AFSEL_R &=(~(1<<1));

    /* Pull down res enable*/
    GPIO_PORTF_PDR_R|=(1<<1);

    /* Enable digital I/O*/
    GPIO_PORTF_DEN_R |=(1<<1);

    /*Enable Interrupt */
    GPIO_PORTF_IM_R|=(1<<1);

    /* Edge*/
    GPIO_PORTF_IS_R&=(~(1<<1));

    /* Single Edge*/
    GPIO_PORTF_IBE_R &= (~(1<<1));

    /*Rising Edge*/
    GPIO_PORTF_IEV_R |= (1<<1);

}

void Enable_Speed_Interrupt(void)
{

    NVIC_EN0_R = 1 <<((30) & 31);

}



void Disable_Speed_Interrupt(void)
{

    NVIC_EN0_R &=~(1 <<((30) & 31));

}
