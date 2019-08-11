/*
 * File:   newmain.c
 * Author: Alex
 *
 * Created on 2 July 2019, 7:15 PM
 */

//remember - all unused pins should be set as outputs, or use pulldown resistors on them!

#include <xc.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC=INTRCIO, WDTE=OFF, PWRTE=OFF, MCLRE=ON, CP=OFF, \
                CPD=OFF, BOREN=OFF, IESO=OFF, FCMEN=OFF

#define AR  PORTCbits.RC5
#define AY  PORTCbits.RC4
#define AG  PORTCbits.RC3
#define BR  PORTCbits.RC0
#define BY  PORTCbits.RC1
#define BG  PORTCbits.RC2
#define button PORTAbits.RA2

int state;

//interrupt enabled only in this state
void _state0()
{
    INTCONbits.GIE = 1; 
    INTCONbits.INTF = 0;
    AR = 0;
    AY = 0;
    AG = 1;
    BR = 1;
    BY = 0;
    BG = 0;
    __delay_ms(10000);
    state = 1;
    
    return;
}

void _state1()
{
    INTCONbits.GIE = 0;
    INTCONbits.INTF = 0;
    AR = 0;
    AY = 1;
    AG = 0;
    BR = 1;
    BY = 0;
    BG = 0;
    __delay_ms(3000);
    
    state = 2;
    
    return;
}

void _state2()
{
    INTCONbits.GIE = 0;
    INTCONbits.INTF = 0;
    AR = 1;
    AY = 0;
    AG = 0;
    BR = 1;
    BY = 0;
    BG = 0;
    __delay_ms(1000);
    
    state = 3;
    
    return;
}

void _state3()
{
    INTCONbits.GIE = 0;
    INTCONbits.INTF = 0;
    AR = 1;
    AY = 0;
    AG = 0;
    BR = 0;
    BY = 0;
    BG = 1;
    __delay_ms(10000);
    
    state = 4;
    
    return;
}

void _state4()
{
    INTCONbits.GIE = 0;
    INTCONbits.INTF = 0;
    AR = 1;
    AY = 0;
    AG = 0;
    BR = 0;
    BY = 1;
    BG = 0;
    __delay_ms(3000);
    
    state = 5;
    
    return;
}

void _state5()
{
    INTCONbits.GIE = 0;
    INTCONbits.INTF = 0;
    AR = 1;
    AY = 0;
    AG = 0;
    BR = 1;
    BY = 0;
    BG = 0;
    __delay_ms(1000);
    
    state = 0;
    
    return;
}

void interrupt isr()
{
    INTCONbits.INTF = 0;
  
    for (int i = 0; i < 5; i++)
    {
        AR = 0;
        AY = 1;
        AG = 0;
        BR = 0;
        BY = 1;
        BG = 0;
        __delay_ms(250);
        AR = 0;
        AY = 0;
        AG = 0;
        BR = 0;
        BY = 0;
        BG = 0;
        __delay_ms(250);
    }

    state = 0;
    _state0();
}

int main()
{
    ANSEL = 0x00;
    TRISA = 0b00000100; //pin RA2 (external interrupt) is input, others on port A output (unused)
    TRISB = 0b00000000; //port B 
    TRISC = 0b00000000; //port C 
    
    INTCONbits.INTF = 0;        //reset the external interrupt flag
    OPTION_REGbits.INTEDG = 1;  //interrupt on the rising edge
    INTCONbits.INTE = 1;        //enable the external interrupt
    INTCONbits.GIE = 1;         //set the Global Interrupt Enable
    
    state = 0;
    
    while(1)
    {
        if (state == 0)
        {
         _state0();
        }
        else if (state == 1)
        {
         _state1();
        }
        else if (state == 2)
        {
         _state2();
        }
        else if (state == 3)
        {
         _state3();
        }
        else if (state == 4)
        {
         _state4();
        }
        else if (state == 5)
        {
         _state5();
        }
    }
    
    return 0;
}
