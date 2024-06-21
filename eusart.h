#ifndef _EUSART1_H
#define _EUSART1_H
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus  
#endif
#define EUSART1_DataReady  (PIR1bits.RC1IF)
void EUSART1_Initialize(void);
uint8_t EUSART1_Read(void);
void EUSART1_Write(uint8_t txData);
void put_string(const unsigned char* str);
void UART_RxString(char *string_ptr);
#ifdef __cplusplus  
#endif
#endif 
