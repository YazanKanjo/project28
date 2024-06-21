#include <xc.h>
#define SBIT_ADON     0
#define SBIT_CHS0     3
#define SBIT_ADFM     7
void delay_adc(int cnt)
{
    while(cnt--);
}
void ADC_Init()
 {
   ADCON0=0x00;  
   ADCON1=(1<<SBIT_ADFM);  
}
int ADC_Read(int adcChannel)
 {  
    ADCON0 = (1<<SBIT_ADON) | (adcChannel<SBIT_CHS0);  

    delay_adc(1000);                   
   
    GO=1;                           // the ADC conversion will start
    while(GO_DONE==1);              // Wait for it to complete
                                    
    return((ADRESH<<8) + ADRESL);   // return right justified 10-bit result
 }
