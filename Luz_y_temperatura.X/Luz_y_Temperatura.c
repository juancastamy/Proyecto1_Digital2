/*
 * Project 1 for Digital 2 course
 * File:   Project 1
 * Author: Juan Diego Castillo Amaya
 * Student ID: 17074
 * 
 * Author: Carlos Avandaño
 * Student ID: 17192
 *
 * Author: Juan Pablo Zea
 * Student ID: 15401
 * Created on March 3, 2020
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
#include "I2C.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "OSCILADOR.h"
#include "ADC.h"

#define _XTAL_FREQ 8000000

#define TRIS_DHT TRISDbits.TRISD2
#define PORT_DHT PORTDbits.RD2

uint8_t z;
unsigned char check;


unsigned char T_byte1;
unsigned char T_byte2;
unsigned char RH_byte1;
unsigned char RH_byte2;

int uniT;
int decT;
int uniHR;
int decHR;

int uniT_int;
int decT_int;
int uniHR_int;
int decHR_int;

char uniT_char[5];
char decT_char[5];
char uniHR_char[5];
char decHR_char[5];

char S0=0;

unsigned sum; 

void SETUP(void);
void START_DHT11(void);
void CHECK_RESPONSE(void);
char ReadData();



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

        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
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
            SSPBUF = T_byte1;
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
    SETUP();
    initOsc(7);
    ADCSETUP();
    while(1){
        ADC1();
        if(adc>=17){
            PORTDbits.RD1=0;
        }
        if(adc<=8){
            PORTDbits.RD1=1;
        }
        
         START_DHT11();
        CHECK_RESPONSE();
        
        if(check == 1){
        
            RH_byte1 = ReadData();
            RH_byte2 = ReadData();
            T_byte1 = ReadData();
            T_byte2 = ReadData();
            sum = ReadData();
            
            if(sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2) & 0xFF)){
              decHR = RH_byte1/10;
              itoa(decHR_char,decHR,10);
              uniHR = RH_byte1%10;
              itoa(uniHR_char,uniHR,10);
              decT = T_byte1/10;
              itoa(decT_char,decT,10);
              uniT = T_byte1%10;
              itoa(uniT_char,uniT ,10);
              strcat(decHR_char,uniHR_char);
              strcat(decT_char,uniT_char);
            }
        }
        if (T_byte1 > 27){
            PORTBbits.RB0 = 1;
        }
        else{
            PORTBbits.RB0 = 0;
        }
    }   
}



void SETUP (void){
    TRISA = 0b00000001;
    TRISB = 0;
    TRISC = 0b00011000;
    TRISD = 0;
    TRISDbits.TRISD1 = 0;
    TRISE = 0;
    ANSEL = 0b00000001;
    ANSELH = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTA = 0;
    I2C_Slave_Init(0x30); //Initialize as a I2C Slave with address 0x20
}

 void START_DHT11(void){  // inicia el sensor de temperatura
     TRIS_DHT = 0;
     PORT_DHT = 0;
     __delay_ms(18);
     PORT_DHT = 1;
     __delay_us(30);
     TRIS_DHT=1;  
 }
 
 void CHECK_RESPONSE(void){
   check = 0;
   __delay_us(40);
   if(PORT_DHT == 0){
       __delay_us(80);
       if(PORT_DHT == 1){         // mantener el ojo en este if
           check = 1;
           __delay_us(40);
       }
   }
 }
 
 char ReadData(){
     char i, j;
     for(j = 0; j<8; j++){
         while(!PORT_DHT);
         __delay_us(30);
         if(PORT_DHT == 0)
             i&= ~(1<<(7-j));
         else{
             i|= (1<< (7-j));
         while(PORT_DHT);
         }  
     }
     return i;
 }