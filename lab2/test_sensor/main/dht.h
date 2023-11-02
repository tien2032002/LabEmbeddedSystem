/*
 * dht.h
 *
 *  Created on: 2 thg 11, 2023
 *      Author: Administrator
 */

#ifndef MAIN_DHT_H_
#define MAIN_DHT_H_

#include <stdio.h>
#include "main.h"
#include "driver/i2c.h"

#define SLAVE_ADDRESS_DHT20 (0x38 << 1)
#define SDA_DHT_IO 33 //dht sda pin
#define SCL_DHT 36 //dht scl pin
#define DHT_FREQ 8000
#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

//variable use for storing tempurature and humid value

extern uint16_t value_x10[2];
extern char temp[20],humid[20];

static esp_err_t init_i2c(void);

void dht20_init(void); //init module

void dht20_reset(uint8_t);//reset module

void dht20_read(uint16_t*);//read sensor

void dht20_start(void);//start sensor

void init_reading(void);



#endif /* MAIN_DHT_H_ */
