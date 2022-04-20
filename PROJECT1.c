/*
 * PROJECT1.c
 *
 *  Created on: Oct 20, 2020
 *      Author: Omar Khaled
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define SET_BIT(REG,BIT) REG|=(1<<BIT)
# define CLEAR_BIT(REG,BIT) REG&=~(1<<BIT)

unsigned char unit_sec=0;
unsigned char tenth_sec=0;
unsigned char unit_min=0;
unsigned char tenth_min=0;
unsigned char unit_hr=0;
unsigned char tenth_hr=0;
unsigned char bool=1;

void TIMER1_init(void)
{
	SET_BIT(SREG,7);
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,CS12);
	SET_BIT(TCCR1B,CS10);
	SET_BIT(TIMSK,OCIE1A);
	TCNT1=0;
	OCR1A=1000;
}

void INT0_init(void)
{
	SET_BIT(SREG,7);
	CLEAR_BIT(DDRD,2);
	SET_BIT(PORTD,2);
	SET_BIT(MCUCR,ISC01);
	SET_BIT(GICR,6);
}

void INT1_init(void)
{
	SET_BIT(SREG,7);
	CLEAR_BIT(DDRD,3);
	SET_BIT(MCUCR,ISC10);
	SET_BIT(MCUCR,ISC11);
	SET_BIT(GICR,7);
}

void INT2_init(void)
{
	SET_BIT(SREG,7);
	CLEAR_BIT(DDRB,2);
	SET_BIT(PORTB,2);
	CLEAR_BIT(MCUCSR,ISC2);
	SET_BIT(GICR,5);
}

ISR(TIMER1_COMPA_vect)
{
	if (bool)
	{
		unit_sec++;
		if (unit_sec == 10)
		{
			unit_sec=0;
			tenth_sec++;
		}
	}
}

ISR(INT0_vect)
{
	unit_sec=0;
	tenth_sec=0;
	unit_min=0;
	tenth_min=0;
	unit_hr=0;
	tenth_hr=0;
}

ISR(INT1_vect)
{
	bool=0;
}

ISR(INT2_vect)
{
	bool=1;
}

int main ()
{
	DDRC |=0X0F;
	DDRA |=0X3F;
	PORTC=0;
	INT0_init();
	INT1_init();
	INT2_init();
	TIMER1_init();

	while(1)
	{
		if (tenth_sec == 6)
		{
			tenth_sec=0;
			unit_min++;
		}
		if (unit_min == 10)
		{
			unit_min=0;
			tenth_min++;
		}
		if (tenth_min == 6)
		{
			tenth_min=0;
			unit_hr++;
		}
		if (unit_hr == 10)
		{
			unit_hr=0;
			tenth_hr++;
		}
		if (tenth_hr == 10)
		{
			tenth_hr=0;
		}

		PORTA =0X01;
		PORTC=unit_sec;
		_delay_ms(5);
		PORTA =0X02;
		PORTC=tenth_sec;
		_delay_ms(5);
		PORTA =0X04;
		PORTC=unit_min;
		_delay_ms(5);
		PORTA =0X08;
		PORTC=tenth_min;
		_delay_ms(5);
		PORTA =0X10;
		PORTC=unit_hr;
		_delay_ms(5);
		PORTA =0X20;
		PORTC=tenth_hr;
		_delay_ms(5);
	}
}
