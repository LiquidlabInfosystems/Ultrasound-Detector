#include <reg51.h>
#include "Serial.h"
sbit Echo = P3^2;
sbit Trig = P3^5;
sbit Status_Red=P2^0;
sbit Status_Green=P2^1;
sbit Power_Led=P2^2;
sbit Pilot_Green=P1^0;
sbit Pilot_Red=P1^1;

void delay_us(unsigned int us)//This function provide delay in us uS.
{
		while(us--);
}
void delay_ms(unsigned int ms)//This function Provide delay in ms Ms.
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<127;j++);
}
void main()
{
	unsigned int Count,Time,Distance; 
	Power_Led = 1;
	Echo = Trig = 0;
	TMOD |= 0x01;//Timer 0 in 16-bit mode
	Serial_Init();
	TX_String("Liquidlab\r\n");
	TX_String("Distance Measurement using Ultrasonics Sensor HC-SR04\r\n");
	do{
	Trig = 1;//Send 10us start pulse to HC-SR04 as per datasheet of HC-SR04
	delay_us(10);//~10us delay
	Trig = 0;	
	while(Echo == 0);//Wait for Rising edge at Echo pin
	TR0=1;//Start Timer
	TL0=TH0=0;//Clear timer count register
	while(Echo == 1)//Wait for Falling edge at Echo pin
	{
		if(TF0 == 1)//timer over if no obstacle is detected
			break;
  }
	TR0=0;//Stop Timer.
	TF0 = 0;//clear Timer Over Flow Flag
	Count = TL0 + TH0*256;//Calculate number of count
	Time = Count*1.085;//Calculate total time in uS.
	Distance = Time/58;//As per datasheet of HC-SR04 Distance is in Centimeter
	TX_String("--------------\r\n");
	TX_String("Distance:-");
	TX_Int(Distance);//Send distance to serial
	TX_String("\r\n");
	if(Distance> 0 && Distance<=150)
	{
	Status_Green = 1;
	Status_Red = 0;
	Pilot_Green = 0;
	Pilot_Red = 1;
	}
	else
	{
	Status_Red = 1;
	Status_Green = 0;
	Pilot_Red = 0;
	Pilot_Green = 1;
	
	}
	if (Distance ==0)
	{
	Echo=0;
	delay_ms(100);
	Echo=1;
	}
	delay_ms(2000);
  }while(1);
}