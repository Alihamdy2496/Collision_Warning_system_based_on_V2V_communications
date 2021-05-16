/*
 * GPIO_Driver.h
 *
 *  Created on: Oct 19, 2018
 *      Author: user
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "std_types.h"
#include "TM4C123GH6PM.h"

extern void Void_Clock_Activator (u8 Port);

extern void Void_Direction (u8 Port,u8 Val);
extern void Void_Pin_Direction (u8 Port,u8 Pin,u8 Val);

extern void Void_Digital_Enable (u8 Port,u8 Val);
extern void Void_Pin_Digital_Enable (u8 Port,u8 Pin);

extern void Void_Wrie_Port(u8 Port,u8 Val);
extern void Void_Read_Port(u8 Port,u8 *Val);

extern void Void_Write_Pin(u8 Port,u8 Pin,u8 Val);
extern void Void_Read_Pin(u8 Port,u8 Pin,u8 *Val);

extern void Void_PUR (u8 Port,u8 Val);
extern void Void_Pin_PUR (u8 Port,u8 Pin);

extern void Void_Unlock_PF0(void);

extern void Delay_ms (u32 Time_ms);


#endif /* GPIO_DRIVER_H_ */
