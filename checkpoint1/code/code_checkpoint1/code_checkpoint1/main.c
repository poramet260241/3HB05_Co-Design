#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

unsigned char LOOKUPTB[] = { 0b00111111, 0b00000110,
	0b01011011, 0b01001111,
	0b01100110, 0b01101101,
	0b01111101, 0b00000111,
	0b01111111, 0b01101111,
	0b01110111, 0b01111100,
	0b00111001, 0b01011110,
	0b01111001, 0b01110001
};

void test_mode(void){
	for(int i=0;i<16;i++){
		PORTB = LOOKUPTB[i];
		_delay_ms(100);
	}
}

int main(void)
{

	unsigned char DISPLY, SWITCH_V;
	DDRB = 0xFF; //port B = output
	DDRC = 0x00; //port C = input
	while(1)
	{
		if (~PIND & 0x01){
			for(int i=0; i<20;i++){
				_delay_ms(100);
				if (~PIND & 0x01)
				{
					//do nothing
				}
				else
					break;
			}
			if (~PIND & 0x01){
				test_mode();
			}
		}
		
		SWITCH_V = PINC;
		SWITCH_V &= 0x0F;
		DISPLY = LOOKUPTB[SWITCH_V];
		PORTB = DISPLY;
	}
}

