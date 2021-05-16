#include "FreeRTOS.h"
#include "task.h"
#include "UART.h"
#include "uart2.h"
#include "lcd_8mode1.h"
#include "Bluetooth.h"
#include "Service_WIFI.h"
#include "motors.h"
#include "PWM.h"
#include "gps.h"
#include "UART3.h"
#include "Speed.h"
#include "Timer1.h"
#include "Ultrasoinc_2.h"
#include "string.h"
#include "PLL.h"
typedef struct{
    char CarIP[12];
    double distance;
    uint8 speed;
    uint8 state; /*0-->nothing   1-->BSW    2-->EEBl    3-->potenial_EEBL    4-->DNPW   5-->could be any thing*/
}Applications;
Applications Applications_data[MAX_NUMBER_OF_CARS]={0};
static uint8 my_distance=0;
TickType_t time1;
TickType_t time2;
uint8 BSW_state=0;
uint8 EEBL_state=0;
uint8 potentail_EEBL_state=0;
uint8 DNPW_state=0;
uint8 General_warning_state=0;
/*application definitions*/
#define bluetooth_speed_factor 5
#define MARGIN_SPEED 20
#define GPS_MARGIN 5
char Bluetooth_reading;
unsigned char x=0;
static unsigned short Counts;
FRAME_DATA FDATA[MAX_NUMBER_OF_CARS]={0};
unsigned long Reading=0;
unsigned short Trigger=0;
unsigned short us_order[6]={0};
FRAME_DATA THIS_CAR_DATA={0};
TaskHandle_t  Ultrasonic_1_ReadingHandle=NULL;
TaskHandle_t  WIFI_Frame_Send_handle=NULL;
TaskHandle_t  Applications_Detection_handle=NULL;
TaskHandle_t  GPS_handle;
char Bluetooth_reading=0;
typedef struct{
    unsigned int Front_reading;
    unsigned int Left_reading;
    unsigned int Right_reading;
    unsigned int Back_reading;
    unsigned int Back_Left_reading;
    unsigned int Back_Right_reading;
}Ultrasonic_reading;
Ultrasonic_reading Ultrasonic_readings;
bool WIFI_Frame_Send_checker=False;
sint8 WIFI_Frame_Send_Counter=0;
bool Applications_Detection_checker=False;
static uint8 coll[6]={0};
static uint8 APPs_coll[4]={0};/*0-->BSW left  1-->BSW right  2-->EEBL*/
static char previous_Transmission_state='X';
static char previous_Direction='N';
//#define ULTRASONIC_PRINT 1
//#define SPEED_PRINT 1
//#define GPS_PRINT 1
#define WIFI_PRINT 1
//#define APPLICATIONS_PRINT 1
bool WIFI_LISTING = False;
//#define second_car 1
bool print=False;
void UART2_Action(void);
void SpeedPinAction(void);
void TimerA1_Action(void);
void ultrasonic_action(void);
void GPS_Task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    while(1)
    {
        #ifdef GPS_PRINT
            Void_Clear_Screen();
            Void_LCD_Print("gps");
        #endif /*GPS_PRINT*/
        get_Buffer();
        #ifdef GPS_PRINT
            Void_Clear_Screen();
            Void_LCD_Print("gps done");
            Void_LCD_Print((s8*)THIS_CAR_DATA.Latitude);
            Void_LCD_Print((s8*)THIS_CAR_DATA.Longitude);
            Void_LCD_Print((s8*)THIS_CAR_DATA.Altitude);
        #endif /*GPS_PRINT*/

        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime,4000);
    }
}
void Ultrasonic_1_Reading( void * pvParameters )
{
    TickType_t xLastWakeTime;
    while(1)
    {
        #ifdef ULTRASONIC_PRINT
            Void_Clear_Screen();
        #endif /*ULTRASONIC_PRINT*/
        Trigger=0;
        Echo=0;
        if(BSW_state==1)
        {
            if(print==False)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    BSW__WARNING    ");
                #endif /*APPLICATIONS_PRINT*/
            }
            us_order[0]=2; us_order[1]=3; us_order[2]=4;
            us_order[3]=5; us_order[4]=6; us_order[5]=1;
            print = True;
        }
        if(EEBL_state==1)
        {
            if(print==False)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    EEBL_WARNING    ");
                #endif /*APPLICATIONS_PRINT*/
            }
            us_order[0]=6; us_order[1]=3; us_order[2]=4;
            us_order[3]=5; us_order[4]=2; us_order[5]=1;
            print = True;
        }
        if(potentail_EEBL_state==1)
        {
            if(print==False)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("   potential_EEBL   ");
                #endif /*APPLICATIONS_PRINT*/
            }
            us_order[0]=6; us_order[1]=3; us_order[2]=4;
            us_order[3]=5; us_order[4]=2; us_order[5]=1;
            print = True;
        }
        if(DNPW_state==1)
        {
            if(print==False)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    DNPW_WARNING    ");
                #endif /*APPLICATIONS_PRINT*/
            }
            us_order[0]=6; us_order[1]=4; us_order[2]=5;
            us_order[3]=2; us_order[4]=3; us_order[5]=1;
            print = True;
        }
        if(General_warning_state==1)
        {
            if(print==False)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("   General_WARNING  ");
                #endif /*APPLICATIONS_PRINT*/
            }
            us_order[0]=6; us_order[1]=2; us_order[2]=3;
            us_order[3]=4; us_order[4]=5; us_order[5]=1;
            print = True;
        }
        else
        {
            time1=0;
            time2=0;
            my_distance=0;
            if( (THIS_CAR_DATA.Transmission_state=='U'||THIS_CAR_DATA.Transmission_state=='X') && THIS_CAR_DATA.Direction=='N')
            {
                us_order[0]=6; us_order[1]=4; us_order[2]=5;
                us_order[3]=2; us_order[4]=3; us_order[5]=1;
            }
            if(THIS_CAR_DATA.Transmission_state=='D' && THIS_CAR_DATA.Direction=='N')
            {
                us_order[0]=1; us_order[1]=4; us_order[2]=5;
                us_order[3]=2; us_order[4]=3; us_order[5]=6;
            }
            if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='R')
            {
                us_order[0]=4; us_order[1]=2; us_order[2]=6;
                us_order[3]=5; us_order[4]=3; us_order[5]=1;
            }
            if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='L')
            {
                us_order[0]=5; us_order[1]=3; us_order[2]=6;
                us_order[3]=4; us_order[4]=2; us_order[5]=1;
            }
        }
        while(Trigger<=5)
        {
            while(Echo);
            if(us_order[Trigger]==1)
            {
                GPIO_PORTB_IM_R&=~(1<<3); GPIO_PORTB_IM_R&=~(1<<4); GPIO_PORTB_IM_R&=~(1<<5);
                GPIO_PORTB_IM_R&=~(1<<2); GPIO_PORTB_IM_R&=~(1<<1);
                GPIO_PORTB_IM_R|=(1<<0);
            }
            else if(us_order[Trigger]==2)
            {
                GPIO_PORTB_IM_R&=~(1<<2); GPIO_PORTB_IM_R&=~(1<<4); GPIO_PORTB_IM_R&=~(1<<5);
                GPIO_PORTB_IM_R&=~(1<<3); GPIO_PORTB_IM_R&=~(1<<0);
                GPIO_PORTB_IM_R|=(1<<1);
            }
            else if(us_order[Trigger]==3)
            {
                GPIO_PORTB_IM_R&=~(1<<1); GPIO_PORTB_IM_R&=~(1<<3); GPIO_PORTB_IM_R&=~(1<<5);
                GPIO_PORTB_IM_R&=~(1<<4); GPIO_PORTB_IM_R&=~(1<<0);
                GPIO_PORTB_IM_R|=(1<<2);
            }
            else if(us_order[Trigger]==4)
            {
                GPIO_PORTB_IM_R&=~(1<<2); GPIO_PORTB_IM_R&=~(1<<5); GPIO_PORTB_IM_R&=~(1<<4);
                GPIO_PORTB_IM_R&=~(1<<1); GPIO_PORTB_IM_R&=~(1<<0);
                GPIO_PORTB_IM_R|=(1<<3);
            }
            else if(us_order[Trigger]==5)
            {
                GPIO_PORTB_IM_R&=~(1<<2); GPIO_PORTB_IM_R&=~(1<<3); GPIO_PORTB_IM_R&=~(1<<1);
                GPIO_PORTB_IM_R&=~(1<<5); GPIO_PORTB_IM_R&=~(1<<0);
                GPIO_PORTB_IM_R|=(1<<4);
            }
            else if(us_order[Trigger]==6)
            {
                GPIO_PORTB_IM_R&=~(1<<2); GPIO_PORTB_IM_R&=~(1<<3); GPIO_PORTB_IM_R&=~(1<<4);
                GPIO_PORTB_IM_R&=~(1<<1);  GPIO_PORTB_IM_R&=~(1<<0);
                GPIO_PORTB_IM_R|=(1<<5);
            }

            Send_Trigger(us_order[Trigger]);
            vTaskSuspend(NULL);
            Trigger++;

            if(us_order[Trigger]==2)
                Trigger++;

        }
        #ifdef ULTRASONIC_PRINT
        //Void_LCD_Print(" ultra sonic done");
        #endif /*ULTRASONIC_PRINT*/
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime,500);
    }
}
void Applications_Detection(void * pvParameters)
{
    TickType_t xLastWakeTime; uint8 check=0;
    int i=0,j=0;double mylat,mylong,otherlat,otherlong;
    double dist;char check_for_car_existance;
    double test[2]={15000,200};
    uint8 counter=0;
    while(1)
    {
        #ifdef APPLICATIONS_PRINT
            Void_Clear_Screen();
            Void_LCD_Print("  Application task  ");
        #endif /*APPLICATIONS_PRINT*/
        Applications_Detection_checker=ESP_GET_OTHER_CARS_DATA();
        if(Applications_Detection_checker == True)
        {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("a frame is receved");
                Delay_ms(500);
            #endif //APPLICATIONS_PRINT
            i=0;
            check_for_car_existance=FDATA[i].Car_IP[0];
            while(check_for_car_existance!=0)
            {
                BSW_state=0;
                FDATA[i].Latitude[11]=0;
                FDATA[i].Longitude[11]=0;
                THIS_CAR_DATA.Latitude[11]=0;
                THIS_CAR_DATA.Longitude[11]=0;
                mylat=atof(FDATA[i].Latitude);
                mylong=atof(FDATA[i].Longitude);
                otherlat=atof(THIS_CAR_DATA.Latitude);
                otherlong=atof(THIS_CAR_DATA.Longitude);
                dist=Distance(mylat,mylong,otherlat,otherlong,'M')/100;
                counter=counter%2;
                dist = test[counter];
                counter++;

                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("Distance= ");
                    Void_LCD_NUM((s32)dist);
                    Delay_ms(3000);
                #endif //APPLICATIONS_PRINT
                j=0;
                check=Applications_data[j].CarIP[0];
                while(check!=0)//BSW_state=1->the car already exist
                {
                    time2 =xTaskGetTickCount();
                    my_distance+=((time2-time1)/1000) * THIS_CAR_DATA.speed;
                    time2=0;
                    time1=0;
                    #ifdef APPLICATIONS_PRINT
                        Void_Clear_Screen();
                        Void_LCD_Print(" my distance= ");
                        Void_LCD_NUM((s32)my_distance);
                        Delay_ms(3000);
                    #endif //APPLICATIONS_PRINT
                        Void_Clear_Screen();
                    if(Applications_data[j].state==3 && dist < Applications_data[j].distance)
                    {
                        Applications_data[j].state=2;
                        EEBL_state=1;
                        potentail_EEBL_state=0;
                        #ifdef APPLICATIONS_PRINT
                            Void_LCD_Print("     EEBL_state      ");
                        #endif //APPLICATIONS_PRINT
                    }
                    else if(!strcmp(Applications_data[j].CarIP,FDATA[i].Car_IP))
                    {
                        //same direction could be BSW or EEBL
                        if(dist > Applications_data[j].distance - my_distance + 2*GPS_MARGIN)
                        {
                            //BSW
                            if(FDATA[i].Transmission_state=='U')
                            {
                                #ifdef APPLICATIONS_PRINT
                                    Void_LCD_Print("      BSW_state      ");
                                #endif //APPLICATIONS_PRINT
                                BSW_state=1;
                                Applications_data[j].state=1;
                            }
                            //EEBL
                            else if( FDATA[i].Transmission_state=='U' && FDATA[i].speed < (THIS_CAR_DATA.speed+MARGIN_SPEED))
                            {
                                #ifdef APPLICATIONS_PRINT
                                    Void_LCD_Print("     EEBL_state      ");
                                #endif //APPLICATIONS_PRINT
                                EEBL_state=1;
                                Applications_data[j].state=2;
                            }
                        }//DNPW
                        else if(dist < Applications_data[j].distance - my_distance - 2*GPS_MARGIN)
                        {

                            #ifdef APPLICATIONS_PRINT
                                Void_LCD_Print("     EEBL_state      ");
                            #endif //APPLICATIONS_PRINT
                            DNPW_state=1;
                            Applications_data[j].state=4;
                        }//could be any case
                        else
                        {
                            //Void_LCD_Print(" general case");
                            #ifdef APPLICATIONS_PRINT
                                Void_LCD_Print(" General_warning_s  ");
                            #endif //APPLICATIONS_PRINT
                            General_warning_state=1;
                            Applications_data[j].state=5;
                        }
                        Delay_ms(500);
                    }
                    else //car stored that no long in range --> delete
                    {
                        Void_LCD_Print("if a car disappear");
                        Applications_data[j].distance=0;
                        Applications_data[j].CarIP[0]=0;
                        if(Applications_data[j].state==5)
                        {
                            General_warning_state=0;
                        }
                        else if(Applications_data[j].state==4)
                        {
                            DNPW_state=0;
                        }
                        else if(Applications_data[j].state==3)
                        {
                            potentail_EEBL_state=0;
                        }
                        else if(Applications_data[j].state==2)
                        {
                            EEBL_state=0;
                        }
                        else if(Applications_data[j].state==1)
                        {
                            BSW_state=0;
                        }
                        Applications_data[j].state=0;
                        print=False;
                    }
                j++;
                check=Applications_data[j].CarIP[0];
                }
                //new car
                if(FDATA[i].Transmission_state=='X')
                {
                    #ifdef APPLICATIONS_PRINT
                        Void_LCD_Print("       NEW CAR      ");
                        Void_LCD_Print("potentail_EEBL_state");
                    #endif //APPLICATIONS_PRINT
                    //potential EEBL collision
                    potentail_EEBL_state=1;
                    Applications_data[j].state=3;
                }
                #ifdef APPLICATIONS_PRINT
                    Void_LCD_Print("1st frame get stored ");
                #endif //APPLICATIONS_PRINT
                j=MAX_NUMBER_OF_CARS-1;
                check=Applications_data[j].CarIP[0];
                while(check==0)
                {
                    j--;
                    check=Applications_data[j].CarIP[0];
                }

                strcpy(Applications_data[0].CarIP,FDATA[i].Car_IP);
                Applications_data[0].distance=dist;
                Applications_data[0].speed=FDATA[i].speed;
                i++;
                check_for_car_existance=FDATA[i].Car_IP[0];
                time1 =  xTaskGetTickCount(); //start calculating the distance
            }
            //clear FDATA
            ESP_FDATA_CLEAR();
        }
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastWakeTime,5000);
    }
}
void WIFI_Frame_Send( void * pvParameters )
{
    TickType_t xLastWakeTime;
    int i;
    for( ;; )
    {
        if(WIFI_LISTING==False)
        {
            #ifdef second_car
            xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil( &xLastWakeTime, 10000);
            #endif /*second_car*/
            #ifdef WIFI_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("F1:Car listing ");
            #endif /*WIFI_PRINT*/
            Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Command_Sending();
            /*the time between sending the at command and receiving the response should be constant and correct so
             * we don't miss the response*/
            xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil( &xLastWakeTime, 1800);

            #ifdef WIFI_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("F2:Car listing response ");
            #endif /*WIFI_PRINT*/
            WIFI_Frame_Send_Counter=ESP_SEND_THIS_CAR_DATA__CARS_LISTING__Response_recv();
        }
        for(i=0;i<=WIFI_Frame_Send_Counter;i++)
        {
            WIFI_LISTING=True;
            #ifdef WIFI_PRINT
                //Void_Clear_Screen();
                //Void_LCD_Print("a Car is detected");
            #endif /*WIFI_PRINT*/
            xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil( &xLastWakeTime, 3000);/*time between listing and data sending*/

            if(i==WIFI_Frame_Send_Counter)
            {
                #ifdef WIFI_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("F3:start connection ");
                    Void_LCD_Print("seen car:");
                    Void_LCD_Print((s8*)APs[0]);
                #endif /*WIFI_PRINT*/
                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Command_sending(i);

                xLastWakeTime = xTaskGetTickCount();
                vTaskDelayUntil( &xLastWakeTime, 2500);/*time between command sending and response recv at connecting to single car*/
                /*this soukd be constand and correct*/

                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Response_Recv();
                if(Applications_Detection_checker==False)
                {
                    //WIFI_LISTING=False;
                }
                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___SENDING_TO_SINGLE_CAR(i,True);
                #ifdef WIFI_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("F4 and F5           ");
                    Void_LCD_Print("Sending Frame");
                #endif /*WIFI_PRINT*/
            }
            else
            {
                #ifdef WIFI_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("F3:start connection ");
                    Void_LCD_Print("seen car:");
                    Void_LCD_Print((s8*)APs[0]);
                #endif /*WIFI_PRINT*/
                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Command_sending(i);

                xLastWakeTime = xTaskGetTickCount();
                vTaskDelayUntil( &xLastWakeTime, 2500);/*time between command sending and response recv at connecting to single car*/

                #ifdef WIFI_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("F4 and F5           ");
                    Void_LCD_Print("sending frame");
                #endif /*WIFI_PRINT*/
                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___CONNECTING_TO_SINGLE_CAR__Response_Recv();

                Applications_Detection_checker=ESP_SEND_THIS_CAR_DATA___SENDING_TO_SINGLE_CAR(i,False);
            }
        }
        vTaskDelayUntil( &xLastWakeTime, 12000);
    }
}
int main(void)
{
    THIS_CAR_DATA.Transmission_state='X';
    THIS_CAR_DATA.Direction='N';

    xTaskCreate(Applications_Detection,"Applications_Detection",180,NULL,3,& Applications_Detection_handle);

    xTaskCreate(Ultrasonic_1_Reading,"Ultrasonic_1_Reading",150,NULL,1,&Ultrasonic_1_ReadingHandle);

    xTaskCreate(WIFI_Frame_Send,"WIFI_Frame_Send",150,NULL,4,&WIFI_Frame_Send_handle);

    xTaskCreate(GPS_Task,"GPS",128,NULL,2,&GPS_handle);

    PLL_Init();
    UART0_Init(115200,80000000);

    //Enable Clock for Port B
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; //enable clock to the GPIO

    // Enable Clock for Port A
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; //enable clock to the GPIO

    // Enable Clock for Port D
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; //enable clock to the GPIO

    configure_callback(ultrasonic_action);
    Captureinit();
    Configure_Echo();
    Configure_Trigger();
    Enable_Ultrasionc_Interrupts();

    if(ESP_init(THIS_CAR_STATIC_IP_IN_SAP_MODE,THIS_CAR_STATIC_IP_IN_STA_MODE,THIS_CAR_SERVER_PORT_NUMBER)==True)
    {
        uart7_interrupt_enable();

        Void_LCD_INIT();

        UART3_INIT();

        uart2_init(UART2_Action);

        Configure_Speed_pin(SpeedPinAction);
        Enable_Speed_Interrupt();
        Timer1A_Init(640000000,TimerA1_Action);

        Motors_init();
        Timer0B_PWM_Init(29000,0);
        Timer0A_PWM_Init(29000,0);

        Void_LCD_Print("start");

        vTaskStartScheduler();

    }
    while(1)
    {


    }

}
void ultrasonic_action(void)
  {
    BaseType_t check;
    if (!Echo)
      {
        TIMER2_TAV_R  = 0;
        TIMER2_CTL_R= 0x00000001;
        Echo=1;
      }

      else
       {
        pulse = TIMER2_TAR_R;
        TIMER2_CTL_R = 0x00000000;
        Echo=0;
        Reading =(unsigned long)(pulse/80.0);
        Reading = Reading / 58;
        if(Reading>=400)
            Reading=400;
        if(us_order[Trigger]==1)
        {
            Ultrasonic_readings.Back_reading=Reading;
        }
        else if(us_order[Trigger]==2)
        {
            Ultrasonic_readings.Back_Right_reading=Reading;
        }
        else if(us_order[Trigger]==3)
        {
            Ultrasonic_readings.Back_Left_reading=Reading;
        }
        else if(us_order[Trigger]==4)
        {
            Ultrasonic_readings.Right_reading=Reading;
        }
        else if(us_order[Trigger]==5)
        {
            Ultrasonic_readings.Left_reading=Reading;
        }
        else if(us_order[Trigger]==6)
        {
            Ultrasonic_readings.Front_reading=Reading;
        }
        if(BSW_state==1)
        {
            if(us_order[Trigger]==2 && Reading < 150 && Reading > 50)
            {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("     BSW_WARNING    ");
                Void_LCD_Print("   DONT TURN RIGHT  ");
            #endif //APPLICATIONS_PRINT
                APPs_coll[1]=1;
            }
            else if(us_order[Trigger]==3 && Reading < 150 && Reading > 50)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("     BSW_WARNING    ");
                    Void_LCD_Print("   DONT TURN LEFT   ");
                #endif //APPLICATIONS_PRINT
                APPs_coll[0]=1;
            }
            else
            {
                APPs_coll[0]=0;
                APPs_coll[1]=0;
            }
        }
        if(EEBL_state==1)
        {
            if(us_order[Trigger]==6 && Reading < 150 && Reading > 50)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    EEBL_WARNING    ");
                    Void_LCD_Print("        STOP!       ");
                #endif //APPLICATIONS_PRINT
                APPs_coll[2]=1;
                THIS_CAR_DATA.Transmission_state='X';
                THIS_CAR_DATA.Direction='N';
                GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(0);
                Timer0B_PWM_Duty(0);
            }
            else
            {
                APPs_coll[2]=0;
            }
        }
        if(potentail_EEBL_state==1)
        {
            if(us_order[Trigger]==6 && Reading < 150 && Reading > 50)
            {
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    EEBL_WARNING    ");
                    Void_LCD_Print("        STOP!       ");
                #endif //APPLICATIONS_PRINT
                print=False;
                APPs_coll[2]=1;
                THIS_CAR_DATA.Transmission_state='X';
                THIS_CAR_DATA.Direction='N';
                GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(0);
                Timer0B_PWM_Duty(0);
            }
            else
            {
                APPs_coll[2]=0;
            }
        }
        if(General_warning_state==1)
        {
            if(us_order[Trigger]==6 && Reading < 150 && Reading > 50)
            {
                EEBL_state=1;
                General_warning_state=0;
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("    EEBL_WARNING    ");
                    Void_LCD_Print("        STOP!       ");
                #endif //APPLICATIONS_PRINT
                APPs_coll[2]=1;
                THIS_CAR_DATA.Transmission_state='X';
                THIS_CAR_DATA.Direction='N';
                GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(0);
                Timer0B_PWM_Duty(0);
           }
            else if(us_order[Trigger]==2 && Reading < 150 && Reading > 50)
            {
                BSW_state=1;
                General_warning_state=0;
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("     BSW_WARNING    ");
                    Void_LCD_Print("   DONT TURN RIGHT  ");
                #endif //APPLICATIONS_PRINT
                APPs_coll[1]=1;
            }
            else if(us_order[Trigger]==3 && Reading < 150 && Reading > 50)
            {
                BSW_state=1;
                General_warning_state=0;
                #ifdef APPLICATIONS_PRINT
                    Void_Clear_Screen();
                    Void_LCD_Print("     BSW_WARNING    ");
                    Void_LCD_Print("   DONT TURN LEFT   ");
                #endif //APPLICATIONS_PRINT
                APPs_coll[0]=1;
            }
            else
            {
                APPs_coll[0]=0;
                APPs_coll[1]=0;
                APPs_coll[2]=0;
            }
        }
        if(Reading>50)
        {
            coll[us_order[Trigger]-1]=0;
        }
        else if(Reading<50 && Reading>0)
        {
            coll[us_order[Trigger]-1]=1;
            if(THIS_CAR_DATA.Transmission_state=='X')
            {

            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("     Watch out!     ");
                Void_LCD_Print(" something is around ");
            #endif /*ULTRASONIC_PRINT*/
            }
            else if(us_order[Trigger]==1)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U')//speed up forward
                {
                    #ifdef ULTRASONIC_PRINT
                        Void_LCD_Print("        BACK        ");
                    #endif /*ULTRASONIC_PRINT*/
                    THIS_CAR_DATA.Transmission_state='U';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0B_PWM_Duty(2900*11);
                    Timer0A_PWM_Duty(2900*11);
                }
                else if(THIS_CAR_DATA.Transmission_state=='D')//stop
                {
                    #ifdef ULTRASONIC_PRINT
                        Void_LCD_Print("     Watch out!     ");
                        Void_LCD_Print("        BACK        ");
                    #endif /*ULTRASONIC_PRINT*/
                    THIS_CAR_DATA.Transmission_state='X';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0A_PWM_Duty(0);
                    Timer0B_PWM_Duty(0);
                }
            }
            else if(us_order[Trigger]==2)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                    Void_LCD_Print("BACK_Right_BlindSpot");
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='R')
                {
                    THIS_CAR_DATA.Transmission_state='U';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                    Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                }
            }
            else if(us_order[Trigger]==3)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                    Void_LCD_Print("    BACK_Left_BSW   ");
                    Delay_ms(3000);
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='L')
                {
                    THIS_CAR_DATA.Transmission_state='U';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                    Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                }

            }
            else if(us_order[Trigger]==4)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                    Void_LCD_Print("       Right        ");
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='R')
                {
                    THIS_CAR_DATA.Transmission_state='U';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                    Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                }
            }
            else if(us_order[Trigger]==5)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                    Void_LCD_Print("        Left        ");
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U' && THIS_CAR_DATA.Direction=='L')
                {
                    THIS_CAR_DATA.Transmission_state='U';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                    Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                }
            }
            else if(us_order[Trigger]==6)
            {
                #ifdef ULTRASONIC_PRINT
                    Void_LCD_Print("     Watch out!     ");
                #endif /*ULTRASONIC_PRINT*/
                if(THIS_CAR_DATA.Transmission_state=='U')//stop
                {
                    #ifdef ULTRASONIC_PRINT
                        Void_LCD_Print("       Front        ");
                    #endif /*ULTRASONIC_PRINT*/
                    THIS_CAR_DATA.Transmission_state='X';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
                    Timer0A_PWM_Duty(0);
                    Timer0B_PWM_Duty(0);
                }
                else if(THIS_CAR_DATA.Transmission_state=='D')//speed up backward
                {
                    #ifdef ULTRASONIC_PRINT
                        Void_LCD_Print("       Front        ");
                    #endif /*ULTRASONIC_PRINT*/
                    THIS_CAR_DATA.Transmission_state='D';
                    THIS_CAR_DATA.Direction='N';
                    GPIO_PORTC_DATA_R|=(1<<5);   GPIO_PORTC_DATA_R&=~(1<<4);
                    Timer0B_PWM_Duty(2900*11);
                    Timer0A_PWM_Duty(2900*11);
                }
            }

        }
        Delay_us(100);
        check = xTaskResumeFromISR(Ultrasonic_1_ReadingHandle);
        portYIELD_FROM_ISR(check);
       }
       GPIO_PORTB_ICR_R|= (1<<2);   GPIO_PORTB_ICR_R|= (1<<3);   GPIO_PORTB_ICR_R|= (1<<4); GPIO_PORTB_ICR_R|= (1<<5);
       GPIO_PORTB_ICR_R|= (1<<0); GPIO_PORTB_ICR_R|= (1<<1);
  }
void SpeedPinAction(void)
{
Counts++;
GPIO_PORTF_ICR_R|= (1<<1);
}
void TimerA1_Action(void)
   {
      TIMER1_ICR_R = TIMER_ICR_TATOCINT;
      THIS_CAR_DATA.speed=((2*3.14*1.3)/160)*Counts;
      Counts=0;
      #ifdef SPEED_PRINT
          Void_Clear_Screen();
          Void_LCD_Print("SPEED=");
          Void_LCD_NUM((int)THIS_CAR_DATA.speed);
      #endif /*SPEED_PRINT*/
   }
void UART2_Action(void)
{
    char Bluetooth_reading=0;
    uint8 left_var=0;uint8 right_var=0;
    previous_Transmission_state=THIS_CAR_DATA.Transmission_state;
    previous_Direction=THIS_CAR_DATA.Direction;
    Bluetooth_reading=(char)(UART2_DR_R&0x00FF);
    #ifdef ULTRASONIC_PRINT
        Void_Clear_Screen();
    #endif /*ULTRASONIC_PRINT*/
    if(Bluetooth_reading=='U'&&THIS_CAR_DATA.bluetooth_speed!=0)
    {
        if(coll[5]==1)
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("     Watch out!     ");
                Void_LCD_Print("       Front        ");
            #endif /*ULTRASONIC_PRINT*/
            THIS_CAR_DATA.Transmission_state='X';
            THIS_CAR_DATA.Direction='N';
            GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
            Timer0A_PWM_Duty(0);
            Timer0B_PWM_Duty(0);
        }
        else
        {
            left_var=0;right_var=0;
            THIS_CAR_DATA.Transmission_state='U';
            THIS_CAR_DATA.Direction='N';
            Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
        }
    }
    else if(Bluetooth_reading=='D'&&THIS_CAR_DATA.bluetooth_speed!=0)
    {
        if(coll[0]==1)
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("     Watch out!     ");
                Void_LCD_Print("        BACK        ");
            #endif /*ULTRASONIC_PRINT*/
            THIS_CAR_DATA.Transmission_state='X';
            THIS_CAR_DATA.Direction='N';
            GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
            Timer0A_PWM_Duty(0);
            Timer0B_PWM_Duty(0);
        }
        else
        {
            left_var=0;right_var=0;
            THIS_CAR_DATA.Transmission_state='D';
            THIS_CAR_DATA.Direction='N';
            GPIO_PORTC_DATA_R|=(1<<5);   GPIO_PORTC_DATA_R&=~(1<<4);
            Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
        }
    }
    else if(Bluetooth_reading=='X')
    {
        if(previous_Transmission_state=='U' && previous_Direction == 'N' && coll[0]==1)
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("Watch out don't stop");
                Void_LCD_Print(" someone is behind  ");
            #endif /*ULTRASONIC_PRINT*/
        }
        else if(previous_Transmission_state=='D' && previous_Direction == 'N' && coll[5]==1)
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("Watch out don't stop");
                Void_LCD_Print(" someone is in front");
            #endif /*ULTRASONIC_PRINT*/
        }
        else
        {
            THIS_CAR_DATA.Transmission_state='X';
            THIS_CAR_DATA.Direction='N';
            GPIO_PORTC_DATA_R&=~(1<<4);    GPIO_PORTC_DATA_R&=~(1<<5);
            Timer0A_PWM_Duty(0);
            Timer0B_PWM_Duty(0);
        }
    }
    else if(Bluetooth_reading=='R'&&THIS_CAR_DATA.bluetooth_speed!=0)
    {
        if(BSW_state==1 && APPs_coll[1]==1)
        {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("     BSW_WARNING    ");
                Void_LCD_Print("   DONT TURN RIGHT  ");
            #endif /*APPLICATIONS_PRINT*/
        }
        else if(DNPW_state==1)
        {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("    DNPW_WARNING    ");
                Void_LCD_Print("   DONT TURN RIGHT  ");
            #endif /*APPLICATIONS_PRINT*/
        }
        else if(coll[1]==1 || coll[3]==1 )
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("     Watch out!     ");
                Void_LCD_Print("  don't turn right  ");
            #endif /*ULTRASONIC_PRINT*/
        }
        else
        {
            left_var=0;right_var=1;
            THIS_CAR_DATA.Direction='R';
            THIS_CAR_DATA.Transmission_state='U';
            if(THIS_CAR_DATA.bluetooth_speed>9)
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                Timer0B_PWM_Duty(2900*0.5*THIS_CAR_DATA.bluetooth_speed);
            }
            else if(THIS_CAR_DATA.bluetooth_speed<9 && THIS_CAR_DATA.bluetooth_speed >4)
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+2));
                Timer0B_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+2));
            }
            else
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+3));
                Timer0B_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+3));
            }
        }
    }
    else if(Bluetooth_reading=='L'&&THIS_CAR_DATA.bluetooth_speed!=0)
    {
        if(BSW_state==1 && APPs_coll[0]==1)
        {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("     BSW_WARNING    ");
                Void_LCD_Print("   DONT TURN LEFT   ");
            #endif /*APPLICATIONS_PRINT*/
        }
        else if(DNPW_state==1)
        {
            #ifdef APPLICATIONS_PRINT
                Void_Clear_Screen();
                Void_LCD_Print("    DNPW_WARNING    ");
                Void_LCD_Print("   DONT TURN RIGHT  ");
            #endif /*APPLICATIONS_PRINT*/
        }
        else if(coll[2]==1 || coll[4]==1 )
        {
            #ifdef ULTRASONIC_PRINT
                Void_LCD_Print("     Watch out!     ");
                Void_LCD_Print("   don't turn left  ");
            #endif /*ULTRASONIC_PRINT*/
        }
        else
        {
            left_var=1;right_var=0;
            THIS_CAR_DATA.Direction='L';
            THIS_CAR_DATA.Transmission_state='U';
            if(THIS_CAR_DATA.bluetooth_speed>9)
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*0.5*THIS_CAR_DATA.bluetooth_speed);
                Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            }
            else if(THIS_CAR_DATA.bluetooth_speed<9 && THIS_CAR_DATA.bluetooth_speed >4)
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+2));
                Timer0B_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+2));
            }
            else
            {
                GPIO_PORTC_DATA_R|=(1<<4);   GPIO_PORTC_DATA_R&=~(1<<5);
                Timer0A_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+3));
                Timer0B_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+3));
            }
        }
    }
    else if(( Bluetooth_reading=='0')||( Bluetooth_reading=='1')||( Bluetooth_reading=='2')||( Bluetooth_reading=='3')||(Bluetooth_reading=='4')||( Bluetooth_reading=='5')||( Bluetooth_reading=='6')||( Bluetooth_reading=='7')||( Bluetooth_reading=='8')||( Bluetooth_reading=='9'))
    {
        /*calculate the distance of the previous speed and add it to the total distance *
          assume the acceleration and deceleration between speed change is 0 and this assumption
          could be valid at 3 seconds interval taking into consideration that we handle the
          the brake state without the distance*/
        time2 = xTaskGetTickCountFromISR();
        my_distance+=((time2-time1)/1000) * THIS_CAR_DATA.speed;
        time2=0;
        time1 = xTaskGetTickCountFromISR();
        THIS_CAR_DATA.bluetooth_speed=Bluetooth_reading-'0'+2;
        if(THIS_CAR_DATA.bluetooth_speed >= 5)
            THIS_CAR_DATA.speed = THIS_CAR_DATA.bluetooth_speed * 6 - 17;
        else
            THIS_CAR_DATA.speed = 0;
        if(left_var==0&&right_var==0)
        {
            Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
        }
        else if (left_var==1&&right_var==0)
        {
            if(THIS_CAR_DATA.bluetooth_speed>9)
            {
                Timer0A_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
                Timer0B_PWM_Duty(2900*0.5*THIS_CAR_DATA.bluetooth_speed);
            }
            else if(THIS_CAR_DATA.bluetooth_speed<9 && THIS_CAR_DATA.bluetooth_speed >4)
            {
                Timer0A_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+2));
                Timer0B_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+2));
            }
            else
            {
                Timer0A_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+3));
                Timer0B_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+3));
            }
        }
        else if (left_var==0&&right_var==1)
        {
            if(THIS_CAR_DATA.bluetooth_speed>9)
            {
                Timer0A_PWM_Duty(2900*0.5*THIS_CAR_DATA.bluetooth_speed);
                Timer0B_PWM_Duty(2900*THIS_CAR_DATA.bluetooth_speed);
            }
            else if(THIS_CAR_DATA.bluetooth_speed<9 && THIS_CAR_DATA.bluetooth_speed >4)
            {
                Timer0A_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+2));
                Timer0B_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+2));
            }
            else
            {
                Timer0A_PWM_Duty(2900*0.5*(THIS_CAR_DATA.bluetooth_speed+3));
                Timer0B_PWM_Duty(2900*(THIS_CAR_DATA.bluetooth_speed+3));
            }
        }
    }
    UART2_ICR_R|=(1<<4);
}
