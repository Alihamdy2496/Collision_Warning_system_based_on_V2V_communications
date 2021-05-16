//=========================================================================
//  PROJECT     : Service_WIFI.h
//  Created on  : 23 oct 2018
//  Author      : GP team
//  Description : Service layer of the esp8266 WIFI module driver
//====================s=====================================================

#ifndef SERVICE_WIFI_H_
#define SERVICE_WIFI_H_
#define MAX_NUMBER_OF_CARS  MAX_NUMBER_OF_APs
#define FRAME_LENGHT  MAX_LEN_OF_RECV_DATA
#include "HAL_WIFI.h"
#include <string.h>
#include <stdlib.h>
char APs[MAX_NUMBER_OF_APs][MAX_LEN_OF_SINGLE_AP_SSID];
ESP_RETURN_DATA DATA[MAX_NUMBER_OF_CARS];
typedef struct{
    char Car_IP[12];
    uint8 speed;
    char Transmission_state;
    char Direction;
    char Latitude[13]; /*11 for the double and 1 for '\0'*/
    char Longitude[13];
    char Altitude[13];
    uint8 bluetooth_speed;
}FRAME_DATA ;
extern FRAME_DATA FDATA[MAX_NUMBER_OF_CARS];
extern FRAME_DATA THIS_CAR_DATA;
bool ESP_GET_OTHER_CARS_DATA(void);
void ESP_FDATA_CLEAR(void);
bool ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Command_Sending(void);
sint8 ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Response_recv(void);
bool ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Command_sending(uint8 CAR_INDEX);
bool ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Response_Recv();
bool ESP_SEND_THIS_CAR_DATA___SENDING_TO_SINGLE_CAR(uint8 CAR_INDEX,bool LAST);
static void itoaa(long unsigned int value, char* result, int base);
//FRAME ExAMPLE ->192.168.4.1 030 F L 092725.0000 4717.113990 00833.91590
#endif /* SERVICE_WIFI_H_ */
