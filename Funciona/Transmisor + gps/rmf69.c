#include <18F4550.h>
#include <stdlib.h>
#Fuses HS, nowdt, nolvp, novregen, noprotect, nomclr, noPUT, nobrownout
#use delay(clock=20M, crystal=20M) // a 3.3V se transforma en ~3MHZ

#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=GPS,errors)
#USE TIMER(TIMER=1,TICK=1ms,BITS=32,ISR)

//-----------------------------------------
//Definicion pines RFM69
#define _slaveSelectPin Pin_D7
#define SCK             Pin_D6
#define SerOut          Pin_D5
#define SerIn           Pin_D4
#define _interruptPin   Pin_B0
#define GPSPower        Pin_D2

 

//-----------------------------------------
#include "GPS.c" 
// ----------------------------------------
#include "RFM69.h"
// ----------------------------------------
//Variables---------------
//GPS--
char GPSData[128];
unsigned int8 GPSDataPtr=0; 
char c; 
int8 GPSDataReady = FALSE; 
int8 GPSmsgCount = 0; 
//RMF69--
byte TX_DATA[61];
//AUX---
//static unsigned int32 Cont = 0;
//short GPS_Status = 1;
//int tmr0 = 0;

//--------------------------------------
//Funciones



void GPS_Pack()
{
   for(int i=0; i<61; i++)
   {
      GPSData[i]=GPSData[i+7];
   }
}

//------------------------
//INT Lectura del UART GPS
#INT_RDA
void RDA_isr(void) 
{ 
   c = getc(); 
   switch (c) 
   { 
      case '$': 
         GPSDataPtr = 0; 
      break; 
      
      case '\n': 
         GPSmsgCount++;
           if ( (StrFnd(GPSData, 'A', 0) == 17) && (GPSData[0]=='$') && (GPSData[1]== 'G') && (GPSData[2]== 'P')
         && (GPSData[3]== 'R')&& (GPSData[4]== 'M') && (GPSData[5]== 'C'))
         {
            GPSDataReady = TRUE;
         }
      break; 
   } 
    
   GPSData[GPSDataPtr++ & 0x7F] = c; 
} 
//---------------------------------------
//INT timer0
/*#INT_TIMER0
void T0inter(void)
{

   if((cont == 1400)        //1seg ~> cont = 12; 1min ~> cont = 700 
      && (GPS_Status == 0))
   {
      Output_high(GPSPower);
      cont = 0;
      GPS_Status = 1;
      break;
   }
   else
      cont++;
}*/

void main(void)
{ 
#zero_ram

set_tris_b(0b00000001);     //DIO0
set_tris_C(0b10000000);     // ser in 0
set_tris_D(0b00010000);

//--------------------------------------
//Config RFM69--------------------------
   Output_High(_slaveSelectPin);
   Output_Low(SCK);
   initialize(2);
   delay_ms(500);
   setHighPower(1);
   //readAllRegs();
//--------------------------------------
//Config GPS
   //GPRMCInfo MyGPRMCInfo;
//--------------------------------------
   //setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256|RTCC_8_bit); //86ms ~ RTCC_DIV_256|RTCC_8_bit
  // set_timer0(tmr0);                                     //86ms ~ tmr0=0;
 
//1seg ~> cont = 12; 1min ~> cont = 700 
//------------------------------------------------------------ 
   Output_high(GPSPower);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
//   enable_interrupts(INT_TIMER0);

   

// --------------------------------------------------------------
while(true)
   {
      if (GPSDataReady)
      { 
         //GPRMC_decode(GPSData, &MyGPRMCInfo);
         //if (MyGPRMCInfo.Valid == 'A') 
         //{ 
            GPS_Pack();
            sprintf(TX_DATA, "%s", GPSData);
           // for (int i = 0; i < 2; i ++)
            //{
               send(0x01, TX_DATA, 61, 0);
               delay_ms(100);
            //}
            
         //}
         Output_low(GPSPower);
         delay_ms(20000);
         Output_high(GPSPower);
         GPSDataReady = FALSE; 
      }  
         

   }
   // End While Loop
}         // End Main 


/*****************************************************************************/











