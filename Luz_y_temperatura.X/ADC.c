
#include "ADC.h"
void ADCSETUP(void){
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADON = 1;   // adc on
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
}
void ADC1 (void){
    __delay_ms(1);
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    __delay_us(600);
    ADCON0bits.ADON = 1;
    PIR1bits.ADIF = 0;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1);
    adc = ADRESH;        
}


