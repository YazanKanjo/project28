#include <xc.h>                    // Include the XC header file
#include <stdint.h>                // Include standard integer definitions
#include "tmr0.h"                  // Include the header file for TMR0

const uint8_t tmr0_mask = 0xFF;    // Define a constant mask value for TMR0

void TMR0_Initialize(void) {
    OPTION_REGbits.T0CS = 0;       // Configure TMR0 for Timer mode
    OPTION_REGbits.T0SE = 0;       // Increment TMR0 on low-to-high transition
    OPTION_REGbits.PSA = 0;        // Enable TMR0 prescaler
    OPTION_REGbits.PS = 0x07;      // Set TMR0 prescaler to 1:256

    // Load TMR0 with a calculated value based on system frequency
    TMR0 = (uint8_t)(tmr0_mask & (256 - (((2 * _XTAL_FREQ) / (256 * 4)) / 500)));

    INTCONbits.TMR0IF = 0;         // Clear TMR0 interrupt flag
    INTCONbits.TMR0IE = 1;         // Enable TMR0 interrupt
}

void TMR0_StartTimer(void) {
    INTCONbits.TMR0IE = 1;         // Enable TMR0 interrupt
}

void TMR0_StopTimer(void) {
    INTCONbits.TMR0IE = 0;         // Disable TMR0 interrupt
}

void TMR0_ISR(void) {
    static volatile uint16_t CountCallBack = 0;  // Static variable to count callbacks

    INTCONbits.TMR0IE = 0;         // Disable TMR0 interrupt
    INTCONbits.TMR0IF = 0;         // Clear TMR0 interrupt flag

    // Reload TMR0 with a calculated value based on system frequency
    TMR0 = (uint8_t)(tmr0_mask & (256 - (((2 * _XTAL_FREQ) / (256 * 4)) / 500)));

    // Increment callback counter and check if it reaches the defined factor
    if (++CountCallBack >= TMR0_INTERRUPT_TICKER_FACTOR) {
        timer_isr();               // Call user-defined timer ISR function
        CountCallBack = 0;         // Reset callback counter
    }

    INTCONbits.TMR0IE = 1;         // Re-enable TMR0 interrupt
}
