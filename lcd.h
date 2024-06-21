void delay(unsigned int j);               // Function to introduce a delay
void lcd_init_write(unsigned char a);     // Function to send initial commands to the LCD
void lcd_com(unsigned char a);            // Function to send a command to the LCD
void lcd_data(unsigned char a);           // Function to send data to the LCD
void lcd_init(void);                      // Function to initialize the LCD
void lcd_puts(char *str);                 // Function to print a string on the LCD
void hex2lcd(unsigned char hex);          // Function to convert a hex value to decimal and display it on the LCD
void LCD_ScrollMessage(char *msg_ptr);    // Function to scroll a message on the LCD
void lcd_print_number(unsigned int i);    // Function to print a number on the LCD
void lcd_clear(void);                     // Function to clear the LCD display
