#pragma config FOSC = HS       
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF     
#pragma config BOREN = ON       
#pragma config LVP = OFF      
#pragma config CPD = OFF       
#pragma config WRT = OFF        
#pragma config CP = OFF

#include "stdio.h"
#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "supporing_cfile/lcd.h"
#include "supporing_cfile/eusart1.h"
#include "supporing_cfile/adc.h"
#include "supporing_cfile/tmr0.h"

#define _XTAL_FREQ 20000000 // Adjusted to 20 MHz
#define READ_ADC  1
#define CALCULATE_HEART_BEAT 2
#define SHOW_HEART_BEAT 3
#define DISPLAY_RESULT 4
#define IDLE 0
#define DEFAULT -1
#define LED1_PIN RB0 // Define the pin for LED1
#define LED2_PIN RB1 // Define the pin for LED2
#define MAX_BPM_RECORDS 20

volatile int rate[10]; 
volatile unsigned long sampleCounter = 0;
volatile unsigned long lastBeatTime = 0;
volatile int P = 512; 
volatile int T = 512; 
volatile int thresh = 530; 
volatile int amp = 0;
volatile bool firstBeat = true;
volatile bool secondBeat = false; 
volatile int BPM; 
volatile int Signal; 
volatile bool Pulse = false; 
volatile bool QS = false;
volatile int count = 0; 
volatile int sum = 0;

int main_state = -1;
int adc_value = 0;
unsigned long display_start_time = 0;
bool first_display_complete = false;
volatile bool final_display_complete = false; // Flag to indicate final display completion

const char *patient_name = "Patient: Ayyash"; // Patient name

int bpm_records[MAX_BPM_RECORDS];
int bpm_record_index = 0;

void system_init(void);

void calculate_heart_beat(int adc_value) {
    Signal = adc_value;
    sampleCounter += 2; 
    int N = sampleCounter - lastBeatTime; 

    if (Signal < thresh && N > 150) { 
        if (Signal < T) { 
            T = Signal; 
        }
    }

    if (Signal > thresh && Signal > P) { 
        P = Signal;
    } 

    if (N > 250) { 
        if ((Signal > thresh) && (Pulse == false) && (N > 150)) {
            Pulse = true;        
            lastBeatTime = sampleCounter; 

            if (secondBeat) { 
                secondBeat = false;
                for (int i = 0; i <= 9; i++) { 
                    rate[i] = N;
                }
            }

            if (firstBeat) { 
                firstBeat = false; 
                secondBeat = true;
                return;
            }

            uint16_t runningTotal = 0; 
            for (int i = 0; i <= 8; i++) { 
                rate[i] = rate[i + 1]; 
                runningTotal += rate[i]; 
            }

            rate[9] = N; 
            runningTotal += rate[9]; 
            runningTotal /= 10; 
            BPM = 60000 / runningTotal;

            QS = true; 

            // Store the BPM value in the records array
            if (BPM >= 60 && BPM <= 100) {
                if (bpm_record_index < MAX_BPM_RECORDS) {
                    bpm_records[bpm_record_index++] = BPM;
                }
            }
        }
    }

    if (Signal < thresh && Pulse == true) { 
        Pulse = false; 
        amp = P - T; 
        thresh = amp / 2 + T; 
        P = thresh; 
        T = thresh;
    }

    if (N > 2500) { 
        thresh = 530; 
        P = 512; 
        T = 512; 
        lastBeatTime = sampleCounter; 
        firstBeat = true;
        secondBeat = false;
    }
}

void control_led1(int state) {
    RB0 = state; // Set LED1 state
}

void control_led2(int state) {
    RB1 = state; // Set LED2 state
}

#define DISPLAY_RESULT_TIME 60000 // 60 seconds in milliseconds

void main() {
    system_init();
    main_state = READ_ADC;

    control_led1(0); // Ensure LEDs are off initially
    control_led2(0); // Ensure LEDs are off initially

    while (1) {
        if (final_display_complete) {
            continue; // Skip the loop if the final display is complete
        }

        switch (main_state) {
            case READ_ADC: {                
                adc_value = ADC_Read(0);
                main_state = CALCULATE_HEART_BEAT;                
                break;
            }
            case CALCULATE_HEART_BEAT: {
                calculate_heart_beat(adc_value);
                main_state = SHOW_HEART_BEAT;
                break;
            }
            case SHOW_HEART_BEAT: {
                if (QS == true) { 
                    QS = false;
                    lcd_init();
                    lcd_com(0x80); // Move to the first line
                    lcd_puts(patient_name); // Display patient name
                    lcd_com(0xC0); // Move to the second line
                    lcd_puts("BPM: ");
                    lcd_print_number(BPM);
                    lcd_puts(" "); // Clear the rest of the line
                    __delay_ms(200); // Small delay to ensure proper display
                    if (!first_display_complete) {
                        display_start_time = sampleCounter;
                        first_display_complete = true;
                    }

                    // Ensure LEDs are off during the first message
                    control_led1(0);
                    control_led2(0);
                }
                if (first_display_complete && (sampleCounter - display_start_time >= DISPLAY_RESULT_TIME)) {
                    main_state = DISPLAY_RESULT;
                } else {
                    main_state = IDLE;
                }
                break;
            }
            case DISPLAY_RESULT: {
                // Calculate average BPM
                int total_bpm = 0;
                for (int i = 0; i < bpm_record_index; i++) {
                    total_bpm += bpm_records[i];
                }
                int average_bpm = 0;
                if (bpm_record_index > 0) {
                    average_bpm = total_bpm / bpm_record_index;
                }

                lcd_com(0x01); // Clear display
                __delay_ms(40); // Delay to allow LCD to clear
                lcd_com(0x80); // Move to the first line
                lcd_puts("PR BPM: ");
                lcd_print_number(average_bpm);
                lcd_puts(" ");
                final_display_complete = true; // Set the flag to indicate the final display is complete

                // Control LEDs based on average BPM for the second message
                if (average_bpm >= 60 && average_bpm <= 100) {
                    control_led1(0); // Turn on LED1
                    control_led2(1); // Turn off LED2
                } else {
                    control_led1(1); // Turn off LED1
                    control_led2(0); // Turn on LED2
                }
                break;
            }
            case IDLE: {
                break;
            }
            default: {
                break;
            }
        }
    }
}

void system_init(void) {
    TRISB = 0x00;
    lcd_init();
    TMR0_Initialize();
    TMR0_StartTimer();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();   
    ADC_Init();
}

void timer_isr() {
    main_state = READ_ADC;    
}

void interrupt INTERRUPT_InterruptManager (void) {
    if (INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1) {
        TMR0_ISR();
    }
}
