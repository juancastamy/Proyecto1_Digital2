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
#include "I2C_SLAVE3.h"
#define __XTAL_FREQ 4000000

uint8_t vehiculos;
uint8_t z;
char S1=0;
char S2=0;
char S3=0;
char S4=0;
char S5=0;
char S6=0;
char S7=0;
char S8=0;
void setup(void);
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
            SSPBUF = vehiculos;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }

        PIR1bits.SSPIF = 0;    
    }
}
void main(void) {
    initOsc(6);
    setup();
    while(1){
        //PORTD = ~PORTAbits.RA0;
        if(PORTAbits.RA0==1 && S1==0){
            vehiculos++;
            S1= 1;
            PORTBbits.RB0=1;
        }
        if(PORTAbits.RA0==0 && S1==1){
            vehiculos--;
            S1=0;
            PORTBbits.RB0 = 0;
        }
        if(PORTAbits.RA1==1 && S2==0){
            vehiculos++;
            S2= 1;
            PORTBbits.RB1=1;
        }
        if(PORTAbits.RA1==0 && S2==1){
            vehiculos--;
            S2=0;
            PORTBbits.RB1 = 0;
        }
        if(PORTAbits.RA2==1 && S3==0){
            vehiculos++;
            S3= 1;
            PORTBbits.RB2=1;
        }
        if(PORTAbits.RA2==0 && S3==1){
            vehiculos--;
            S3=0;
            PORTBbits.RB2 = 0;
        }
        if(PORTAbits.RA3==1 && S4==0){
            vehiculos++;
            S4= 1;
            PORTBbits.RB3=1;
        }
        if(PORTAbits.RA3==0 && S4==1){
            vehiculos--;
            S4=0;
            PORTBbits.RB3 = 0;
        }
        if(PORTAbits.RA4==1 && S5==0){
            vehiculos++;
            S5= 1;
            PORTBbits.RB4=1;
        }
        if(PORTAbits.RA4==0 && S5==1){
            vehiculos--;
            S5=0;
            PORTBbits.RB4 = 0;
        }
        if(PORTAbits.RA5==1 && S6==0){
            vehiculos++;
            S6= 1;
            PORTBbits.RB5=1;
        }
        if(PORTAbits.RA5==0 && S6==1){
            vehiculos--;
            S6=0;
            PORTBbits.RB5 = 0;
        }
        if(PORTAbits.RA6==1 && S7==0){
            vehiculos++;
            S7= 1;
            PORTBbits.RB6=1;
        }
        if(PORTAbits.RA6==0 && S7==1){
            vehiculos--;
            S7=0;
            PORTBbits.RB6 = 0;
        }
        if(PORTAbits.RA7==1 && S8==0){
            vehiculos++;
            S8= 1;
            PORTBbits.RB7=1;
        }
        if(PORTAbits.RA7==0 && S8==1){
            vehiculos--;
            S8=0;
            PORTBbits.RB7 = 0;
        }
    }
    return;
}
void setup(void){
    TRISA=0b11111111;
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
    I2C_Slave_Init(0x10);
}