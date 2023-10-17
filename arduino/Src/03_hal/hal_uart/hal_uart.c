
#include "hal_uart.h"
#include "usart.h"
#include "ring_buffer.h"

typedef enum
{
    HAL_UART_IDLE,
    HAL_UART_RUNNING
} hal_uart_state_t;

static ring_buffer_handle_t tx_buffer_handle;
static ring_buffer_handle_t rx_buffer_handle;


static hal_uart_state_t hal_uart_state;

static inline hal_uart_err_t hal_uart_write_byte()
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    uint8_t data = 0;

    if (RB_ERR_OK != rb_pop(tx_buffer_handle, &data)){
        hal_uart_state = HAL_UART_IDLE;

        ret_val = HAL_UART_ERR_NOT_OK;
        return ret_val;
    }
    
    if (USART_ERR_OK != usart_write(data)){
        ret_val = HAL_UART_ERR_NOT_OK;
        return ret_val;
    }
    
    hal_uart_state = HAL_UART_RUNNING;
    return ret_val; 
}

hal_uart_err_t hal_uart_init(hal_uart_baud_t baud_rate, uint8_t settings, uint16_t rx_buffer_size, uint16_t tx_buffer_size)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    uint8_t char_size = (uint8_t)(settings & (7<<3)) >> 3; 
    uint8_t stopbit_mode = (uint8_t)((settings & (1<<2)) >> 2);
    uint8_t parity = (uint8_t)(settings & 0x3); 

    uint8_t double_speed = (uint8_t)((baud_rate & 0x8000) >> 15);
    uint16_t baud = (uint16_t)(baud_rate & ~(0x8000));

    usart_settings_t usart_settings = {
        .char_size = 0,
        .clk_polarity = 0,
        .data_reg_empty_int_en = 1,
        .rx_complete_int_en = 1,
        .tx_complete_int_en = 1,
        .double_speed = 0,
        .mode = USART_MODE_ASYNC,
        .multi_proc_mode = 0,
        .parity = 0,
        .prescaler = 0,
        .receiver_en = 1,
        .transmitter_en = 1,
        .stopbit_mode = 0
    };
    
    usart_settings.char_size = char_size;
    usart_settings.stopbit_mode = stopbit_mode;
    usart_settings.parity = parity;

    usart_settings.double_speed = double_speed;
    usart_settings.prescaler = baud;

    if(USART_ERR_OK != usart_init(usart_settings)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }
    
    if(RB_ERR_OK != rb_init(&tx_buffer_handle, tx_buffer_size)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }
    if(RB_ERR_OK != rb_init(&rx_buffer_handle, rx_buffer_size)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }
    
    hal_uart_state = HAL_UART_IDLE;

    return ret_val;
}

hal_uart_err_t hal_uart_sendByte(uint8_t data)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    if(RB_ERR_OK != rb_insert(tx_buffer_handle, data)){
        ret_val = HAL_UART_ERR_BUFF_FULL;
        return ret_val;
    } 
    
    if (HAL_UART_RUNNING != hal_uart_state){
        if (HAL_UART_ERR_OK != hal_uart_write_byte()){
            ret_val = HAL_UART_ERR_NOT_OK;
            return ret_val;
        }
    }

    return ret_val;
}


hal_uart_err_t hal_uart_sendBytes(uint8_t *data, uint16_t data_len)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    if(RB_ERR_OK != rb_insertMultiple(tx_buffer_handle, data, data_len)){
        ret_val = HAL_UART_ERR_BUFF_FULL;
        return ret_val;
    } 
    
    if (HAL_UART_RUNNING != hal_uart_state){
        if (HAL_UART_ERR_OK != hal_uart_write_byte()){
            ret_val = HAL_UART_ERR_NOT_OK;
            return ret_val;
        }
    }

    return ret_val;
}

hal_uart_err_t hal_uart_readByte(uint8_t *data)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    
    if(RB_ERR_OK != rb_pop(rx_buffer_handle, data)){
        ret_val = HAL_UART_ERR_BUFF_EMPTY;
        return ret_val;
    }

    return ret_val;
}

hal_uart_err_t hal_uart_readBytes(uint8_t *data, uint16_t data_len)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    
    if (rb_spaceUsed(rx_buffer_handle) < data_len){
        ret_val = HAL_UART_ERR_BUFF_EMPTY;
        return ret_val;
    }
    
    if (RB_ERR_OK != rb_popMultiple(rx_buffer_handle, data, &data_len)){
        ret_val = HAL_UART_ERR_BUFF_EMPTY;
        return ret_val;
    }

    return ret_val;
}

hal_uart_err_t hal_uart_getRxBufferCount(uint16_t *rx_buffer_count)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    *rx_buffer_count = rb_spaceUsed(rx_buffer_handle);
    return ret_val;
}

hal_uart_err_t hal_uart_response_init(uint8_t *format, uint8_t format_len, uint8_t *data, uint16_t *data_len)
{
    return 0;
}

// usart_data_reg_empty_int()
ISR(USART_UDRE_vect)
{
    uint8_t data = 0;

    if (RB_ERR_OK != rb_pop(tx_buffer_handle, &data)){
        hal_uart_state = HAL_UART_IDLE;
        return;
    }
    
    usart_write_int(data);
    return;
}

// usart_rx_int()
ISR(USART_RX_vect)
{
    rb_insert(rx_buffer_handle, usart_read_int());
    return;
}

void hal_temp(uint8_t data){
    rb_insert(tx_buffer_handle, data);
    usart_write(data);
}