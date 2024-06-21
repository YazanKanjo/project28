#include <xc.h>
#include <stdint.h>
#include "tmr0.h"

const uint8_t tmr0_mask = 0xFF;

void TMR0_Initialize(void) {
    // Set TMR0 to the options selected in the User Interface

    // T0PS 1:256; T0SE Increment_hi_lo; T0CS FOSC/4; PSA assigned;
    OPTION_REGbits.T0CS = 0;   // Timer0 clock source is instruction cycle clock (FOSC/4)
    OPTION_REGbits.T0SE = 0;   // Increment on low-to-high transition on T0CKI pin
    OPTION_REGbits.PSA = 0;    // Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS = 0x07;  // Prescaler rate is 1:256

    // Calculating TMR0 reload value for 2ms delay
    TMR0 = (uint8_t)(tmr0_mask & (256 - (((2 * _XTAL_FREQ) / (256 * 4)) / 500)));

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
}

void TMR0_StartTimer(void) {
    // Start the Timer0 by enabling the interrupt
    INTCONbits.TMR0IE = 1;
}

void TMR0_StopTimer(void) {
    // Stop the Timer0 by disabling the interrupt
    INTCONbits.TMR0IE = 0;
}

void TMR0_ISR(void) {
    static volatile uint16_t CountCallBack = 0;

    // Disable Timer0 interrupt
    INTCONbits.TMR0IE = 0;
    // Clear the Timer0 interrupt flag
    INTCONbits.TMR0IF = 0;

    // Reload Timer0 for the next cycle
    TMR0 = (uint8_t)(tmr0_mask & (256 - (((2 * _XTAL_FREQ) / (256 * 4)) / 500)));

    if (++CountCallBack >= TMR0_INTERRUPT_TICKER_FACTOR) {
        // Call the ticker function
        timer_isr();

        // Reset the ticker counter
        CountCallBack = 0;
    }

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
}
