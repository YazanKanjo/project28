#include <xc.h>
#define LCDPORT PORTB
#define RS PORTBbits.RB0
#define RW PORTBbits.RB1
#define E PORTBbits.RB2
bit  status=0;
#define lcd_delay 300
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xc0
#define BlankSpace ' '

int num[10];
unsigned char c;

void delay(unsigned int j)
{
	unsigned int i=0;
	for(i=0;i<j;i++);
}

void lcd_init_write(unsigned char a)
{
	RS=0;
	RW=0;
	LCDPORT=a;
	E=1;
	delay(lcd_delay);
	E=0;
}


void lcd_com(unsigned char a)
{
	unsigned char temp;
	if(status)
	{
		status=0;
		goto next;
	}
	RS=0;
	next:
	RW=0;					
	temp=a;					
	temp&=0xf0;
	LCDPORT&=0x0f;
	LCDPORT|=temp;
	E=1;
	delay(lcd_delay);
	E=0;				
	temp=a<<4;
	temp&=0xf0;
	LCDPORT&=0x0f;
	LCDPORT|=temp;
	E=1;
	delay(lcd_delay);
	E=0;
}

void lcd_data(unsigned char a)
{
	status=1;
	RS=1;
	lcd_com(a);
}
void lcd_init(void)
{
	delay(lcd_delay);
	lcd_init_write(0x30);
	delay(lcd_delay);
	lcd_init_write(0x30);
	delay(lcd_delay);
	lcd_init_write(0x30);
	delay(lcd_delay);
	lcd_init_write(0x20);
	delay(lcd_delay);
	lcd_com(0xC);
	//lcd_com(0x28);         //Display Off, Cursor Off, Blink Off
	delay(lcd_delay);
	lcd_com(4);
	delay(lcd_delay);
	lcd_com(0x85);       
	delay(lcd_delay);       
	lcd_com(6);
	delay(lcd_delay);
	lcd_com(1);
	delay(lcd_delay);
}
void lcd_puts(char *str)
{
 	unsigned int i=0;
	for(;str[i]!=0;i++)
		lcd_data(str[i]);
}

void hex2lcd(unsigned char hex){
	char temp1,temp2;
	temp1 = hex;
	temp2=0;
	do{
		temp1 = temp1-100;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);
	if(temp2>0)
	lcd_data(temp2+0x30);
	temp2=0;
	temp1 = temp1+100;
	do{
		temp1 = temp1-10;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);
	lcd_data(temp2+0x30);
	temp2 = temp1+10;
	lcd_data(temp2+0x30);
}
void LCD_ScrollMessage(char *msg_ptr)
{
  unsigned char i,j;

  for(i=0;msg_ptr[i];i++)
    {

      lcd_com(LineOne);

    for(j=0;j<LCDMaxChars && msg_ptr[i+j];j++)
	  lcd_data(msg_ptr[i+j]);
	  delay(1200);

	for(j=j; j<LCDMaxChars; j++)
      lcd_data(BlankSpace);
      delay(1200);
    }
}

void lcd_print_number(unsigned int i)
{   int p;
int k=0;
while(i>0)
{
  num[k]=i%10;
  i=i/10;
  k++;
}
k--;
for (p=k;p>=0;p--)
{
  c=num[p]+48;
  lcd_data(c);
}
return;
}
