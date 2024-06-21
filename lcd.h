void delay(unsigned int j);               // Function to create a delay
void lcd_init_write(unsigned char a);     // Function to send initial setup commands to the LCD
void lcd_com(unsigned char a);            // Function to send a control command to the LCD
void lcd_data(unsigned char a);           // Function to send data to the LCD display
void lcd_init(void);                      // Function to set up the LCD
void lcd_puts(char *str);                 // Function to display a string on the LCD
void hex2lcd(unsigned char hex);          // Function to convert a hex value to a decimal and display it on the LCD
void LCD_ScrollMessage(char *msg_ptr);    // Function to scroll a message across the LCD
void lcd_print_number(unsigned int i);    // Function to display a number on the LCD
void lcd_clear(void);                     // Function to clear the LCD screen
