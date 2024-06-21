#define INTERRUPT_GlobalInterruptEnable() (INTCONbits.GIE = 1)       // Enable global interrupts
#define INTERRUPT_GlobalInterruptDisable() (INTCONbits.GIE = 0)      // Disable global interrupts
#define INTERRUPT_PeripheralInterruptEnable() (INTCONbits.PEIE = 1)   // Enable peripheral interrupts
#define INTERRUPT_PeripheralInterruptDisable() (INTCONbits.PEIE = 0)  // Disable peripheral interrupts
#define _XTAL_FREQ 20000000   // Define crystal frequency for delay calculation
#define TMR0_INTERRUPT_TICKER_FACTOR    1   // Factor for TMR0 interrupt ticker
void TMR0_Initialize(void);   // Initialize Timer0
void TMR0_StartTimer(void);   // Start Timer0
void TMR0_StopTimer(void);    // Stop Timer0
void TMR0_ISR(void);          // Timer0 interrupt service routine declaration
void timer_isr(void);         // Another interrupt service routine declaration
void interrupt INTERRUPT_InterruptManager(void);  // Handles all interrupts
