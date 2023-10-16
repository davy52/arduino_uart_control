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


#include "arduino_boards.h"

#include <util/delay.h>
#include "usart.h"

void delay_ms(uint32_t ms){
    while(ms--){
        _delay_ms(1);
    }
}

int main(void)
{
    DDRB |= LED_BUILTIN;
    uint16_t dtime = 200;
    
    usart_settings_t settings = {
        .char_size = USART_CHAR_8BIT,
        .data_reg_empty_int_en = 0,
        .rx_complete_int_en = 0,
        .tx_complete_int_en = 0,
        .double_speed = 0,
        .mode = USART_MODE_ASYNC,
        .multi_proc_mode = 0,
        .parity = USART_PARITY_EVEN,
        .prescaler = 51,
        .receiver_en = 0,
        .transmitter_en = 0,
        .stopbit_mode = USART_STOP_1BIT
    };

    usart_init(settings);
    uint8_t data = 0;

    while(1){

        while(usart_write(data) != USART_ERR_OK);
        data += 3;
        
        PORTB ^= LED_BUILTIN;
        delay_ms(dtime);
    }
}
