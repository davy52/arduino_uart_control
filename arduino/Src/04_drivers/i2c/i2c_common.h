#ifndef I2C_COMMON_H
#define I2C_COMMON_H

typedef enum {
    I2C_OK,
    I2C_NOT_OK,
    I2C_BUSY
} i2c_error_t;

typedef struct {
    uint8_t ack_enable : 1;
    uint8_t int_enable : 1;
    uint8_t general_call_enable;
} i2c_settings_t;


#define SCL A5
#define SDA A4

//TODO SCL_Freq = F_CPU/(16 + 2(TWBR) * Prescaler)



#endif /* I2C_COMMON_H */
