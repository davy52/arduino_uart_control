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
#define F_CPU 8000000ul
// #include "arduino_boards.h"
#include <avr/io.h>
#define LED_BUILTIN _BV(PB5)

#include <util/delay.h>
#include "hal_uart.h"
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


#include <stdlib.h>
int main(void)
{
    DDRB |= LED_BUILTIN;
    uint16_t dtime = 1000;
    

    while( HAL_UART_ERR_OK != hal_uart_init(HAL_UART_BAUD_9600, HAL_UART_STOP_1BIT | HAL_UART_PARITY_EVEN | HAL_UART_CHAR_8BIT, 16u, 4u)){
        PORTB ^= LED_BUILTIN;
        delay_ms(100);
    }
    volatile uint16_t rx_count = 0;
    uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t));

    while(1){
        // hal_uart_getRxBufferCount(&rx_count);
        // if(0 < rx_count){
        //     hal_uart_readByte(data);
        //     hal_uart_sendByte(*data);
        // }
        hal_uart_sendByte(0x55);
        hal_temp(0x80);

        //hal_uart_sendByte(0x55u);
        // usart_read(&data);
        // usart_write(data);
        
        PORTB ^= LED_BUILTIN;
        delay_ms(dtime);
    }
}
