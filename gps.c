/*
 * GPS_GGA.c
 *
 *  Created on: Oct 28, 2018
 *      Author: user
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gps.h"
#include "UART3.h"
#include "UART.h"
#include "GPIO_Driver.h"
#include"lcd_8mode1.h"
//char* frame = "$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B \n";
uint8 comma_Location[9]={0};
uint8 frame[150]={0};
const char * get_location(uint8 index)
{
    char temp [11]={0};  uint8 j=0,k=0;
    for (k = comma_Location[index]+1; frame[k]!=','; k++)
    {
        temp [j] = frame[k];
        j++;
    }
    temp[j]=0;

    return temp;
}
void get_Buffer()
{
    struct Car_location mycar;
    uint8 i = 0;
    uint8 j = 0;
    uint8 k = 0;
    uint8 v=1;
    uint8 flag = 0;
    frame[0]=UART3_recieveByte();
    for(k=0;frame[0]!='$';k++)
    {
        frame[0]=UART3_recieveByte();
    }
    frame[1]=UART3_recieveByte();
    v=1;
    while(frame[v]!='$')
    {
        v++;
        frame[v]=UART3_recieveByte();
    }
    frame[v]=0;
    frame[v-1]='\0';
    for (i=0,j=1 ; frame[i] != '\0' ; i++)
    {
        if( frame[i] == 'L'&&frame[i-1] == 'L'&&frame[i-2] == 'G')
         {
                flag=1;
                comma_Location[0]=i+1;
                i+=2;
         }
        if( frame[i] == ','&&flag==1)
        {
            comma_Location[j] = i;
            j++;
        }
    }
    flag=0;
    strcpy(THIS_CAR_DATA.Latitude, get_location(0));
    strcpy(THIS_CAR_DATA.Longitude, get_location(2));
    //strcpy(THIS_CAR_DATA.Altitude, get_location(8));
    THIS_CAR_DATA.Latitude[12]=0;
    THIS_CAR_DATA.Longitude[12]=0;
    //THIS_CAR_DATA.Altitude[12]=0;
    for(i=0;i<9;i++)
    {
        comma_Location[j] = 0;
    }
        strcpy(frame,"");
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
  return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
  return (rad * 180 / pi);
}


double Distance(double lat1, double lon1, double lat2, double lon2, char unit)
{
    int r = 6371;
    double deltaLat;
    double deltaLon;
    deltaLat = deg2rad(lat2 - lat1);
    deltaLon = deg2rad(lon2 - lon1);
    lat1=deg2rad(lat1);
    lat2=deg2rad(lat2);

    double v_a;
    double v_c;
    double distance;

    v_a = sin(deltaLat/2) * sin(deltaLat/2) + cos(lat1) * cos(lat2) * sin(deltaLon/2) * sin(deltaLon/2);
    v_c = 2 * atan2(sqrt(v_a),sqrt(1-v_a));
    distance = r * v_c;
    if(unit=='M') return distance*1000;
    else if(unit =='K') return distance;
    else return -1;

 }

