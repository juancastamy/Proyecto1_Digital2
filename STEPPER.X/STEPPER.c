/*
 * File:   STEPSPI.c
 * Author: CHARLIE
 *
 * Created on 12 de marzo de 2020, 12:22
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
#include "UART.h"
#define BOTON RA0
#define _XTAL_FREQ 8000000
void LOOP(void);
void Abrir(void);
void Cerrar (void);
void cicloAbrir(void);
void cicloCerrar (void);
char S0 = 0;
char S1 = 0;
char S3=0;
char S4=0;
char S5=0;
char S6=0;
char i = 0;
char P1=0;
uint8_t P2;
void __interrupt() ISR(void){
    if(PIR1bits.RCIF==1){
        PORTE = UART_READ();
        P1=PORTE;
        PIR1bits.RCIF=0;
    }
    return;
}

void main(void) {
  OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1;
    UART_INIT(9600);
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    TRISA = 0b00000001;
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    TRISE = 0b0000;
    ANSEL = 0b00000000;
    ANSELH= 0b00000000;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTA = 0;
   
    LOOP();
           
   
   
 
}

void LOOP(void){
    while(1){
    Abrir();
    cicloAbrir();
    Cerrar();
    cicloCerrar();
   }
    }

   
   
   
       
       
       

   



void Abrir(void){
    if (P1 == 1 && S0 == 0){
        for (i=0; i<=128;i++){
            if (i<=128){
                PORTB = 0b00000001;
                __delay_ms(2);
                PORTB = 0b00000010;
                __delay_ms(2);
                PORTB = 0b00000100;
                __delay_ms(2);
                PORTB = 0b00001000;
                __delay_ms(2);
            }
        }
        S0=1;
        i=0;
        S3=1;
    }
    else{
        PORTB=0;
    }
    return;
}
void cicloAbrir (void){
    if (S3==1 && S0 == 1){
        for (S4=0;S4<=4;S4++){
        for (i=0; i<=128;i++){
            if (i<=128){
                PORTB = 0b00000001;
                __delay_ms(2);
                PORTB = 0b00000010;
                __delay_ms(2);
                PORTB = 0b00000100;
                __delay_ms(2);
                PORTB = 0b00001000;
                __delay_ms(2);
            }
        }
        S0=1;
        i=0;
        S3=0;
    }
        S4=0;
    }
    else{
        PORTB=0;
    }
    return;
}
    


void Cerrar (void){
    if (P1==0 && S0==1 && BOTON==1){
        S1=1;
    }
    if (P1==0 && S0==1 && S1==1 && BOTON==0){
        S1=0;
        for (i=0; i<=128;i++){
            if (i<=128){
                PORTB = 0b00001000;
                __delay_ms(2);
                PORTB = 0b00000100;
                __delay_ms(2);
                PORTB = 0b00000010;
                __delay_ms(2);
                PORTB = 0b00000001;
                __delay_ms(2);
            }
        } 
        S0=1;
        i=0;
        S5=1;
    }
    return;
}

void cicloCerrar(void){
    if (S5==1 && S0 == 1){
        for (S6=0;S6<=4;S6++){
        for (i=0; i<=128;i++){
            if (i<=128){
                PORTB = 0b00001000;
                __delay_ms(2);
                PORTB = 0b00000100;
                __delay_ms(2);
                PORTB = 0b00000010;
                __delay_ms(2);
                PORTB = 0b00000001;
                __delay_ms(2);
            }
        }
        S0=0;
        i=0;
        S5=0;
    }
        S6=0;
    }
    else{
        PORTB=0;
    }
    return;
}