#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
  
#define SERVO_MIN 500
#define SERVO_MAX 2500  
#define DIR_UP 0
#define DIR_DOWN 1
#define STEP 100
  
uint8_t count = 0;
uint16_t freq_top = 20000;
uint16_t duty = 1500;
uint8_t dir = DIR_DOWN;
  
ISR(TIMER1_COMPA_vect)
{
	count++;
	
    if (count % 2)
	{
		OCR1A = freq_top - duty;
		PORTB &= ~(1 << PB3);
	}
	else
	{    
		OCR1A = duty;
		PORTB |= (1 << PB3);
		
		switch(dir)
		{
			case DIR_DOWN:
				duty -= STEP;
				if(duty < SERVO_MIN)
				{
					duty = SERVO_MIN;
					dir = DIR_UP;
				}
				break;
			
			case DIR_UP:
				duty += STEP;
				if(duty > SERVO_MAX)
				{
					duty = SERVO_MAX;
					dir = DIR_DOWN;
				}
				break;
		}
	}
}
  
int main(void)
{
    DDRB |= (1 << PB3) | (1 << PB2);
    TCCR1A |= (0 << COM1A1) | (0 << COM1A0) | // Normal port operation, OC1A/OC1B disconnected
	          (0 << WGM11) | (0 << WGM10);    // CTC/OCR1A
	TCCR1B |= (0 << WGM13) | (1 << WGM12);    // CTC/OCR1A
	OCR1A = duty;
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // Clock - no prescaling
	TIMSK |= (1 << OCIE1A); // Enable interrupt on output compare A match 
	sei();
	
	while(1)
	{
	}
}