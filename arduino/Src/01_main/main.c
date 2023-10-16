/*
 * @file main.c
 * @author  ()
 * @brief 
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#define ARDUINO_BOARD arduino_pro_mini_3v3
#include "arduino_boards.h"

#include <util/delay.h>
#include "usart.h"

// FUSES = {
//     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
//     .high = HFUSE_DEFAULT,
//     .extended = EFUSE_DEFAULT
// };

// LOCKBITS = LOCKBITS_DEFAULT;

void delay_ms(uint32_t ms){
    while(ms--){
        _delay_ms(1);
    }
}

volatile uint8_t data = 85;
#define BAUD 51u

void my_usart_init()
{
    usart_settings_t settings = {
        .char_size = USART_CHAR_8BIT,
        .data_reg_empty_int_en = 0,
        .rx_complete_int_en = 0,
        .tx_complete_int_en = 0,
        .double_speed = 0,
        .mode = USART_MODE_ASYNC,
        .multi_proc_mode = 0,
        .parity = USART_PARITY_EVEN,
        .prescaler = BAUD,
        .receiver_en = 1,
        .transmitter_en = 1,
        .stopbit_mode = USART_STOP_1BIT
    };
/*
    stat_a = 0b00000000
    stat_b = 0b00011000
    stat_c = 0b00000110
*/
    usart_init(settings);
}

void my_usart()
{
        while(usart_write(data) != USART_ERR_OK);
}

void doc_usart_init()
{
    // from documentation
    UCSR0A &= ~(1<<U2X0);
    UBRR0H = (uint8_t)(BAUD >> 8) & 0xf;
    UBRR0L = (uint8_t)(BAUD);

    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    UCSR0C = (1<<USBS0) | (3<UCSZ00);
}

void doc_usart()
{
        while(!(UCSR0A & (1<<UDRE0)));
        UDR0 = data;
}

int main(void)
{
    DDRB |= LED_BUILTIN;
    uint16_t dtime = 1000;
    

    // doc_usart_init();
    my_usart_init();

    while(1){

        // doc_usart();
        my_usart();

        // data += 3;
        
        PORTB ^= LED_BUILTIN;
        delay_ms(dtime);
    }
}
