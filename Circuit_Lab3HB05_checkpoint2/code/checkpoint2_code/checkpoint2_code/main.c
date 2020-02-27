#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

unsigned char on[] = {0b01011100, 0b01010100};
unsigned char off[] = {0b01011100, 0b01110001};
	
void wait_until_sw_pressed();
void function_checkstate();
void function_blynk();

unsigned char LOOKUPTB[] = { 0b00111111, 0b00000110,
	0b01011011, 0b01001111,
	0b01100110, 0b01101101,
	0b01111101, 0b00000111,
	0b01111111, 0b01101111,
	0b01110111, 0b01111100,
	0b00111001, 0b01011110,
	0b01111001, 0b01110001 
};

int state = 0;

void self_test(void){					//use to test this circuit
	PORTB |= 0xFF;
	PORTC |= 0xFF;
	PORTD |= (1<<PD1);
	_delay_ms(1000);
	PORTB &= 0x00;
	PORTC &= 0x00;
	PORTD &= (0<<PD1);
}

void function_delay(void){				//use to generate clk 1Hz to PD2
	_delay_ms(500);
	PORTD &= (0<<PD2);
	_delay_ms(500);
	PORTD |= (1<<PD2);
}

void function_turnOn(void){				//use to turn on the lamp and led
	PORTB = on[0];
	PORTC = on[1];
	PORTD = PORTD | (1<<PD1);
	state = 1;
	function_blynk();
}

void function_blynk(void){
	int state_blynk = 0;
	int sw_test;
	do{
		sw_test = ~PIND & 0x01;
	}while(sw_test);
	
	while(1){
		if(state == 0){
			if(state_blynk == 0){
				PORTB = off[0];
				PORTC = off[1];
				state_blynk = 1;
			}
			else{
				PORTB = 0x00;
				PORTC = 0x00;
				state_blynk = 0;
			}
		}
		else{
			if(state_blynk == 0){
				PORTB = on[0];
				PORTC = on[1];
				state_blynk = 1;
			}
			else{
				PORTB = 0x00;
				PORTC = 0x00;
				state_blynk = 0;
			}
		}
		for(int i=0; i<10; i++){
			if(~PIND & 0x01){
				do{
					sw_test = ~PIND & 0x01;
				}while(sw_test);
				
				function_checkstate();
			}
			_delay_ms(50);
		}
	}
}

void function_countTurnOff(void){		//use to countdown and turn lamp off
	int digit0, digit1;
	for(int i=20;i>0;i--){
		digit1 = i/10;
		digit0 = i%10;
		PORTB = LOOKUPTB[digit1];
		PORTC = LOOKUPTB[digit0];
		_delay_ms(1000);
	}
	PORTB = off[0];
	PORTC = off[1];
	PORTD = PORTD & (0<<PD1);
	state = 0;
	function_blynk();
}

void function_checkstate(void){			//use to check this lamp is on or off
	if(state == 1){
		function_countTurnOff();
	}
	else{
		function_turnOn();
	}
}

void wait_until_sw_pressed(){			//wait for sw is pressed
	int sw;
	do{
		do{
			sw = ~PIND & 0x01;
		} while (sw);
		_delay_ms(10);
		sw = ~PIND &0x01;
	
	} while (sw);
	do{
		sw = ~PIND & 0x01;
	} while (!sw);
	function_checkstate();
}

int main(void){
	
	DDRB = 0xFF;					//set direction of data
	DDRC = 0xFF;
	DDRD = 0x0E;	
	
	PORTB = 0x00;					//out to port
	PORTC = 0x00;
	PORTD = 0x00;
	
	self_test();					//self test before using
	
	while(1){
		//function_delay();			//checkpoint 2.1
		wait_until_sw_pressed();	//checkpoint 2.2
	}
}