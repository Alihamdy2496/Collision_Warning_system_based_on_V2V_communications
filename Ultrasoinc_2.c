#include"Ultrasoinc_2.h"
#include"TM4C123GH6PM.h"
#include"lcd_8mode1.h"

//#define TIMER2_TAV_R  (*((volatile unsigned long *)0x40032050))

unsigned long pulse=0;

unsigned short int Trigger_pin;

unsigned short Echo=0;

void Delay_us (unsigned short Time_ms)
{
    unsigned long i;
    unsigned short j;
    for (i=Time_ms;i>0;i--)
            for(j=3;j>0;j--)
                {;}
}
/*
void Delay_ms (unsigned short Time_ms)
{
    unsigned long i;
    unsigned short j;
    for (i=Time_ms;i>0;i--)
            for(j=3180;j>0;j--)
                {;}
}
*/
void Configure_Trigger(void)
{

GPIO_PORTA_DIR_R|=(1<<3);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<3);
GPIO_PORTA_DEN_R|=(1<<3);

GPIO_PORTA_DIR_R|=(1<<2);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<2);
GPIO_PORTA_DEN_R|=(1<<2);

GPIO_PORTA_DIR_R|=(1<<4);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<4);
GPIO_PORTA_DEN_R|=(1<<4);

GPIO_PORTA_DIR_R|=(1<<5);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<5);
GPIO_PORTA_DEN_R|=(1<<5);

GPIO_PORTA_DIR_R|=(1<<6);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<6);
GPIO_PORTA_DEN_R|=(1<<6);

GPIO_PORTA_DIR_R|=(1<<7);
// Disable alternate Functions
GPIO_PORTA_AFSEL_R&=~(1<<7);
GPIO_PORTA_DEN_R|=(1<<7);
}
static void (*handler)(void);
void configure_callback(void(*func)(void)){
    handler=func;
}
void inputInt(void)
{
    handler();
}

void Captureinit()
{
   // Timer 2 Configuration

    SYSCTL_RCGCTIMER_R |= 0x04;   /* activate TIMER2 */
    TIMER2_CTL_R = 0x00000000;    /* disable TIMER2 during setup */
    TIMER2_CFG_R = 0x00000000;    /* configure for 32-bit mode */
    TIMER2_TAMR_R = 0x00000002;   /* configure for One Shot mode*/
    TIMER2_TAMR_R |= 0x00000010;  /* Count Up*/
    TIMER2_TAPR_R = 0;            /* bus clock resolution*/
    //TIMER2_ICR_R |=(1<<0);    //  clear TIMER2A timeout flag
    //TIMER2_IMR_R |=(1<<0);    //  Arm timeout interrupt
    //NVIC_EN0_R = 1<<23;       //  Enable Timer 2A Interrupts
    TIMER2_CTL_R = 0x00000001;    //enable TIMER2A

}


void Configure_Echo(void)
 {
 //Configure Echo pin 2B Input

volatile unsigned long delay;

SYSCTL_RCGC2_R |= 0x02;          // activate Port B
delay = SYSCTL_RCGC2_R;          // allow time for clock to stabilize


//SYSCTL_RCGC2_R |= (1<<3);          // activate Port B
//delay = SYSCTL_RCGC2_R;          // allow time for clock to stabilize


GPIO_PORTB_AMSEL_R &=(~(1<<2));  // disable analog functionality on PB2
GPIO_PORTB_AMSEL_R &=(~(1<<3));  // disable analog functionality on PB2
GPIO_PORTB_AMSEL_R &=(~(1<<4));  // disable analog functionality on PB2
GPIO_PORTB_AMSEL_R &=(~(1<<5));  // disable analog functionality on PB2

GPIO_PORTB_AMSEL_R &=(~(1<<0));  // disable analog functionality on PB2
GPIO_PORTB_AMSEL_R &=(~(1<<1));  // disable analog functionality on PB2

//GPIO_PORTD_AMSEL_R &=(~(1<<0));  // disable analog functionality on PB2
//GPIO_PORTD_AMSEL_R &=(~(1<<1));  // disable analog functionality on PB2

GPIO_PORTB_PCTL_R &=~(1<<2);     //  configure PB2 as GPIO
GPIO_PORTB_PCTL_R &=~(1<<3);     //  configure PB2 as GPIO
GPIO_PORTB_PCTL_R &=~(1<<4);     //  configure PB2 as GPIO
GPIO_PORTB_PCTL_R &=~(1<<5);     //  configure PB2 as GPIO

GPIO_PORTB_PCTL_R &=~(1<<0);     //  configure PB2 as GPIO
GPIO_PORTB_PCTL_R &=~(1<<1);     //  configure PB2 as GPIO

//GPIO_PORTD_PCTL_R &=~(1<<0);     //  configure PB2 as GPIO
//GPIO_PORTD_PCTL_R &=~(1<<1);     //  configure PB2 as GPIO

GPIO_PORTB_DIR_R &=(~(1<<2));    //  set 0 to make PB2 input
GPIO_PORTB_DIR_R &=(~(1<<3));    //  set 0 to make PB2 input
GPIO_PORTB_DIR_R &=(~(1<<4));    //  set 0 to make PB2 input
GPIO_PORTB_DIR_R &=(~(1<<5));    //  set 0 to make PB2 input

GPIO_PORTB_DIR_R &=(~(1<<0));    //  set 0 to make PB2 input
GPIO_PORTB_DIR_R &=(~(1<<1));    //  set 0 to make PB2 input

//GPIO_PORTD_DIR_R &=(~(1<<0));    //  set 0 to make PB2 input
//GPIO_PORTD_DIR_R &=(~(1<<1));    //  set 0 to make PB2 input

GPIO_PORTB_AFSEL_R &=(~(1<<2));  //  set 0 to disable alt funct on PB2
GPIO_PORTB_AFSEL_R &=(~(1<<3));  //  set 0 to disable alt funct on PB2
GPIO_PORTB_AFSEL_R &=(~(1<<4));  //  set 0 to disable alt funct on PB2
GPIO_PORTB_AFSEL_R &=(~(1<<5));  //  set 0 to disable alt funct on PB2

GPIO_PORTB_AFSEL_R &=(~(1<<0));  //  set 0 to disable alt funct on PB2
GPIO_PORTB_AFSEL_R &=(~(1<<1));  //  set 0 to disable alt funct on PB2

//GPIO_PORTD_AFSEL_R &=(~(1<<0));  //  set 0 to disable alt funct on PB2
//GPIO_PORTD_AFSEL_R &=(~(1<<1));  //  set 0 to disable alt funct on PB2

GPIO_PORTB_PDR_R|=(1<<2);        //  pull down res enable
GPIO_PORTB_PDR_R|=(1<<3);        //  pull down res enable
GPIO_PORTB_PDR_R|=(1<<4);        //  pull down res enable
GPIO_PORTB_PDR_R|=(1<<5);        //  pull down res enable

GPIO_PORTB_PDR_R|=(1<<0);        //  pull down res enable
GPIO_PORTB_PDR_R|=(1<<1);        //  pull down res enable


//GPIO_PORTD_PDR_R|=(1<<0);        //  pull down res enable
//GPIO_PORTD_PDR_R|=(1<<1);        //  pull down res enable

GPIO_PORTB_DEN_R |=(1<<2);       // enable digital I/O
GPIO_PORTB_DEN_R |=(1<<3);       // enable digital I/O
GPIO_PORTB_DEN_R |=(1<<4);       // enable digital I/O
GPIO_PORTB_DEN_R |=(1<<5);       // enable digital I/O

GPIO_PORTB_DEN_R |=(1<<0);       // enable digital I/O
GPIO_PORTB_DEN_R |=(1<<1);       // enable digital I/O



//GPIO_PORTD_DEN_R |=(1<<0);       // enable digital I/O
//GPIO_PORTD_DEN_R |=(1<<1);       // enable digital I/O



//GPIO_PORTB_IM_R|=(1<<2);  /*Enable Interrupt
//GPIO_PORTB_IM_R|=(1<<3);  /*Enable Interrupt


GPIO_PORTB_IS_R&=(~(1<<2));    /* Edge*/
GPIO_PORTB_IS_R&=(~(1<<3));    /* Edge*/
GPIO_PORTB_IS_R&=(~(1<<4));    /* Edge*/
GPIO_PORTB_IS_R&=(~(1<<5));    /* Edge*/

GPIO_PORTB_IS_R&=(~(1<<0));
GPIO_PORTB_IS_R&=(~(1<<1));


GPIO_PORTB_IBE_R|=((1<<2));   /*Both Edges*/
GPIO_PORTB_IBE_R|=((1<<3));   /*Both Edges*/
GPIO_PORTB_IBE_R|=((1<<4));   /*Both Edges*/
GPIO_PORTB_IBE_R|=((1<<5));  // Both Edges

GPIO_PORTB_IBE_R|=((1<<0));  // Both Edges
GPIO_PORTB_IBE_R|=((1<<1));  // Both Edges

}

void Enable_Ultrasionc_Interrupts(void)
{
    NVIC_EN0_R = 1 << (1 & 31);
   // NVIC_EN0_R = 1 << (3 & 31);
}

void Disable_Ultrasionc_Interrupts(void)
{
    NVIC_EN0_R &= ~(1 << (1 & 31));
}


 void Send_Trigger(unsigned short int Trigger_pin)

 {
     if(Trigger_pin ==1)
     {
          GPIO_PORTA_DATA_R|=(1<<2);
          Delay_us(10);
          GPIO_PORTA_DATA_R&=~(1<<2);
     }

     else  if(Trigger_pin ==2)
     {
          GPIO_PORTA_DATA_R|=(1<<3);
          Delay_us(10);
          GPIO_PORTA_DATA_R&=~(1<<3);
     }
     else if(Trigger_pin ==3)
     {
         GPIO_PORTA_DATA_R|=(1<<4);
         Delay_us(10);
         GPIO_PORTA_DATA_R&=~(1<<4);

     }
     else if(Trigger_pin ==4)
       {
         GPIO_PORTA_DATA_R|=(1<<5);
         Delay_us(10);
         GPIO_PORTA_DATA_R&=~(1<<5);
        }

     else if(Trigger_pin ==5)
        {
         GPIO_PORTA_DATA_R|=(1<<6);
         Delay_us(10);
         GPIO_PORTA_DATA_R&=~(1<<6);
        }
     else if(Trigger_pin ==6)
     {
        GPIO_PORTA_DATA_R|=(1<<7);
        Delay_us(10);
        GPIO_PORTA_DATA_R&=~(1<<7);
      }

 }


void TimerA2_Handler(void)
   {
       TIMER2_ICR_R = TIMER_ICR_TATOCINT;
       //TIMER2_TAV_R =0;
   }


