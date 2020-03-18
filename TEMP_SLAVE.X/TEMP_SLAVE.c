/*
 * Project 1 Digital 2 course
 * File:   lab_4.c
 * Author: Juan Diego Castillo Amaya
 * Student ID: 17074
 *
 * Author: Carlos Avendaño
 * Student ID: 17192
 * 
 * Author: Juan Pablo Zea
 * Student ID: 15401
 * 
 * Created on March 9, 2020
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

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TEMP_SENSE.h"
#include "OSCILADOR.h"  
#include "ADC.h"
#include "PWM.h"
#define  TRIGGER RD0
#define  ECHO    RD1

uint8_t z;

char ultrasonico;
char distance;
char dist = 0;
char S0 = 0;
char S1 = 0;
char S2 = 0;


void SETUP(void);
int calc_distance(void);

void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;

        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);

        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if(S0==0){
            SSPBUF = adc;
            S0 = 1;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
            PIR1bits.SSPIF = 0;
            return;
            }
            if(S0==1){
            SSPBUF = dist;
            S0 = 0;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
            PIR1bits.SSPIF = 0; 
            return;
            }
        }
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    initOsc(4);
    SETUP();
    ADCSETUP();
    I2C_Slave_Init(0x20); //Initialize as a I2C Slave with address 0x20
    PWM_INIT();
    while(1){
        ADC1();
        //---------------FUNCION PARA ABRIR---------------------------------
        if(ultrasonico == 0x0f & CCPR1L == 10){
            CCPR1L = 25;
            S1= 1;
        }
        if(adc>=5 & S1 ==1 & CCPR1L == 25){
            S1=0;
        }
        if(adc==0 & S1==0 & CCPR1L == 25){
            CCPR1L = 10;
        }
        //----------------FUNCION PARA CERRAR------------------------------
        if(adc>= 5 & CCPR1L == 10){
            CCPR1L = 25;   
        }
        if(ultrasonico == 0x0f & S2==1 & CCPR1L == 25){
            S2 = 1;
        }
        if (ultrasonico == 0x00 & S2 == 1 & CCPR1L == 25){
            CCPR1L = 10;
            S2 = 0;
        }
        
        dist = calc_dist()/5;

        if(dist>0){
        ultrasonico = 0x00;
        }
        else
        {ultrasonico = 0x0F;}
        }
  
}

void SETUP(void){
    TRISA = 0b00000001;
    TRISB = 0;
    TRISC = 0b00011000;
    TRISD = 0b00000010;
    TRISE = 0;
    ANSEL = 0b00000001;
    ANSELH = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTA = 0;
    S0 = 0;
    S1 = 0;
    S2 = 0;
}

int calc_dist(void){
    int distance=0;
    TMR1=0;
    // Send Trigger Pulse To The Sensor
    TRIGGER=1;
    __delay_us(10);
    TRIGGER=0;
    // Wait For The Echo Pulse From The Sensor
    while(!ECHO);
    // Turn ON Timer Module & Disable Interrupts

    TMR1ON=1;
    // Wait Until The Pulse Ends
    while(ECHO);
    // Turn OFF The Timer & Re-Enable The Interrupts
    TMR1ON=0;

    // Calculate The Distance Using The Equation
    distance=TMR1/58.82;
    return distance;
}