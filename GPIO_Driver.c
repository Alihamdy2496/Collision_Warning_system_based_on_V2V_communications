/*
 * GPIO_Driver.c
 *
 *  Created on: Oct 19, 2018
 *      Author: user
 */


#include "GPIO_Driver.h"
extern void  Void_Clock_Activator (u8 Port)
{
    switch (Port)
    {
        case 0:SYSCTL_RCGCGPIO_R |=0x01;break; //PORT A
        case 1:SYSCTL_RCGCGPIO_R |=0x02;break; //PORT B
        case 2:SYSCTL_RCGCGPIO_R |=0x04;break; //PORT C
        case 3:SYSCTL_RCGCGPIO_R |=0x08;break; //PORT D
        case 4:SYSCTL_RCGCGPIO_R |=0x10;break; //PORT E
        case 5:SYSCTL_RCGCGPIO_R |=0x20;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Direction (u8 Port,u8 Val)
{
    switch (Port)
    {
        case 0:GPIO_PORTA_DIR_R =Val;break; //PORT A
        case 1:GPIO_PORTB_DIR_R =Val;break; //PORT B
        case 2:GPIO_PORTC_DIR_R =Val;break; //PORT C
        case 3:GPIO_PORTD_DIR_R =Val;break; //PORT D
        case 4:GPIO_PORTE_DIR_R =Val;break; //PORT E
        case 5:GPIO_PORTF_DIR_R =Val;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Pin_Direction (u8 Port,u8 Pin,u8 Val)
{
    if (Val==1)
    {
        switch (Port)
        {
            case 0:GPIO_PORTA_DIR_R |=(1<<Pin);break; //PORT A   any pin
            case 1:GPIO_PORTB_DIR_R |=(1<<Pin);break; //PORT B   any pin
            case 2:GPIO_PORTC_DIR_R |=(1<<Pin);break; //PORT C   any pin
            case 3:GPIO_PORTD_DIR_R |=(1<<Pin);break; //PORT D   any pin
            case 4:GPIO_PORTE_DIR_R |=(1<<Pin);break; //PORT E   any pin
            case 5:GPIO_PORTF_DIR_R |=(1<<Pin);break; //PORT F   any pin
            default:break;
        }
    }
    else
    {
        switch (Port)
        {
            case 0:GPIO_PORTA_DIR_R &=(~(1<<Pin));break; //PORT A   any pin
            case 1:GPIO_PORTB_DIR_R &=(~(1<<Pin));break; //PORT B   any pin
            case 2:GPIO_PORTC_DIR_R &=(~(1<<Pin));break; //PORT C   any pin
            case 3:GPIO_PORTD_DIR_R &=(~(1<<Pin));break; //PORT D   any pin
            case 4:GPIO_PORTE_DIR_R &=(~(1<<Pin));break; //PORT E   any pin
            case 5:GPIO_PORTF_DIR_R &=(~(1<<Pin));break; //PORT F   any pin
            default:break;
        }
    }
}
//**********************************************************************************
extern void Void_Digital_Enable (u8 Port,u8 Val)
{
    switch (Port)
    {
        case 0:GPIO_PORTA_DEN_R |=Val;break; //PORT A
        case 1:GPIO_PORTB_DEN_R |=Val;break; //PORT B
        case 2:GPIO_PORTC_DEN_R |=Val;break; //PORT C
        case 3:GPIO_PORTD_DEN_R |=Val;break; //PORT D
        case 4:GPIO_PORTE_DEN_R |=Val;break; //PORT E
        case 5:GPIO_PORTF_DEN_R |=Val;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Pin_Digital_Enable (u8 Port,u8 Pin)
{
    switch (Port)
    {
        case 0:GPIO_PORTA_DEN_R |=(1<<Pin);break; //PORT A
        case 1:GPIO_PORTB_DEN_R |=(1<<Pin);break; //PORT B
        case 2:GPIO_PORTC_DEN_R |=(1<<Pin);break; //PORT C
        case 3:GPIO_PORTD_DEN_R |=(1<<Pin);break; //PORT D
        case 4:GPIO_PORTE_DEN_R |=(1<<Pin);break; //PORT E
        case 5:GPIO_PORTF_DEN_R |=(1<<Pin);break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Wrie_Port(u8 Port,u8 Val)
{
    switch (Port)
    {
        case 0:GPIO_PORTA_DATA_R =Val;break; //PORT A
        case 1:GPIO_PORTB_DATA_R =Val;break; //PORT B
        case 2:GPIO_PORTC_DATA_R =Val;break; //PORT C
        case 3:GPIO_PORTD_DATA_R =Val;break; //PORT D
        case 4:GPIO_PORTE_DATA_R =Val;break; //PORT E
        case 5:GPIO_PORTF_DATA_R =Val;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Read_Port(u8 Port,u8 *Val)
{
    switch (Port)
    {
        case 0:*Val=GPIO_PORTA_DATA_R ;break; //PORT A
        case 1:*Val=GPIO_PORTB_DATA_R ;break; //PORT B
        case 2:*Val=GPIO_PORTC_DATA_R ;break; //PORT C
        case 3:*Val=GPIO_PORTD_DATA_R ;break; //PORT D
        case 4:*Val=GPIO_PORTE_DATA_R ;break; //PORT E
        case 5:*Val=GPIO_PORTF_DATA_R ;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Write_Pin(u8 Port,u8 Pin,u8 Val)
{
    switch(Val)
    {
        case 0:switch(Port)
                {
                    case 0:GPIO_PORTA_DATA_R &=(~(1<<Pin));break; //PORT A
                    case 1:GPIO_PORTB_DATA_R &=(~(1<<Pin));break; //PORT B
                    case 2:GPIO_PORTC_DATA_R &=(~(1<<Pin));break; //PORT C
                    case 3:GPIO_PORTD_DATA_R &=(~(1<<Pin));break; //PORT D
                    case 4:GPIO_PORTE_DATA_R &=(~(1<<Pin));break; //PORT E
                    case 5:GPIO_PORTF_DATA_R &=(~(1<<Pin));break; //PORT F
                    default:break;
                }
        break;
        case 1:switch(Port)
                {
                    case 0:GPIO_PORTA_DATA_R |=(1<<Pin);break; //PORT A
                    case 1:GPIO_PORTB_DATA_R |=(1<<Pin);break; //PORT B
                    case 2:GPIO_PORTC_DATA_R |=(1<<Pin);break; //PORT C
                    case 3:GPIO_PORTD_DATA_R |=(1<<Pin);break; //PORT D
                    case 4:GPIO_PORTE_DATA_R |=(1<<Pin);break; //PORT E
                    case 5:GPIO_PORTF_DATA_R |=(1<<Pin);break; //PORT F
                    default:break;
                }
        default:break;
    }
}
//**********************************************************************************
extern void Void_Read_Pin(u8 Port,u8 Pin,u8 *Val)
{
    switch (Port)
    {
        case 0:*Val=((GPIO_PORTA_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT A
        case 1:*Val=((GPIO_PORTB_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT B
        case 2:*Val=((GPIO_PORTC_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT C
        case 3:*Val=((GPIO_PORTD_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT D
        case 4:*Val=((GPIO_PORTE_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT E
        case 5:*Val=((GPIO_PORTF_DATA_R)&(1<<Pin));*Val >>= Pin;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_PUR (u8 Port,u8 Val)
{
    switch (Port)//Pull Up Resistance
    {
        case 0:GPIO_PORTA_PUR_R |=Val;break; //PORT A
        case 1:GPIO_PORTB_PUR_R |=Val;break; //PORT B
        case 2:GPIO_PORTC_PUR_R |=Val;break; //PORT C
        case 3:GPIO_PORTD_PUR_R |=Val;break; //PORT D
        case 4:GPIO_PORTE_PUR_R |=Val;break; //PORT E
        case 5:GPIO_PORTF_PUR_R |=Val;break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Pin_PUR (u8 Port,u8 Pin)
{
    switch (Port)//Pull Up Resistance for a particular pin
    {
        case 0:GPIO_PORTA_PUR_R |=(1<<Pin);break; //PORT A
        case 1:GPIO_PORTB_PUR_R |=(1<<Pin);break; //PORT B
        case 2:GPIO_PORTC_PUR_R |=(1<<Pin);break; //PORT C
        case 3:GPIO_PORTD_PUR_R |=(1<<Pin);break; //PORT D
        case 4:GPIO_PORTE_PUR_R |=(1<<Pin);break; //PORT E
        case 5:GPIO_PORTF_PUR_R |=(1<<Pin);break; //PORT F
        default:break;
    }
}
//**********************************************************************************
extern void Void_Unlock_PF0(void)
{
    GPIO_PORTF_LOCK_R=0x4C4F434B;   //Unlock PF0 "magic number key"
    GPIO_PORTF_CR_R=0x01;          //making PF0 Configureable
}
//**********************************************************************************
extern void Delay_ms (u32 Time_ms)
{
    u32 i;
    u16 j;
    for (i=Time_ms;i>0;i--)
            for(j=3180;j>0;j--)
                {;}
}
