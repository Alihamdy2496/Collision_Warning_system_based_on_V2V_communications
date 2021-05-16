//=========================================================================
//  PROJECT     : HAL_WIFI.c
//  Created on  : 25 nov 2018
//  Author      : GP team
//  Description : HAl layer of the esp8266 WIFI module driver
//==========================================================================
#include"HAL_WIFI.h"
static char receive_Buffer[1024]={0};//The hardware TX & RX BUFFERS OF THE ESP are 128 bytes long
static char wait_Buffer[1024]={0};
static char send_Buffer[100]={0};
char* ESP_Send_AT_Command_and_Receive_Response(char* ATCommand)
{
    int i;
    uint8 TEMP=0;
    for(i=0;receive_Buffer[i]!=0;i++)
    {
        receive_Buffer[i]=0;
    }
    if( !(strcmp(ATCommand,"J")==0||strcmp(ATCommand,"L")==0) )
    {
        UART7_OutString(ATCommand);
        UART7_OutString("\r\n");
    }
    receive_Buffer[0]=UART7_InChar();
    for(i=1 ;i<=strlen(ATCommand)+5-TEMP;i++)
    {
        receive_Buffer[0]=UART7_InChar();
    }
    receive_Buffer[1]=UART7_InChar();
    receive_Buffer[2]=UART7_InChar();
    i=2;
    if(ATCommand[6]=='S'&&ATCommand[9]=='R')
    {
        while(!(receive_Buffer[i]==13&&(receive_Buffer[i-1]=='T'||receive_Buffer[i-1]=='D')))
        {
            i++;
            receive_Buffer[i]=UART7_InChar();
        }
        if(receive_Buffer[i-1]=='T')
        {
            receive_Buffer[i]=UART7_InChar();//10
            receive_Buffer[i]=UART7_InChar();//13
            receive_Buffer[i]=UART7_InChar();//1
            receive_Buffer[i]=UART7_InChar();//O
            receive_Buffer[i]=UART7_InChar();//K
            receive_Buffer[i]=UART7_InChar();//13
        }
    }
    else if(strcmp(ATCommand,"J")==0)//at+cwjap
    {
        while((!(receive_Buffer[i]==13&&( (receive_Buffer[i-1]=='K'&&receive_Buffer[i-2]=='O') || (receive_Buffer[i-1]=='L'&&receive_Buffer[i-2]=='I') ))))
        {
            i++;
            receive_Buffer[i]=UART7_InChar();
        }
    }
    else
    {
        while((!( (receive_Buffer[i]==13&&receive_Buffer[i-1]=='K'&&receive_Buffer[i-2]=='O') || (receive_Buffer[i]==13&&receive_Buffer[i-1]=='y'&&receive_Buffer[i-2]=='d') || (receive_Buffer[i]==13&&receive_Buffer[i-1]=='R'&&receive_Buffer[i-2]=='O')) ))
        {
            i++;
            receive_Buffer[i]=UART7_InChar();
        }
    }
    receive_Buffer[i]=0;
    i=UART7_InChar();
    TEMP=0;
    return receive_Buffer;
}
bool ESP_init(const char* IP_IN_SAP_MODE,const char* IP_IN_STA_MODE,const char* THIS_SAP_SERVER_PORT_NUMBER)
{
    int i;
    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    uart7_init();
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT"),"OK")!=0)
        return False;

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWMODE=2"),"OK")!=0)
        return False;

    strcat(send_Buffer,"AT+CIPAP_DEF="); strcat(send_Buffer,IP_IN_SAP_MODE);
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response(send_Buffer),"OK")!=0)
        return False;

    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWMODE=1"),"OK")!=0)
        return False;

    strcat(send_Buffer,"AT+CIPSTA_DEF="); strcat(send_Buffer,IP_IN_STA_MODE);
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response(send_Buffer),"OK")!=0)
        return False;

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWMODE=2"),"OK")!=0)
        return False;

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CIPMUX=1"),"OK")!=0)
        return False;

    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CIPSERVER=1,"); strcat(send_Buffer,THIS_SAP_SERVER_PORT_NUMBER);
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response(send_Buffer),"OK")!=0)
        {
        //return False;
        }

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CIPSTO=1"),"OK")!=0)//set server times out to 1 sec
        return False;

    return True;
}
bool ESP_SAP_AND_SERVER_Configuration(const char* SSID,const char* PWD)
{
    int i;

    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWMODE_CUR=2"),"OK")!=0)
        return False;

    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CWSAP_DEF="); strcat(send_Buffer,SSID); strcat(send_Buffer,",");
    strcat(send_Buffer,PWD); strcat(send_Buffer,",1,3");
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response(send_Buffer),"OK")!=0)//DELAY
        return False;

    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CIPSTO=1"),"OK")!=0)//set server times out to 1 sec
        return False;

    return True ;
}
bool ESP_STA_Configuration()
{
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWMODE_CUR=1"),"OK")!=0)
        return False;
    return True;
}
void ESP_List_Nearby_APs__Command_sending()
{
    UART7_OutString("AT+CWLAP");
    UART7_OutString("\r\n");
}
bool ESP_List_Nearby_APs__APs_recv()
{
    int i,j;
    char* temp=ESP_Send_AT_Command_and_Receive_Response("L");
    if(strlen(temp)==0)
        return False;
    uint8 counter=0;
    for(i=0;i<strlen(temp);i++)
    {
        if(temp[i]=='\"')
        {
            if(counter%3==0)
            {
                i++;
                for(j=0;temp[i]!='\"';i++,j++)
                {
                    APs[counter/3][j]=temp[i];
                }
            }
            counter++;
        }
    }
    return True;
}
void APs_CLEAR()
{
    int i;
    int j;
    for(i=0;i<MAX_NUMBER_OF_APs;i++)
    {
        for(j=0;j<MAX_LEN_OF_SINGLE_AP_SSID;j++)
        {
            APs[i][j]=0;
        }
    }
}
void ESP_Join_AP__Command_sending(uint8 AP_INDIX,char* PWD)
{
    int i;
    for(i=0;i<80;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CWJAP_CUR=\""); strcat(send_Buffer,APs[AP_INDIX]);
    strcat(send_Buffer,"\"");    strcat(send_Buffer,",");
    strcat(send_Buffer,PWD);
    //char* temp=ESP_Send_AT_Command_and_Receive_Response(send_Buffer);
    UART7_OutString(send_Buffer);
    UART7_OutString("\r\n");
}
bool ESP_Join_AP__Response_Recv(void)
{
    char* temp=ESP_Send_AT_Command_and_Receive_Response("J");
    if(temp[3]!='C')//WIFI CONNECTED
        return False;

    return True;
}
bool ESP_Quit_AP()
{
    int i;
    for(i=0;receive_Buffer[i]!=0;i++)
    {
        receive_Buffer[i]=0;
    }
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response("AT+CWQAP"),"OK")!=0)
        return False;
    i=0;
    while(!(receive_Buffer[i]=='T'&&receive_Buffer[i-1]=='C'))
    {
        i++;
        receive_Buffer[i]=UART7_InChar();
    }
    return True;
}
bool ESP_START_CONNECTION_AT_STA_MODE(char* DEST_AP_PORT_NUMBER,char* DEST_IP,char* CHANNEL_ID)
{
    int i;
    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CIPSTART="); strcat(send_Buffer,CHANNEL_ID); strcat(send_Buffer,",\"TCP\",");
    strcat(send_Buffer,DEST_IP); strcat(send_Buffer,","); strcat(send_Buffer,DEST_AP_PORT_NUMBER);
    char* temp=ESP_Send_AT_Command_and_Receive_Response(send_Buffer);
    if(temp[0]=='C') //the correct response is CONNECT
        return True;
    return False;
}
bool ESP_SEND_DATA_AT_STA_Mode(char* DATA,char* CHANNEL_ID,char* DATA_LEN)
{
    int i;
    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    for(i=0;receive_Buffer[i]!=0;i++)
    {
        receive_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CIPSEND="); strcat(send_Buffer,CHANNEL_ID); strcat(send_Buffer,",");
    strcat(send_Buffer,DATA_LEN);
    char* temp=ESP_Send_AT_Command_and_Receive_Response(send_Buffer);
    if(strcmp(temp,"OK")!=0)
        return False;
    for(i=0;i<70;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,DATA);strcat(send_Buffer,"\r\n");
    UART7_OutString(send_Buffer);
    receive_Buffer[0]=UART7_InChar();
    receive_Buffer[1]=UART7_InChar();
    receive_Buffer[2]=UART7_InChar();
    i=2;
    while(!(receive_Buffer[i]=='s'&&receive_Buffer[i-1]=='e'))
    {
        i++;
        receive_Buffer[i]=UART7_InChar();
    }
    //receive_Buffer[i]=UART7_InChar();
    //receive_Buffer[i]=0;
    //if(! (receive_Buffer[i-1]=='K'&&receive_Buffer[i-2]=='O') ) //the correct response is Recv DATA length bytes SEND OK
      //  return False;
    return True;
}
bool ESP_END_CONNECTION_AT_STA_MODE(char* CHANNEL_ID)
{
    int i;
    for(i=0;send_Buffer[i]!=0;i++)
    {
        send_Buffer[i]=0;
    }
    strcat(send_Buffer,"AT+CIPCLOSE="); strcat(send_Buffer,CHANNEL_ID);
    if(strcmp(ESP_Send_AT_Command_and_Receive_Response(send_Buffer),"OK")!=0);
        //return False;

    return True;
}
bool ESP_RECEIVE_DATA_AT_SERVER_MODE(void)
{
    int i=0,j=0;
    char temp[3]={0};
    uint8 AP_COUNTER=0; uint8 DATA_LENGTH=0;
    i=1;
    if(wait_Buffer[0]==0)
    {
        return False;
    }
    for(i=0;wait_Buffer[i]!=0;i++)
    {
        if(wait_Buffer[i]=='+'&&wait_Buffer[i+1]=='I'&&wait_Buffer[i+2]=='P'&&wait_Buffer[i+3]=='D')
        {
            DATA[AP_COUNTER].CHANNEL_ID=(wait_Buffer[i+5]-'0');
            temp[0]=wait_Buffer[i+7];
            temp[1]=wait_Buffer[i+8];
            DATA_LENGTH=atoi(temp);
            for(j=0;j<DATA_LENGTH;j++,i++)
            {
                DATA[AP_COUNTER].DATA[j]=wait_Buffer[i+10];
            }
            AP_COUNTER++;
            i+=8;
        }
    }
    return True;
}
void ESP_RECEIVE_BUFFER_CLEAR(void)
{
    int i=0,j=0;
    for(i=0;i<MAX_NUMBER_OF_APs;i++)
    {
        DATA[i].CHANNEL_ID=0;
        for(j=0;j<MAX_LEN_OF_RECV_DATA;j++)
        {
            DATA[i].DATA[j]=0;
        }
    }
    for(i=0;wait_Buffer[i]!=0;i++)
    {
        wait_Buffer[i]=0;
    }
}
void UART7_Handler()
{
    int i=0,j=0;
    /*88=0,connet+frame+o,closed*/
    while(wait_Buffer[i]!=0)
    {
        i++;
    }
    while(j<88)
     {
         wait_Buffer[i]=UART7_InChar();
         i++;j++;
     }
}
