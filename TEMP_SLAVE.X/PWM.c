#include "PWM.h"
void PWM_INIT(void){
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
