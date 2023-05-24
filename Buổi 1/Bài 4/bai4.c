#include <mcs51/8052.h>
#include<math.h>
#include<string.h>
#define DQ	P3_7//Data pin of the DS18B20
#define LCD_DATA P2
#define EN P0_5
#define RS P0_7
#define RW P0_6
//Temp variables
unsigned char currentTemp;
unsigned char lowestTemp6s = 0 , heighestTemp6s = 0;//update everytime read temp
unsigned char lowestTemp1m = 0 , heighestTemp1m = 0;//update each 1 minute
unsigned char timeReadTemp = 0;//times read currentTemp:
unsigned char startTemp = 0;//first time get temp

unsigned char unit = 0;// convert unit C(0) <-> F(1)

// create Prototype function
void delay_ms(unsigned int t);
void delay_us_DS18B20(unsigned int t);

void Init_DS18B20();
unsigned char ReadByteFromScratchpad();
void WriteByteToScratchpad(unsigned char byte);
void ReadTemperature();

void Init_System();
void Wait_For_LCD();
void LCD_Send_Command(unsigned char x);
void LCD_Write_One_Char(unsigned char c);
void LCD_Write_String(unsigned char *s);
void transform_to_xy(unsigned char x, unsigned char y);
void Show_Temperature();

unsigned char Convert_C_F(unsigned char c);
unsigned char Convet_F_C(unsigned char f);

void main()
{

    Init_System();
    LCD_init();
    LCD_Write_String("Taking temp...");//waiting
    delay_ms(500);

    //waiting for sensor connection
    while(1){
        ReadTemperature();
        if(currentTemp!= 85) break;
    }
    timeReadTemp = 0;
    startTemp = 0 ;
    LCD_Send_Command(0x01);//clear
    while(1){
          transform_to_xy(0,0);
          ReadTemperature();     //Read the DS18B20 ...
          Show_Temperature();
          delay_ms(6000);        //for each 6s
    }
}

//  Interupt
void ISR0() __interrupt (0)
{
	EA = 0;
	delay_ms(500);
	if(unit == 0)
    {
        currentTemp = Convert_C_F(currentTemp);
        lowestTemp1m= Convert_C_F(lowestTemp1m);
        lowestTemp6s= Convert_C_F(lowestTemp6s);
        heighestTemp1m = Convert_C_F(heighestTemp1m);
        heighestTemp6s = Convert_C_F(heighestTemp6s);
        Show_Temperature();
    }
    else
    {
        currentTemp= Convet_F_C(currentTemp);
        lowestTemp1m= Convet_F_C(lowestTemp1m);
        lowestTemp6s= Convet_F_C(lowestTemp6s);
        heighestTemp1m = Convet_F_C(heighestTemp1m);
        heighestTemp6s = Convet_F_C(heighestTemp6s);
        Show_Temperature();
    }
	EA = 1;
}


//Delay func using timer0
void delay_ms(unsigned int s)
{
    unsigned int i = 0;
    for(i = 0 ; i < s; i++)
    {
        //~1ms
        TH0 = 0xFC;
        TL0 = 0x66; // 64614 => ~1ms with f_XTAL = 11. 0592MHz
        TF0 = 0;
        TR0 = 1;
        while(TF0 == 0);
    }

}

//Delay func for the DS18B20 (not exactly)
void delay_us_DS18B20(unsigned int t)
{
    while(t--);
}

//-------DS18B20----------
//Init for the DS18B20
void Init_DS18B20()
{
    DQ = 1;
    delay_us_DS18B20(8);
    DQ = 0;					//Reset pulse ...
    delay_us_DS18B20(65); 	//for a minimum period. See the datasheet.
    DQ = 1;					//Rising edge
    delay_us_DS18B20(20);   //Wait for the DS18B20 to response
}
//Read a byte from the Scratchpad
unsigned char ReadByteFromScratchpad()
{
    unsigned char i = 0;
    unsigned char byte = 0;
    for (i = 8; i > 0; i--){
          DQ = 0;
          byte >>= 1;
          DQ = 1;			//The master bus releases the 1-Wire bus
          if(DQ)			//Read 1? Otherwise, Read 0
          	byte |= 0x80;
          delay_us_DS18B20(4);
    }
    return(byte);
}
//Write a byte to the Scratchpad
void WriteByteToScratchpad(unsigned char byte)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--){
        DQ = 0;
        DQ = byte&0x01;
        delay_us_DS18B20(5);
        DQ = 1;	  	//The master bus releases the 1-Wire bus
        byte >>= 1;
    }
}
//Read the DS18B20
void ReadTemperature()
{
    unsigned char Byte0 = 0;		//Byte0 of the Scratchpad
    unsigned char Byte1 = 0;		//Byte1 of the Scratchpad

    Init_DS18B20();				   	//DS18B20 initialization
    WriteByteToScratchpad(0xCC);	//The master issues Skip ROM [CCh] command
    WriteByteToScratchpad(0x44);	//Convert T [44h] command. To initiate a temp measurement and A-to-D conversion.
    delay_us_DS18B20(10);

    Init_DS18B20();					//DS18B20 initialization
    WriteByteToScratchpad(0xCC);	//The master issues Skip ROM [CCh] command
    WriteByteToScratchpad(0xBE);	//Read temp value. Read Scratpad [BEh] command.
    delay_us_DS18B20(10);
    Byte0=ReadByteFromScratchpad();	//Read Byte0 of the Scratchpad (low byte of the temp value)
    Byte1=ReadByteFromScratchpad();	//Read Byte1 of the Scratchpad (high byte of the temp value)

	//Calculate the temp value from Byte0 & Byte1. Then save it to the temp variable always with unit 'C
	currentTemp=((Byte1*256+Byte0)>>4);
    //with F unit
    if(unit == 1)
    {
        currentTemp=Convert_C_F(currentTemp);
    }

	timeReadTemp++;//times update currentTemp

	if(lowestTemp6s > currentTemp) 
    lowestTemp6s= currentTemp;
	if(heighestTemp6s < currentTemp) 
    heighestTemp6s = currentTemp;

    //first time
    if(startTemp == 0)
    {
        lowestTemp1m=currentTemp;
        lowestTemp6s=currentTemp;
        heighestTemp1m = currentTemp;
        heighestTemp6s = currentTemp;
        startTemp = 1;
    }

}

//-------LCD----------

void Init_System()
{
    //Interupt
    TMOD = 0x01;//module 1 for timer0
    EA = 1;
    EX0 = 1;//using int0 to convert temp unit

    //lCD
	RW=1;//Write mode
	unit = 0;
}

void LCD_init()
{
	LCD_Send_Command(0x38); //8 bit, 2 lines
	LCD_Send_Command(0x0C); //Display on, cursor off
	LCD_Send_Command(0x01); //clear
	LCD_Send_Command(0x80); //return home
}

//wait until LCD is ready
void Wait_For_LCD()
{
	//Delay_By_Timer_0(80);
	delay_ms(1);
}
//send command to LCD
void LCD_Send_Command(unsigned char x)
{
	LCD_DATA=x;
	RS=0;
	RW=0;
	EN=1;
	delay_ms(1);
	EN=0;
	Wait_For_LCD();
	EN=1;
}
//display one char
void LCD_Write_One_Char(unsigned char c)
{
	LCD_DATA=c;
	RS=1;
	RW=0;
	EN=1;
	delay_ms(1);
	EN=0;
	Wait_For_LCD();
	EN=1;
}

//display one string
void LCD_Write_String(unsigned char *s)
{
	unsigned char length;
	length=strlen(s);
	while(length!=0)
	{
		LCD_Write_One_Char(*s);
		s++;
		length--;
	}
}

//move LCD cursor to (x, y)
void transform_to_xy(unsigned char x, unsigned char y){
        unsigned char address;
        if(!y)address=(0x80+x);
        else address=(0xc0+x);
        delay_us_DS18B20(1000);
        LCD_Send_Command(address);
        delay_us_DS18B20(50);
}
// display temp in LCD
void Show_Temperature()
{
     transform_to_xy(0,0);
     //update L and H
     if(timeReadTemp == 10)//one minute
     {
         //update
         lowestTemp1m= lowestTemp6s;
         heighestTemp1m = heighestTemp6s;

         //reset
         timeReadTemp= 0;
     }
     //line1
     LCD_Write_String("Cur Temp: ");
     LCD_Write_One_Char((currentTemp%100)/10+48);//tens
     LCD_Write_One_Char((currentTemp%10)+48);//units
     if(unit == 0 )
     LCD_Write_String("'C");
     else LCD_Write_String("'F");
     //line2
     LCD_Send_Command(0xC0);//start in line2
     LCD_Write_String("L:");
     LCD_Write_One_Char((lowestTemp1m%100)/10+48);//tens
     LCD_Write_One_Char((lowestTemp1m%10)+48);//units
     LCD_Write_String(" H:");
     LCD_Write_One_Char((heighestTemp1m%100)/10+48);//tens
     LCD_Write_One_Char((heighestTemp1m%10)+48);//units
}

//convert unit
unsigned char Convert_C_F(unsigned char c) {
    float f = (c * 1.8) + 32;
    return (unsigned char)(f + 0.5);
}

unsigned char Convet_F_C(unsigned char f) {
    float c = (f - 32) * 5 / 9.0;
    return (unsigned char)(c + 0.5);
}


