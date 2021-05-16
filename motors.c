#include"motors.h"
#include "TM4C123GH6PM.h"



void Direction(char input_char)
{
    SYSCTL_RCGCGPIO_R|=(1<<2);
           if(input_char=='F')  // Forward
           {
               GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);

           }
           else if(input_char=='B') // Backward
           {
               GPIO_PORTC_DATA_R|=(1<<5);   GPIO_PORTC_DATA_R&=~(1<<4);

            }
           else if (input_char=='S')  // Stop
           {
              GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);

           }

           else if (input_char=='R')  // Right
           {


           }
           else if (input_char=='L')  // Left
           {


           }

}

void Motors_init(void)
{
       // Enable Clock for Port C
        SYSCTL_RCGCGPIO_R|=(1<<2);

        // Set Control Pins as Output
        GPIO_PORTC_DIR_R|=(1<<4);         GPIO_PORTC_DIR_R|=(1<<5);

        // Disable alternate Functions
        GPIO_PORTC_AFSEL_R&=~(1<<4);      GPIO_PORTC_AFSEL_R&=~(1<<5);

        //Digital Enable
        GPIO_PORTC_DEN_R|=(1<<4);         GPIO_PORTC_DEN_R|=(1<<5);
}
