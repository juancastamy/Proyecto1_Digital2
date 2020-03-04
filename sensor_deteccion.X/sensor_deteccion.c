/*
 * Second lab for Digital 2 course
 * File:   sensor_deteccion.c
 * Author: Juan Diego Castillo Amaya
 * Student ID: 17074
 *
 * Created on March 4, 2020
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "OSCILADOR.h"
#include <stdint.h>
#define __XTAL_FREC 4000000

uint8_t vehiculos = 8;
char S1=0;
void setup(void);
void main(void) {
    initOsc(6);
    setup();
    while(1){
        //PORTD = ~PORTAbits.RA0;
        if(PORTAbits.RA0==1&&S1==0){
            S1= 1;
            PORTBbits.RB0=0;
        }
        if(PORTAbits.RA0==0 && S1==1){
            vehiculos--;
            S1=0;
            PORTBbits.RB0 = 1;
        }
        
//        if(PORTAbits.RA0==1 && S1==2){
//            vehiculos++;
//            PORTBbits.RB0 = 0;
//            S1=0;
//        }
        
        
    }
    return;
}
void setup(void){
    TRISA=1;
    TRISB=0;
    TRISC=0;
    TRISD=0;
    TRISE=0;
    ANSEL=0;
    ANSELH=0;  
    
    PORTA=0;
    PORTB=0;
    PORTC=0;
    PORTD=0;
    PORTE=0;
}