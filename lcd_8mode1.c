#include "lcd_8mode1.h"

extern void Void_LCD_CMD (u8 U8_CMD)
{

    Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(U8_CMD/16 )%2); // Assigning bit 4 to D4
    Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(U8_CMD/32 )%2); // Assigning bit 5 to D5
    Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(U8_CMD/64 )%2); // Assigning bit 6 to D6
    Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(U8_CMD/128)%2); // Assigning bit 7 to D7
//----------------------------------------------------------*
    Void_Write_Pin(LCD_RS_PRT,LCD_RS_PIN,0);              // RS=0 to command.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,0);              // RW=0 to write.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);              // Making E signal 1.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);              // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).

    Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(U8_CMD  )%2);   // Assigning bit 0 to D4
        Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(U8_CMD/2)%2);   // Assigning bit 1 to D5
        Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(U8_CMD/4)%2);   // Assigning bit 2 to D6
        Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(U8_CMD/8)%2);   // Assigning bit 3 to D7
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);              // Making E signal 1.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);              // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                          // Delay to overcome parasitic delays(see data sheet).


    Void_Wait_Busy_Flag ();                               // Waiting till processing ends.
}
//======================================================================================================================================================
extern void Void_Send_Init_SEQ(u8 U8_INIT)
{

       Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(U8_INIT/16 )%2); // Assigning bit 4 to D4
       Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(U8_INIT/32 )%2); // Assigning bit 5 to D5
       Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(U8_INIT/64 )%2); // Assigning bit 6 to D6
       Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(U8_INIT/128)%2); // Assigning bit 7 to D7
//----------------------------------------------------------*
    Void_Write_Pin(LCD_RS_PRT,LCD_RS_PIN,0);               // RS=0 to command.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,0);               // RW=0 to write.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);               // Making E signal 1.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);               // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(U8_INIT  )%2);   // Assigning bit 0 to D4
        Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(U8_INIT/2)%2);   // Assigning bit 1 to D5
        Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(U8_INIT/4)%2);   // Assigning bit 2 to D6
        Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(U8_INIT/8)%2);   // Assigning bit 3 to D7
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);               // Making E signal 1.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);               // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).

    Void_Wait_Busy_Flag ();                                // Waiting till processing ends.

}

//======================================================================================================================================================

extern void Void_LCD_DATA (s8 S8_Data)
{
   Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(S8_Data/16 )%2); // Assigning bit 4 to D4
   Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(S8_Data/32 )%2); // Assigning bit 5 to D5
   Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(S8_Data/64 )%2); // Assigning bit 6 to D6
   Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(S8_Data/128)%2); // Assigning bit 7 to D7
//-----------------------------------------------------------*
    Void_Write_Pin(LCD_RS_PRT,LCD_RS_PIN,1);               // RS=1 to data.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,0);               // RW=0 to write.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);               // Making E signal 1.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);               // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_D4_PRT,LCD_D4_PIN,(S8_Data  )%2);   // Assigning bit 0 to D4
        Void_Write_Pin(LCD_D5_PRT,LCD_D5_PIN,(S8_Data/2)%2);   // Assigning bit 1 to D5
        Void_Write_Pin(LCD_D6_PRT,LCD_D6_PIN,(S8_Data/4)%2);   // Assigning bit 2 to D6
        Void_Write_Pin(LCD_D7_PRT,LCD_D7_PIN,(S8_Data/8)%2);   // Assigning bit 3 to D7
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);               // Making E signal 1.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);               // Applying a negative edge on the E pin so that the LCD would take the data.
    Delay_ms(1);                                           // Delay to overcome parasitic delays(see data sheet).
    Void_Wait_Busy_Flag ();                                // Waiting till processing ends.
}

//======================================================================================================================================================

extern void Void_LCD_INIT()
{

    Void_Clock_Activator (LCD_RS_PRT);               // Activating RS port.
    Void_Clock_Activator (LCD_RW_PRT);               // Activating RW port.
    Void_Clock_Activator (LCD_E__PRT);               // Activating E  port.
    Void_Clock_Activator (LCD_D4_PRT);               // Activating D4 port.
    Void_Clock_Activator (LCD_D5_PRT);               // Activating D5 port.
    Void_Clock_Activator (LCD_D6_PRT);               // Activating D6 port.
    Void_Clock_Activator (LCD_D7_PRT);               // Activating D7 port.
//-----------------------------------------------------*
    Void_Pin_Direction (LCD_RS_PRT,LCD_RS_PIN,1);    // Making RS output pin.
    Void_Pin_Direction (LCD_RW_PRT,LCD_RW_PIN,1);    // Making RW output pin.
    Void_Pin_Direction (LCD_E__PRT,LCD_E__PIN,1);    // Making E  output pin.
    Void_Pin_Direction (LCD_D4_PRT,LCD_D4_PIN,1);    // Making D4 output pin.
    Void_Pin_Direction (LCD_D5_PRT,LCD_D5_PIN,1);    // Making D5 output pin.
    Void_Pin_Direction (LCD_D6_PRT,LCD_D6_PIN,1);    // Making D6 output pin.
    Void_Pin_Direction (LCD_D7_PRT,LCD_D7_PIN,1);    // Making D7 output pin.
//-----------------------------------------------------*
    Void_Pin_Digital_Enable (LCD_RS_PRT,LCD_RS_PIN); // Enabling digital function of RS pin.
    Void_Pin_Digital_Enable (LCD_RW_PRT,LCD_RW_PIN); // Enabling digital function of RW pin.
    Void_Pin_Digital_Enable (LCD_E__PRT,LCD_E__PIN); // Enabling digital function of E  pin.
    Void_Pin_Digital_Enable (LCD_D4_PRT,LCD_D4_PIN); // Enabling digital function of D4 pin.
    Void_Pin_Digital_Enable (LCD_D5_PRT,LCD_D5_PIN); // Enabling digital function of D5 pin.
    Void_Pin_Digital_Enable (LCD_D6_PRT,LCD_D6_PIN); // Enabling digital function of D6 pin.
    Void_Pin_Digital_Enable (LCD_D7_PRT,LCD_D7_PIN); // Enabling digital function of D7 pin.
//-----------------------------------------------------*
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0); // Initializing the pin with zero.
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,0); // Initializing the pin with zero.
    Void_Write_Pin(LCD_RS_PRT,LCD_RS_PIN,0); // Initializing the pin with zero.
//---------------------------------------------*
    Void_Send_Init_SEQ(0X33);                // Initialization sequence.
    Void_Send_Init_SEQ(0X32);                // Initialization sequence.

      Void_LCD_CMD(0x28);                      // 4-bit mode, 2-lines, 5*8-character size Configuration (function set instruction).
      Void_LCD_CMD(0X0c);                      // Blink off, screen on, cursor on (entry mode).
      Void_LCD_CMD(0X06);                      // Cursor moves right and no display shift (cursor or display shift).
      Void_LCD_CMD(0X01);                        //*Clear screen if desired.
}

//======================================================================================================================================================

extern void Void_LCD_Location (u8 U8_Column,u8 u8_Row)
{
    //SET DDRAM ADDRESS
    u8 first_char_adr[]={0x80,0xc0,0x94,0xd4};           //Accessing address in DDRAM.//check
    // Start must be 0x80 but the rest depend on the line length ie the number of character the line can take not the display can take.
    Void_LCD_CMD((first_char_adr[u8_Row-1])+U8_Column-1);// Assigning data to DDRAM.
}

//======================================================================================================================================================

extern void Void_LCD_Print(s8* S8_STR)
{
    u8 u8_Counter=0;                       // Counter.
    while(S8_STR[u8_Counter]>0)            // Looping the string till the null is found.
    {
        Void_LCD_DATA(S8_STR[u8_Counter]); // Sending character to the LCD.
        u8_Counter++;                      // Incrementing the counter for the next character.
    }


}

//======================================================================================================================================================

extern void Void_LCD_NUM (s32 S32_NUM)
{
    u8 U8_Digits_Num=1;                // Digits number to know how many digits to display.
    s32 S32_Temp=S32_NUM;              // Temporary variable to check if the number digits is complete.

    if(S32_NUM<0)                      // To check if the number is negative.
    {
        Void_LCD_DATA('-');            // Printing a '-' sign on display.
        S32_NUM=0-S32_NUM;             // Taking the absolute value of the number.
        S32_Temp=S32_NUM;              // Assigning the number value to temp.
    }

    if(S32_NUM>9)                      // Enter the loop if the number is more than nine ie the number consists of more than one digit.
    {
        for (U8_Digits_Num=0;(S32_Temp!=0);U8_Digits_Num++)
        {
            S32_Temp=S32_Temp/10;      // Dividing the temp value by 10 and incrementing the number of digits till the temp equals zero.
        }
    }

    U8_Digits_Num-=1;                  // Decrementing  the number of digits.

    while((U8_Digits_Num>0))           // Looping till number of digits is zero.
    {
        S32_Temp=U32_POW(10,U8_Digits_Num);        // Getting the weight of the decimal number ie tens or hundreds ..etc.
        Void_LCD_DATA(((S32_NUM/S32_Temp)%10)+48); // Printing only the digit representing number of the weight calculated added the ASCII of zero which is 48.
        U8_Digits_Num-=1;                          // Decrementing  the number of digits.
    }
    Void_LCD_DATA((S32_NUM % 10)+48);  // Printing the first digit which represent the weight of ones.
}

//======================================================================================================================================================

extern void Void_Clear_Screen ()
{
    Void_LCD_CMD(0X01);                        //*Clear screen if desired.

}

//======================================================================================================================================================

extern void Void_Wait_Busy_Flag ()
{
    u8 val=1;                                      // The value to read in.
    Void_Pin_Direction (LCD_D4_PRT,LCD_D4_PIN,0);  // Making D4 input.
    Void_Pin_Direction (LCD_D5_PRT,LCD_D5_PIN,0);  // Making D5 input.
    Void_Pin_Direction (LCD_D6_PRT,LCD_D6_PIN,0);  // Making D6 input.
    Void_Pin_Direction (LCD_D7_PRT,LCD_D7_PIN,0);  // Making D7 input.
//---------------------------------------------------*
    Void_Write_Pin(LCD_RS_PRT,LCD_RS_PIN,0);       // RS=0 to command.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,1);       // RW=1 to read (LCD pins are output pins).
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);       // Making E signal 1 so that the LCD puts data on bus.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
//---------------------------------------------------*
    do{
        Void_Read_Pin(LCD_D7_PRT,LCD_D7_PIN,& val);// polling for the busy flag to be cleared(Using interrupts would be much efficient).
    }while(val == 1);                              // Staying in the loop as long as the BF-Pin is high.

    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);       // Applying a negative edge on the E pin so that the LCD would understand that the first part of data is read
                                                   // and prepare the rest of data.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,1);       // Making E signal 1 so that the LCD puts data on bus.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
    Void_Write_Pin(LCD_E__PRT,LCD_E__PIN,0);       // Applying a negative edge on the E pin so that the LCD would understand that the first part of data is read.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).
//---------------------------------------------------*
    Void_Write_Pin(LCD_RW_PRT,LCD_RW_PIN,0);       //RW=0 to make LCD bus input.
    Delay_ms(1);                                   // Delay to overcome parasitic delays(see data sheet).


    Void_Pin_Direction (LCD_D4_PRT,LCD_D4_PIN,1);  //Making D4 output pin.
    Void_Pin_Direction (LCD_D5_PRT,LCD_D5_PIN,1);  //Making D5 output pin.
    Void_Pin_Direction (LCD_D6_PRT,LCD_D6_PIN,1);  //Making D6 output pin.
    Void_Pin_Direction (LCD_D7_PRT,LCD_D7_PIN,1);  //Making D7 output pin.
}

//======================================================================================================================================================



//======================================================================================================================================================

extern u32 U32_POW(u8 U8_NUM,u8 U8_PWR)
{
    u8 i=0;
    u32 number=(u32)U8_NUM;

    for(i=1;i<U8_PWR;i++)
    {
        number*=U8_NUM;
    }
    return number;
}
