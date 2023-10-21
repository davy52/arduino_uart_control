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

// #define arduino_pro_mini_3v3
#include "arduino_boards.h"

#include <util/delay.h>
#include "hal_uart.h"
#include "usart.h"

#include "debug.h"

// FUSES = {
//     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
//     .high = HFUSE_DEFAULT,
//     .extended = EFUSE_DEFAULT
// };

// LOCKBITS = LOCKBITS_DEFAULT;

// void delay_ms(uint32_t ms){
//     while(ms--){
//         _delay_ms(1);
//     }
// }


#include <stdlib.h>
int main(void)
{
    DDRB |= LED_BUILTIN;

    uint16_t dtime = 1000;
    uint16_t rx_count = 0;
    uint8_t data[16];
    hal_uart_err_t err;
    
    blink_dur(2, 5);

    // init uart
    while( HAL_UART_ERR_OK != hal_uart_init(F_CPU, 9600u, HAL_UART_STOP_1BIT | HAL_UART_PARITY_EVEN | HAL_UART_CHAR_8BIT, 16u, 16u)){
        PORTB ^= LED_BUILTIN;
        delay_ms(100);
    }


    // show end of main init on led
    blink_dur(2, 5);
    sei();

    while(1){
        
        hal_uart_getRxBufferCount(&rx_count);
        if(0 < rx_count){
            err = hal_uart_readBytes(data, rx_count);
            err = hal_uart_sendBytes(data, rx_count);
        }

        // delay_ms(dtime);
    }
}
