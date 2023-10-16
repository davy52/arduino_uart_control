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

#include "i2c_master.h"

void delay_ms(uint32_t ms){
    while(ms--){
        _delay_ms(1);
    }
}

int main(void)
{
    DDRB |= LED_BUILTIN;
    uint16_t dtime = 500;

    while(1){
        for(uint8_t i = 0; i < 6; i++){
            PORTB ^= LED_BUILTIN;
            delay_ms(dtime);
        }
        
        if(500 <= dtime){
            dtime = 100;
        }
        else{
            dtime = 500;
        }


    }

    
    
}
