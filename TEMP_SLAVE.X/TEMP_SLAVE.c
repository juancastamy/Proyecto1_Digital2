/*
 * File:   SLAVE_I2C.c
 * Author: CHARLIE
 *
 * Created on 26 de febrero de 2020, 17:54
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
#include "TEMP_SENSE.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define  TRIGGER RD0
#define  ECHO    RD1

uint8_t z;
char ADC;
void LOOP(void);
void ANALOGICO(void);
void PWM (void);
char ultrasonico;
char distance;
char dist = 0;
char S1 =0;
int calc_distance(void);
void LOOP(void);


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
            SSPBUF = ADC;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }

        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    OSCCONbits.IRCF = 0b100; //8Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1; 
    
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

            
    ADCON0bits.ADCS = 01;
    ADCON0bits.ADON = 1;   // adc on
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    I2C_Slave_Init(0x20); //Initialize as a I2C Slave with address 0x20
    PWM();
    LOOP();
  
}

void LOOP(void){
    while(1){
        ANALOGICO();
        if(ultrasonico == 0x0f){
            CCPR1L = 25;
            S1= 1;
        }
        if(ADC>=5 & S1 ==1){
            S1=0;
        }
        if(ADC==0 & S1==0){
            CCPR1L = 10;
        }
       
    dist = calc_dist()/5;
   
    if(dist>0){
    ultrasonico = 0x00;
    }
    else
    {ultrasonico = 0x0F;}
    }
    
}

void ANALOGICO(void){
    __delay_ms(1);
    ADCON0bits.CHS = 0000;
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    ADC = ADRESH;
    return;
}



void PWM(void){
    // inicializacion de PWM
    TRISCbits.TRISC2 = 1;       // CCP1 entrada
    PR2 = 200;                  //valor para periodo de 20ms
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;
    
   
    CCPR1L = 27;
    CCP1CONbits.DC1B0 = 1;
    CCP1CONbits.DC1B1 = 1;
    
  
    
    PIR1bits.TMR2IF = 0;        // se limpea la bandera del TMR2
    
    T2CONbits.T2CKPS = 0b11;    //se coloca el Prescaler 16
    T2CONbits.TMR2ON = 1;       //se enciende el TRM2
    while(!TMR2IF);
    TMR2IF = 0;                //SE LIEMPIA LA BANDERA DEL TRM2
    TRISC2=0;                  //PORTC 2 SE DECLARA COMO SALIDA
    return;
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