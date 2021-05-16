/*
 * Bluetooth.c
 *
 *  Created on: Jan 26, 2019
 *      Author: Amany
 */

#include "Bluetooth.h"
typedef char bool;
//enum Blutooth_characters{Forward='F',Backward='B',Left='L',Right='R',G,I,H,J,S,W,w,U,u,V,v,X,x};

char Bluetooth( )
{
    bool flag_Max_or_not = 0;
    char temp;int i=0;
    char counters[18]={0};
    int index =0;
    char max ;
    for(i=0;i<=15;i++)
    {
        temp = UART2_InChar();
        if (temp == 'F')
            counters[0]++;
        else if(temp=='B')
            counters[1]++;
        else if(temp=='L')
            counters[2]++;
        else if(temp=='R')
            counters[3]++;
        else if(temp=='G')
            counters[4]++;
            else if(temp=='I')
                counters[5]++;
            else if(temp=='H')
                counters[6]++;
            else if(temp=='J')
                counters[7]++;
            else if(temp=='S')
                counters[8]++;
            else if(temp=='W')
                counters[9]++;
            else if(temp=='w')
                counters[10]++;
            else if(temp=='U')
                counters[11]++;
            else if(temp=='u')
                counters[12]++;
            else if(temp=='V')
                counters[13]++;
            else if(temp=='v')
                counters[14]++;
            else if(temp=='X')
                counters[15]++;
            else if(temp=='x')
                counters[16]++;
            else
               counters[17]++;
        if(temp == '0' || temp == '1' || temp == '2' || temp == '3' || temp == '4' || temp == '5' || temp == '6' ||temp == '7' || temp == '8' || temp == '9' || temp == 'q' || temp == 'D')
            {
            flag_Max_or_not=1;
                return temp;
            }
    }
   if(!flag_Max_or_not)
    {
   max= counters[0];
   for (i=1; i<=17;i++)
   {
       if (counters[i] >max)
       {
           index=i;
           max=counters[i];
       }
   }
    }
   for (i=0;i<17;i++)
   {
      if ( counters[i]== max && i != index)
       return Bluetooth();
   }
     if ( index ==0)
   return 'F';
     else if (index ==1)
         return 'B';
     else if (index ==2)
         return 'L';
     else if (index ==3)
         return 'R';
     else if (index ==4)
         return 'G';
     else if (index ==5)
             return 'I';
         else if (index ==6)
             return 'H';
         else if (index ==7)
             return 'J';
         else if (index ==8)
             return 'S';
         else if (index ==9)
             return 'W';
         else if (index ==10)
             return 'w';
         else if (index ==11)
             return 'U';
         else if (index ==12)
             return 'u';
         else if (index ==13)
             return 'V';
         else if (index ==14)
             return 'v';
         else if (index ==15)
             return 'X';
         else
             return 'x';
}
