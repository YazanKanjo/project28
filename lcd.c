#include <xc.h>                         // Include the XC header file
#define LCDPORT PORTB                   // Define LCDPORT as PORTB
#define RS PORTBbits.RB0                // Define RS as bit 0 of PORTB
#define RW PORTBbits.RB1                // Define RW as bit 1 of PORTB
#define E PORTBbits.RB2                 // Define E as bit 2 of PORTB
bit  status=0;                          // Declare a status bit
#define lcd_delay 300                   // Define a delay duration
#define LCDMaxLines 2                   // Define the number of LCD lines
#define LCDMaxChars 16                  // Define the number of characters per line
#define LineOne 0x80                    // Address for the first line
#define LineTwo 0xc0                    // Address for the second line
#define BlankSpace ' '                  // Define a blank space character

int num[10];                            // Array to store digits of a number
unsigned char c;                        // Character variable

// Function to introduce a delay
void delay(unsigned int j)
{
	unsigned int i=0;
	for(i=0;i<j;i++);                   // Simple loop for delay
}

// Function to send initial commands to the LCD
void lcd_init_write(unsigned char a)
{
	RS=0;                              // Set RS for command mode
	RW=0;                              // Set RW for write mode
	LCDPORT=a;                         // Send command to LCDPORT
	E=1;                               // Generate enable pulse
	delay(lcd_delay);                  // Wait for the command to process
	E=0;                               // End enable pulse
}

// Function to send a command to the LCD
void lcd_com(unsigned char a)
{
	unsigned char temp;
	if(status)                         // If status is set
	{
		status=0;                      // Reset status
		goto next;                     // Skip to next section
	}
	RS=0;                              // Set RS for command mode
	next:
	RW=0;                             // Set RW for write mode
	temp=a;                            // Store the command
	temp&=0xf0;                        // Mask lower nibble
	LCDPORT&=0x0f;                    // Clear upper nibble of LCDPORT
	LCDPORT|=temp;                    // Send upper nibble
	E=1;                              // Generate enable pulse
	delay(lcd_delay);                 // Wait for the command to process
	E=0;                              // End enable pulse
	temp=a<<4;                        // Shift lower nibble to upper nibble position
	temp&=0xf0;                       // Mask lower nibble
	LCDPORT&=0x0f;                    // Clear upper nibble of LCDPORT
	LCDPORT|=temp;                    // Send upper nibble
	E=1;                              // Generate enable pulse
	delay(lcd_delay);                 // Wait for the command to process
	E=0;                              // End enable pulse
}

// Function to send data to the LCD
void lcd_data(unsigned char a)
{
	status=1;                         // Set status to indicate data mode
	RS=1;                             // Set RS for data mode
	lcd_com(a);                       // Send data using lcd_com function
}

// Function to initialize the LCD
void lcd_init(void)
{
	delay(lcd_delay);                 // Initial delay
	lcd_init_write(0x30);             // Wake up command
	delay(lcd_delay);                 
	lcd_init_write(0x30);             // Repeat wake up command
	delay(lcd_delay);
	lcd_init_write(0x30);             // Repeat wake up command
	delay(lcd_delay);
	lcd_init_write(0x20);             // Set to 4-bit mode
	delay(lcd_delay);
	lcd_com(0xC);                     // Display ON, cursor OFF, blink OFF
	delay(lcd_delay);
	lcd_com(4);                       // Entry mode set
	delay(lcd_delay);
	lcd_com(0x85);                    // Set cursor position
	delay(lcd_delay);
	lcd_com(6);                       // Entry mode set
	delay(lcd_delay);
	lcd_com(1);                       // Clear display
	delay(lcd_delay);
}

// Function to print a string on the LCD
void lcd_puts(char *str)
{
 	unsigned int i=0;
	for(;str[i]!=0;i++)               // Iterate through the string
		lcd_data(str[i]);             // Display each character
}

// Function to convert a hex value to decimal and display it on the LCD
void hex2lcd(unsigned char hex){
	char temp1,temp2;
	temp1 = hex;
	temp2=0;
	do{
		temp1 = temp1-100;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);                // Check if the value is in the hundreds place
	if(temp2>0)
	lcd_data(temp2+0x30);             // Display the hundreds place if non-zero
	temp2=0;
	temp1 = temp1+100;
	do{
		temp1 = temp1-10;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);                // Check if the value is in the tens place
	lcd_data(temp2+0x30);             // Display the tens place
	temp2 = temp1+10;
	lcd_data(temp2+0x30);             // Display the ones place
}

// Function to scroll a message on the LCD
void LCD_ScrollMessage(char *msg_ptr)
{
  unsigned char i,j;

  for(i=0;msg_ptr[i];i++)
    {

      lcd_com(LineOne);              // Set cursor to the beginning of the first line

    for(j=0;j<LCDMaxChars && msg_ptr[i+j];j++)
	  lcd_data(msg_ptr[i+j]);       // Display characters on the first line
	  delay(1200);                   // Wait for scrolling effect

	for(j=j; j<LCDMaxChars; j++)
      lcd_data(BlankSpace);          // Fill the rest of the line with spaces
      delay(1200);                   // Wait for scrolling effect
    }
}

// Function to print a number on the LCD
void lcd_print_number(unsigned int i)
{   
    int p;
    int k=0;
    while(i>0)
    {
      num[k]=i%10;                  // Store each digit of the number in the array
      i=i/10;
      k++;
    }
    k--;
    for (p=k;p>=0;p--)
    {
      c=num[p]+48;                  // Convert digit to character
      lcd_data(c);                  // Display character on the LCD
    }
    return;
}
