#include "avr/io.h"

#ifndef F_CPU
#define F_CPU 8000000ul
#endif
#include <util/delay.h>
#include "debug.h"
#ifndef LED_BUILTIN
#define LED_BUILTIN _BV(PB5)
#endif
#include "avr/interrupt.h"

void delay_ms(uint32_t ms){
    while(ms--){
        _delay_ms(1);
    }
}

void blink(uint8_t n)
{
    delay_ms(5);
    for(int i = 0; i < n; i++){
        PORTB |= LED_BUILTIN;
        delay_ms(1);
        PORTB &= ~(LED_BUILTIN);
        delay_ms(1);
    }
}

void blink_dur(uint8_t n, uint32_t dur)
{
    delay_ms(5);
    for(int i = 0; i < n; i++){
        PORTB |= LED_BUILTIN;
        delay_ms(dur);
        PORTB &= ~(LED_BUILTIN);
        delay_ms(1);
    }
    
}

void blink_reg(uint8_t reg)
{
    delay_ms(5);
    blink_dur(1, 2);
    for(int i =0; i < 8; i++){
        PORTB &= ((reg >> i) & 1) ? 0 : LED_BUILTIN;
        delay_ms(1);
        PORTB |= (LED_BUILTIN);
        delay_ms(1);
    }
    blink_dur(1, 2);
}
