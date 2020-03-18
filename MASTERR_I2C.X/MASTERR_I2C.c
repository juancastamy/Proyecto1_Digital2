/*
 * File:   MASTERR_I2C.c
 * Author: CHARLIE
 *
 * Created on 26 de febrero de 2020, 22:03
 */



// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PANTALLALCD.h"
#include "I2C.h"
#include "UART.h"
#define _XTAL_FREQ  8000000
#define BOTON RA0

uint8_t analogic_digital_a;
uint8_t CONTADOR;
uint8_t analogic_digital_b;
uint8_t SONICO;
float voltage;
float luz;
int CONTLUZA;
int CONTLUZB;
char CONTLUZA_CHAR[5];
char CONTLUZB_CHAR[5];

int uniT;
int decT;
int uniHR;
int decHR;

int DC2;
int LUZ_1_A;
int LUZ_1_B;
int LUZ_A;
char LUZA_CHARA[5];
char LUZB_CHARB[5];
char LUZC_CHARC[5];
char POINTERB[5];
char SONICOD_CHAR[5];
char SONICOU_CHAR[5];
int CONTD;
int SONICOD;
char CONTD_CHAR[5];
int CONTU;
int SONICOU;
char CONTU_CHAR[5];
float voltage;
int DC1;
int ADC_1_A;
int ADC_1_B;
int ADC_A;
char ADCA_CHARA[5];
char ADCB_CHARB[5];
char ADCC_CHARC[5];
char POINTERA[5];
char T_byte1;
char S0;
char S1;
int uniT_int;
int decT_int;
char uniT_char[5];
char decT_char[5];

void SETUP(void);
void main(void) {
    SETUP();
    I2C_INIT(100000);
    inicializacion();
    UART_INIT(9600);
    
    while(1){
        //-------------------------------------SENSOR DE LUZ----------------------------------------------
        I2C_Master_Start();         //Start condition
        I2C_Master_Write(0x31);     //7 bit address + Read
        analogic_digital_b = I2C_Master_Read(0); //Read + Acknowledge
        I2C_Master_Stop();          //Stop condition
        __delay_ms(100);
        UART_WRITE(analogic_digital_b);
        __delay_ms(100);
        //------------------------------CONVERCION DE VARIABLES PARA PANTALLA LCD-------------------------
        CONTLUZA = analogic_digital_b/10;
        itoa(CONTLUZA_CHAR,CONTLUZA,10);
        CONTLUZB = analogic_digital_b%10;
        itoa(CONTLUZB_CHAR,CONTLUZB,10);
        strcat(CONTLUZA_CHAR,CONTLUZB_CHAR);
        
        //------------------------------------SENSOR DE TEMPERATURA---------------------------------------
        I2C_Master_Start();         //Start condition
        I2C_Master_Write(0x31);     //7 bit address + Read
        T_byte1 = I2C_Master_Read(0); //Read + Acknowledge
        I2C_Master_Stop();          //Stop condition
        __delay_ms(100);
        UART_WRITE(T_byte1);
        __delay_ms(100);
        //------------------------------CONVERCION DE VARIABLES PARA PANTALLA LCD-------------------------
        decT = T_byte1/10;
        itoa(decT_char,decT,10);
        uniT = T_byte1%10;
        itoa(uniT_char,uniT ,10);
        strcat(decT_char,uniT_char);
        
        //-------------------------------------SENSOR DE PARQUEOS-------------------------------------------
        I2C_Master_Start();         //Start condition
        I2C_Master_Write(0x11);     //7 bit address + Read
        CONTADOR = I2C_Master_Read(0); //Read + Acknowledge
        I2C_Master_Stop();          //Stop condition
        __delay_ms(100);
        UART_WRITE(CONTADOR);
        __delay_ms(100);
        //------------------------------CONVERCION DE VARIABLES PARA PANTALLA LCD-------------------------
        CONTD = CONTADOR/10;
        itoa(CONTD_CHAR,CONTD,10);
        CONTU = CONTADOR%10;
        itoa(CONTU_CHAR,CONTU,10);
        
        //----------------------------------SENSOR DE PRESION---------------------------------------------
        I2C_Master_Start();         //Start condition
        I2C_Master_Write(0x21);     //7 bit address + Read
        analogic_digital_a = I2C_Master_Read(0); //Read + Acknowledge
        I2C_Master_Stop();          //Stop condition
        __delay_ms(100);
        UART_WRITE(analogic_digital_a);
        __delay_ms(100);
        //------------------------------CONVERCION DE VARIABLES PARA PANTALLA LCD-------------------------
         DC1 = analogic_digital_a/10;
        itoa(ADCA_CHARA,DC1,10);
        DC2 = analogic_digital_a%10;
        itoa(ADCB_CHARB,DC2,10);
        strcat(ADCA_CHARA,ADCB_CHARB);
        
        //-----------------------------SENSOR ULTRASONICO------------------------------------------------
        I2C_Master_Start();         //Start condition
        I2C_Master_Write(0x21);     //7 bit address + Read
        SONICO = I2C_Master_Read(0); //Read + Acknowledge
        I2C_Master_Stop();          //Stop condition
        __delay_ms(100);
        UART_WRITE(SONICO);
        __delay_ms(100);
        //------------------------------CONVERCION DE VARIABLES PARA PANTALLA LCD-------------------------
        SONICOD = SONICO/10;
        itoa(SONICOD_CHAR,SONICOD,10);
        SONICOU = SONICO%10;
        itoa(SONICOU_CHAR,SONICOU,10);
        
        
        //-----------------------------PRIMER SETUP DE PANTALLA LCD PARA DESPLEGAR------------------------
        if(BOTON==0){
            if(S0==1){
            lcd_cmd(0x01);
            S0=0;
            }
        lcd_cmd(0x80); 
        lcd_msg("TEMP CARROS LUZ ");
        lcd_cmd(0xC0);
        lcd_msg(" ");
        lcd_msg(decT_char);
        lcd_msg("    ");
        lcd_msg(CONTU_CHAR);
        lcd_msg("    ");
        lcd_msg(CONTLUZA_CHAR);
        lcd_msg(" ");
        }    
        
        //-----------------------------SEGUNDO SETUP DE PANTALLA LCD PARA DESPLEGAR------------------------
        if(BOTON == 1){
            if(S0==0){
            lcd_cmd(0x01);
            S0=1;
            }
            lcd_cmd(0x80); 
            lcd_msg(" FUERZA ULTRA   ");
            lcd_cmd(0xC0);
            lcd_msg("  ");
            lcd_msg(ADCA_CHARA);
            lcd_msg("     ");
            lcd_msg(SONICOU_CHAR);
            lcd_msg("      ");
        }
    }
}

void SETUP (void){
     TRISA=0b00000001;
    TRISB=0;
    TRISC=0b000011000;
    TRISD=0;
    TRISE=0;
    ANSEL = 0b00000000;
    ANSELH =0b00000000;
    PORTA=0;
    PORTB=0;
    PORTC=0;
    PORTD=0;
    PORTE=0;
}