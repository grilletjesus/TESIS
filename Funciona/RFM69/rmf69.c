#include <18F4550.h>
#include <stdlib.h>
#Fuses HS, nowdt, nolvp, vregen, noprotect, nomclr, PUT, nobrownout
#use delay(clock=20M, crystal=20M)

#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=GPS,errors)
#USE TIMER(TIMER=1,TICK=1ms,BITS=32,ISR)


#define _slaveSelectPin Pin_D7
#define SCK             Pin_D6
#define SerOut          Pin_D5
#define SerIn           Pin_D4
#define _interruptPin   Pin_B0



// ----------------------------------------
#include "RFM69.h"
// ----------------------------------------

byte TX_DATA[61];

void main(void)
{ 
#zero_ram

set_tris_C(0b00010001);     // ser in 0
set_tris_D(0b00010000);


Output_High(_slaveSelectPin);
Output_Low(SCK);

//LCD_init();


initialize(2);

delay_ms(500);

setHighPower(1);

readAllRegs();



// --------------------------------------------------------------
while(true)
   {
      
      sprintf(TX_DATA, "%s", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
      send(0x01, TX_DATA, 63, 0);
      printf("%s", TX_DATA);
      
      delay_ms(100);
   }
   // End While Loop
}         // End Main 


/*****************************************************************************/











