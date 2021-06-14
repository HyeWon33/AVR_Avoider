#define F_CPU 16000000L
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Text_LCD.h"

#define PRESCALER 256
uint8_t MODE = 4;

uint8_t step_regular_data[] = {0x03, 0x06, 0x0C, 0x09};
uint8_t step_reverse_data[] = {0x09, 0x0C, 0x06, 0x03};
uint8_t step_late_data[] = {0x09, 0x03, 0x06, 0x0C};

int step_index = 0;
int dir = 2;
int distance;
int count = 0;

int adc;
int adc_distance;

volatile ISR(TIMER1_COMPA_vect){
	
	if(distance <= 20 || adc_distance < 9){ //거리 20cm 보다 작다. ->회전
		
		OCR1AH = 0x01;
		OCR1AL = 0xf3;
		
		PORTE = 0x0F; // 1111 0000
		
		step_index++;
		if(step_index >= 4){
			step_index = 0;
		}
		PORTB = step_regular_data[step_index];
		PORTD = step_late_data[step_index];
	}
	
	else if(distance > 20 && distance < 400){//거리 20cm 보다 크다 400cm	보다 작다. -> 직진
		
		OCR1AH = 0x01;
		OCR1AL = 0x8b;
		
		PORTE = 0xFF; // 1111 1111
		
		step_index++;
		if(step_index >= 4){
			step_index = 0;
		}
		PORTB = step_reverse_data[step_index];
		PORTD = step_regular_data[step_index];
		
	}
}

//정면 초음파 거리 계산
int measure_distance(void){
	PORTC &= ~(1 << PC1);
	_delay_us(1);
	PORTC |= (1 << PC1);
	_delay_us(10);
	PORTC &= ~(1 << PC1);
	
	TCNT3 = 0;
	while(!(PINC & 0x01)){
		if(TCNT3 > 65000) return 0;
	}
	TCNT3 = 0;
	while(PINC & 0x01){
		if(TCNT3 > 65000){
			TCNT3 = 0;
			break;
		}
	}
	double pulse_width = 1000000.0 * TCNT3 * PRESCALER / F_CPU;
	
	return (int)(pulse_width / 58);
}

//ADC
void ADC_init(unsigned char channel){
	ADMUX |= (1 << REFS0);
	ADMUX |= channel;
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADFR);
	ADCSRA |= (1 << ADSC);
}

int read_ADC(void){
	while(!(ADCSRA & (1 << ADIF)));
	return ADC;
}

//레지스터 설정
void All_init(){
	DDRB = 0x0F; // right motor PORTB
	DDRD = 0x0F; // left motor PORTD
	
	DDRE = 0xFF; //LED
	
	DDRC |= 0x02; //정면 초음파
	DDRC &= 0xFE;
	
	//타이머/카운터 3번 
	TCCR3A |= (1 << WGM31) | (1 << WGM30); //고속 PWM 모드 
	TCCR3B |= (1 << WGM33) | (1 << WGM32);
	TCCR3B |= (1 << CS32); //분주비 256
	
	//타이머/카운터 1번
	TCCR1A |= (1 << WGM11) | (1 << WGM10); //고속 PWM 모드 
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	TCCR1B |= (1 << CS12); //분주비 256
	
	TCCR1A |= (1 << COM1A1); //비반전 모드
	
	TIMSK |= (1 << OCIE1A); //비교일치 인터럽트 횔성화
}


int main(void){
	All_init();
	ADC_init(2);
	
	OCR1A = 1249; 
	OCR3A = 1249;
	
	sei();
	
	while(1){

		adc = read_ADC();
		adc_distance = 5461 / (adc - 17) - 2;
	
		distance = measure_distance();
		
		_delay_ms(100);
	}
	
	return 0;
}