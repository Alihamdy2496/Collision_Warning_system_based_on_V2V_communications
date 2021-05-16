//=========================================================================
//  PROJECT     : Service_WIFI.c
//  Created on  : 23 oct 2018
//  Author      : GP team
//  Description : Service layer of the esp8266 WIFI module driver
//====================s=====================================================
#include"Service_WIFI.h"
#include"string.h"

//FRAME ExAMPLE ->192.168.4.1 300 F L 092725.00** 4717.11399* 00833.91590
bool ESP_GET_OTHER_CARS_DATA()
{
    int i,j,k;
    char spd[3]={0};
    char check;

    if(ESP_RECEIVE_DATA_AT_SERVER_MODE()==False)
        return False;

    i=0;
    check=FDATA[i].Car_IP[i];
    while(check!=0)
    {
        i++;
        check=FDATA[i].Car_IP[i];
        if(i==MAX_NUMBER_OF_CARS)
            return False;
    }
    check=DATA[i].DATA[0];
    while(check!=0)
    {
        for(j=0;j<11;j++)
        {
            FDATA[i].Car_IP[j]=DATA[i].DATA[j];
        }
        spd[0]=DATA[i].DATA[12];
        spd[1]=DATA[i].DATA[13];
        spd[2]=DATA[i].DATA[14];
        FDATA[i].speed=(uint8)atoi(spd);
        FDATA[i].Transmission_state=DATA[i].DATA[16];
        FDATA[i].Direction=DATA[i].DATA[18];

        for(j=20,k=0;k<11;j++,k++)
        {
            FDATA[i].Latitude[k]=DATA[i].DATA[j];
        }
        for(j=32,k=0;k<11;j++,k++)
        {
            FDATA[i].Longitude[k]=DATA[i].DATA[j];
        }
        for(j=44,k=0;k<11;j++,k++)
        {
            FDATA[i].Altitude[k]=DATA[i].DATA[j];
        }
        i++;
        check=DATA[i].DATA[0];
    }
    ESP_RECEIVE_BUFFER_CLEAR();
    return True;
}
void ESP_FDATA_CLEAR()
{
    int i,j;
    for(i=0;i<MAX_NUMBER_OF_CARS;i++)
    {
        FDATA[i].Direction=0;
        FDATA[i].Transmission_state=0;
        FDATA[i].speed=0;
        for(j=0;j<11;j++)
        {
            FDATA[i].Car_IP[j]=0;
            FDATA[i].Altitude[j]=0;
            FDATA[i].Latitude[j]=0;
            FDATA[i].Longitude[j]=0;
        }
    }

}
bool ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Command_Sending()
{
    uart7_interrupt_disable();

    if(ESP_STA_Configuration()==False)
        return False;

    ESP_List_Nearby_APs__Command_sending();
}
sint8 ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Response_recv()
{
    int i;
    char test; uint8 counter=0;
    char carName[5]={0};

    if(ESP_List_Nearby_APs__APs_recv()==False)
        return False;

    counter=0;

    i=0;
    test=APs[0][0];
    while(test!=0)
    {
        carName[0]=APs[i][0];
        carName[1]=APs[i][1];
        carName[2]=APs[i][2];
        carName[3]=APs[i][3];
        carName[4]=0;
        //CAR_192.168.4.1_5
        if(strcmp(carName,"CAR_")==0)
        {
            if(counter!=i)
            {
                strcpy(APs[counter],"");
                strcpy(APs[counter],APs[i]);
            }
            counter++;
        }
        i++;
        test=APs[i][0];
    }
    if(ESP_SAP_AND_SERVER_Configuration(THIS_CAR_SSID,THIS_CAR_PWD)==False)
        return False;

    uart7_interrupt_enable();

    return counter-1;
}
bool ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Command_sending(uint8 CAR_INDEX)
{
    char pwd[11]={0};

    pwd[0]='\"';
    pwd[9]='\"';
    //192
    pwd[1]=APs[CAR_INDEX][4];
    pwd[2]=APs[CAR_INDEX][5];
    pwd[3]=APs[CAR_INDEX][6];

    //168
    pwd[4]=APs[CAR_INDEX][8];
    pwd[5]=APs[CAR_INDEX][9];
    pwd[6]=APs[CAR_INDEX][10];

    //4
    pwd[7]=APs[CAR_INDEX][12];
    //1
    pwd[8]=APs[CAR_INDEX][14];

    uart7_interrupt_disable();

    if(ESP_STA_Configuration()==False)
        return False;

    ESP_Join_AP__Command_sending(CAR_INDEX,pwd);

    return True;
}
bool ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Response_Recv()
{
    if(ESP_Join_AP__Response_Recv()==False)
        return False;

    return True;
}
bool ESP_SEND_THIS_CAR_DATA___SENDING_TO_SINGLE_CAR(uint8 CAR_INDEX,bool LAST)
{
    char IP[14]={0};
    char carName[5]={0}; char pwd[11]={0};
    char DATA_TO_SEND[56]={0};
    char x[2]={0}; int i;

    if(strlen(THIS_CAR_DATA.Latitude)<11)
    {
        for(i=strlen(THIS_CAR_DATA.Latitude);i<11;i++)
        {
            THIS_CAR_DATA.Latitude[i]='0';
        }
    }
    if(strlen(THIS_CAR_DATA.Altitude)<11)
    {
        for(i=strlen(THIS_CAR_DATA.Altitude);i<11;i++)
        {
            THIS_CAR_DATA.Altitude[i]='0';
        }
    }
    if(strlen(THIS_CAR_DATA.Longitude)<11)
    {
        for(i=strlen(THIS_CAR_DATA.Longitude);i<11;i++)
        {
            THIS_CAR_DATA.Longitude[i]='0';
        }
    }
    if( !(THIS_CAR_DATA.Transmission_state=='U'||THIS_CAR_DATA.Transmission_state=='D') )
    {
        THIS_CAR_DATA.Transmission_state='X';
    }
    if( !(THIS_CAR_DATA.Direction=='L'||THIS_CAR_DATA.Direction=='R') )
    {
        THIS_CAR_DATA.Direction='N';
    }
    itoaa((const long unsigned int)THIS_CAR_DATA.speed,carName,10);
    if(strlen(carName)==0)
    {
        for(i=0;i<3;i++)
        {
            carName[i]='0';
        }
    }
    else if(strlen(carName)==1)
    {
        pwd[0]=carName[0];
        carName[0]='0';
        carName[1]='0';
        carName[2]=pwd[0];
    }
    else if (strlen(carName)==2)
    {
        pwd[0]=carName[0];
        pwd[1]=carName[1];
        carName[0]='0';
        carName[1]=pwd[0];
        carName[2]=pwd[1];
    }
    else if (strlen(carName)>3)
    {
        carName[3]=0;
    }
    strcat(DATA_TO_SEND,THIS_CAR_STATIC_IP_IN_SAP_MODE_);strcat(DATA_TO_SEND," ");
    strcat(DATA_TO_SEND,carName);strcat(DATA_TO_SEND," ");
    x[0]=THIS_CAR_DATA.Transmission_state;
    strcat(DATA_TO_SEND,x);strcat(DATA_TO_SEND," ");
    x[0]=THIS_CAR_DATA.Direction;
    strcat(DATA_TO_SEND,x);strcat(DATA_TO_SEND," ");
    strcat(DATA_TO_SEND,THIS_CAR_DATA.Latitude);strcat(DATA_TO_SEND," ");
    strcat(DATA_TO_SEND,THIS_CAR_DATA.Longitude);strcat(DATA_TO_SEND," ");
    strcat(DATA_TO_SEND,THIS_CAR_DATA.Altitude);

    pwd[0]='\"';IP[0]='\"';
    pwd[9]='\"';IP[12]='\"';
    //192
    pwd[1]=APs[CAR_INDEX][4];IP[1]=APs[CAR_INDEX][4];
    pwd[2]=APs[CAR_INDEX][5];IP[2]=APs[CAR_INDEX][5];
    pwd[3]=APs[CAR_INDEX][6];IP[3]=APs[CAR_INDEX][6];
                     IP[4]='.';
    //168
    pwd[4]=APs[CAR_INDEX][8];IP[5]=APs[CAR_INDEX][8];
    pwd[5]=APs[CAR_INDEX][9];IP[6]=APs[CAR_INDEX][9];
    pwd[6]=APs[CAR_INDEX][10];IP[7]=APs[CAR_INDEX][10];
                     IP[8]='.';
    //4
    pwd[7]=APs[CAR_INDEX][12];IP[9]=APs[CAR_INDEX][12];IP[10]='.';
    //1
    pwd[8]=APs[CAR_INDEX][14];IP[11]=APs[CAR_INDEX][14];

    x[0]=APs[CAR_INDEX][16];

    if(ESP_START_CONNECTION_AT_STA_MODE(x,IP,"0")==False)
        return False;

    if(ESP_SEND_DATA_AT_STA_Mode(DATA_TO_SEND,"0","55")==False)
        return False;

    if(ESP_END_CONNECTION_AT_STA_MODE("0")==False)
        return False;

    if(ESP_Quit_AP()==False)
        return False;

    if(ESP_SAP_AND_SERVER_Configuration(THIS_CAR_SSID,THIS_CAR_PWD)==False)
        return False;

    uart7_interrupt_enable();

    if(LAST == True)
        APs_CLEAR();

    return True;
}
static void itoaa(long unsigned int value, char* result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0';}

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
    }

}
