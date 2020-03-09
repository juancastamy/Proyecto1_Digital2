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

uint8_t z;
char ADC;
void LOOP(void);
void ANALOGICO(void);

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
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.OSTS= 0;
    OSCCONbits.HTS = 0;
    OSCCONbits.LTS = 0;
    OSCCONbits.SCS = 1; 
    
    TRISA = 0b00000001;
    TRISB = 0;
    TRISC = 0b00011000;
    TRISD = 0;
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
    I2C_Slave_Init(0x30); //Initialize as a I2C Slave with address 0x20
  
      LOOP();
  
}

void LOOP(void){
    while(1){
        ANALOGICO();
    }
    
}

void ANALOGICO(void){
    __delay_ms(1);
        ADCON0bits.CHS = 0000;
        ADCON0bits.ADON = 1;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
       ADC = ADRESH;
       PORTB = ADC;
       return;
}