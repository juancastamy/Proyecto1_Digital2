/*
 * Second lab for Digital 2 course
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
 * Created on March 10, 2020
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
#include "OSCI.h"
#include "SPI_SLAVE.h"
#define _XTAL_FREQ 8000000

char Luz;
char Temp;
char Parq;
char ULTRA;
char Presion;
char RASPBERRY;
char S0;
void setup(void);
void __interrupt() ISR(void){
       
    if(PIR1bits.SSPIF==1){
        RASPBERRY = spiRead();
        
        if(RASPBERRY == 0){
          SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 1){
            SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 2){
            SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 3){
            SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          return;
        }
        if(RASPBERRY == 4){
           SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          return;
        }
       
          SSPSTATbits.BF= 0;
          PIR1bits.SSPIF = 0;
          
           
         
           return;
    }
    if(PIR1bits.RCIF == 1){
        if(S0 == 0){
        Luz= UART_READ();
        PIR1bits.RCIF = 0;
        S0=1;
        return;
        }
         if(S0 == 1){
        Temp= UART_READ();
        PIR1bits.RCIF = 0;
        S0=2;
        return;
        }
         if(S0 == 2){
        Parq = UART_READ();
        PIR1bits.RCIF = 0;
        S0=3;
        return;
        }
         if(S0 == 3){
        ULTRA= UART_READ();
        PIR1bits.RCIF = 0;
        S0=4;
        return;
        }
         if(S0 == 4){
        Presion= UART_READ();
        PIR1bits.RCIF = 0;
        S0 = 0;
        return;
        }
    }
   
    return;
   
}
void main(void) {
    setup();
    initOsc(7);
    UART_INIT(9600);
    while(1){

        
        
    UART_WRITE(Luz);
    __delay_ms(1);
    UART_WRITE(Temp);
    __delay_ms(1);
    UART_WRITE(Parq);
    __delay_ms(1);
    UART_WRITE(ULTRA);
    __delay_ms(1);
    UART_WRITE(Presion);
    __delay_ms(1);
    }
    return;
}

void setup (void){
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b11010000;
    TRISD = 0;
    TRISDbits.TRISD1 = 0;
    ANSEL = 0;
    ANSELH = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    RASPBERRY = 0;
    S0=0;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}