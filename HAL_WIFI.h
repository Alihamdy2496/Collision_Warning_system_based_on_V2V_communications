//=========================================================================
//  PROJECT     : HAL_WIFI.h
//  Created on  : 25 nov 2018
//  Author      : GP team
//  Description : HAl layer of the esp8266 WIFI module driver
//====================s=====================================================
#ifndef HAL_WIFI_H_
#define HAL_WIFI_H_
#define MAX_NUMBER_OF_APs 20        //depend on the wifi range and the street
#define MAX_LEN_OF_SINGLE_AP_SSID 20
#define MAX_LEN_OF_RECV_DATA 57
#include"standard_types.h"
#include"macros.h"
#include"UART7.h"
#include"This_Car_info.h"
#include"string.h"
#include"TM4C123GH6PM.h"
extern char APs[MAX_NUMBER_OF_APs][MAX_LEN_OF_SINGLE_AP_SSID];
typedef struct{
    char DATA[MAX_LEN_OF_RECV_DATA];
    uint8 CHANNEL_ID;
}ESP_RETURN_DATA;
extern ESP_RETURN_DATA DATA[MAX_NUMBER_OF_APs];
char* ESP_Send_AT_Command_and_Receive_Response(char* ATCommand);
bool ESP_init(const char* IP_IN_SAP_MODE,const char* IP_IN_STA_MODE,const char* THIS_SAP_SERVER_PORT_NUMBER);
bool ESP_SAP_AND_SERVER_Configuration(const char* SSID,const char* PWD);
bool ESP_STA_Configuration(void);
void ESP_List_Nearby_APs__Command_sending(void);
bool ESP_List_Nearby_APs__APs_recv(void);
void APs_CLEAR(void);
void ESP_Join_AP__Command_sending(uint8 AP_INDIX,char* PWD);
bool ESP_Join_AP__Response_Recv();
bool ESP_Quit_AP(void);
bool ESP_START_CONNECTION_AT_STA_MODE(char* DEST_AP_PORT_NUMBER,char* DEST_IP,char* CHANNEL_ID);
bool ESP_SEND_DATA_AT_STA_Mode(char* DATA,char* CHANNEL_ID,char* DATA_LEN);
bool ESP_END_CONNECTION_AT_STA_MODE(char* CHANNEL_ID);
bool ESP_RECEIVE_DATA_AT_SERVER_MODE(void);
void ESP_RECEIVE_BUFFER_CLEAR(void);
#endif /* HAL_WIFI_H_ */
