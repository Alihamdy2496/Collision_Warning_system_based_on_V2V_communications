#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_

#include "GPIO_Driver.h"
//*********************
//control pins of lcd
#define LCD_RS_PRT 4
#define LCD_RS_PIN 3
//------------------------
#define LCD_RW_PRT 4
#define LCD_RW_PIN 4
//------------------------
#define LCD_E__PRT 4
#define LCD_E__PIN 2
//*****************************************
//lcd port & pins

//------------------------------
#define LCD_D4_PRT 5
#define LCD_D4_PIN 2
//-------------------------
#define LCD_D5_PRT 5
#define LCD_D5_PIN 3
//-------------------------
#define LCD_D6_PRT 3
#define LCD_D6_PIN 2
//------------------------
#define LCD_D7_PRT 3
#define LCD_D7_PIN 3
//********************
//===============================================================================================

extern void Void_LCD_CMD      (u8 cmd);
extern void Void_LCD_DATA     (s8 data);
extern void Void_LCD_INIT     (void);
extern void Void_LCD_Location (u8 x,u8 y);
extern void Void_LCD_Print    (s8* str);
extern void Void_LCD_NUM      (s32 num);
extern u32  U32_POW(u8 U8_NUM,u8 U8_PWR);
extern void Void_Send_Init_SEQ(u8 U8_INIT);
extern void Void_Wait_Busy_Flag ();
extern void Void_Clear_Screen ();
#endif /* LCD DRIVER_H_ */
