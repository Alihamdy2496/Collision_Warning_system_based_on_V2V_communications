/*
 * gps.h
 *
 *  Created on: Oct 21, 2018
 *      Author: user
 */

#ifndef GPS_H_
#define GPS_H_
#include "standard_types.h"
#include "Service_WIFI.h"
#include "UART.h"
extern uint8 frame[150];
struct Car_location
{
    double latitude;
    double longitude;
    double altitude;
};
#include <stdio.h>
#include <math.h>
#include <string.h>
#define pi 3.14159265358979323846
extern FRAME_DATA THIS_CAR_DATA;
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Function prototypes                                           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

double deg2rad(double);
double rad2deg(double);
void get_Buffer();

double Distance(double lat1, double lon1, double lat2, double lon2, char unit);
const char * get_location(uint8 index);
static void itoaa(long unsigned int value, char* result, int base);


#endif /* GPS_H_ */
