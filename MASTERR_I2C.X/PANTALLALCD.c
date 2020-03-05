/* 
 * File:   LCD.c
 * Author: CHARLIE
 *
 * Created on 14 de febrero de 2020, 12:50
 */


#include "PANTALLALCD.h"
#include <xc.h>
#include <pic16f887.h>
#define _XTAL_FREQ  8000000
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void lcd_cmd(unsigned char x){
    lcd_ready();
    LCD = x;
    RS = 0;
    RW = 0;
    lcd_lat();  
    return;
}

void lcd_lat(void){
    EN = 1;
   __delay_ms(30);
   EN = 0;
   return;
}
void lcd_ready(void) {
    LCD = 0xFF;
    LCD &= 0x80;
    RS = 0;
    RW = 1;
    EN = 0;
    __delay_ms(30);
    EN = 1;
    if (LCD == 0x80){
    EN = 0;
    __delay_ms(30);
    EN = 1;  
    }
    else{   
    }  
    return;
}

void lcd_dwr(unsigned char x){
    lcd_ready();
    LCD = x;
    RS = 1;
    RW = 0;
    lcd_lat(); 
    return;
}

void lcd_msg(unsigned char *c){
    while(*c != 0)
        lcd_dwr(*c++);
    return;
}

void inicializacion(void){   
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x06);
    lcd_cmd(0x80);  
    return;
}

