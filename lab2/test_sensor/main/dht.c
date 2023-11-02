/*
 * dht.c
 *
 *  Created on: 2 thg 11, 2023
 *      Author: Administrator
 */

#ifndef MAIN_DHT_C_
#define MAIN_DHT_C_

#include "dht.h"

static esp_err_t init_i2c() {
	int i2c_master_port = 0;
	i2c_config_t conf = {
	    .mode = I2C_MODE_MASTER,
	    .sda_io_num = SDA_DHT_IO,         // select SDA GPIO specific to your project
	    .sda_pullup_en = GPIO_PULLUP_ENABLE,
	    .scl_io_num = SCL_DHT,         // select SCL GPIO specific to your project
	    .scl_pullup_en = GPIO_PULLUP_ENABLE,
	    .master.clk_speed = DHT_FREQ,  // select frequency specific to your project
	    .clk_flags = 0,                          // optional; you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here
	};
	i2c_param_config(I2C_NUM_0,  &conf);
	return i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}

esp_err_t i2c_example_master_write_slave(i2c_port_t i2c_num, uint8_t* data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( SLAVE_ADDRESS_DHT20 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_example_master_read_slave(i2c_port_t i2c_num, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( SLAVE_ADDRESS_DHT20 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

//bien luu tru gia tri temp va humid
uint16_t value_x10[2] = {0, 0};
char temp[20], humid[20];
//int status = INIT;

void dht20_init(void){
	//Set register when call a wrong reset
	uint8_t init[3];

	init[0] = 0xA8;
	init[1] = 0x00;
	init[2] = 0x00;
	// HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) init, 3, 0xFF);
	i2c_example_master_write_slave(I2C_NUM_0, (uint8_t*) init, 3);
	// HAL_Delay(20);

	init[0] = 0xBE;
	init[1] = 0x08;
	init[2] = 0x00;
	// HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) init, 3, 0xFF);
	i2c_example_master_write_slave(I2C_NUM_0, (uint8_t*) init, 3);
	// HAL_Delay(20);
}

void dht20_reset(uint8_t regist){
	//reset register
	uint8_t reset[3], reply[3];

	reset[0] = regist;
	reset[1] = 0x00;
	reset[2] = 0x00;
 	// HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) reset, 3, 0xFF);
 	i2c_example_master_write_slave(I2C_NUM_0, (uint8_t*) reset, 3);
	// HAL_Delay(10);

//	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) reply, 3, 0xFF);
 	i2c_example_master_read_slave(I2C_NUM_0, (uint8_t*) reply, 3);
	reset[0] = 0xB0 | regist;
	reset[1] = reply[1];
	reset[2] = reply[2];
	// HAL_Delay(10);

	// HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) reset, 3, 0xFF);
	i2c_example_master_write_slave(I2C_NUM_0, (uint8_t*) reset, 3);
}

void dht20_start(void){
//	//query the DHT20
//	uint8_t status[1];
//	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) status, 1, 0xFF);
//
//	if((status[0] & 0x18) != 0x18){
//		dht20_reset(0x1B);
//		dht20_reset(0x1C);
//		dht20_reset(0x1E);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
//	}
//
//	if ((status[0] & 0x18) == 0x18){
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, SET);
//	}
//
//	uint8_t data[3] = {0xAC, 0x33, 0x00};
//
//	//HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) trigger, 1, 0xFF);
//	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_DHT20, (uint8_t*) data, 3, 0xFF);
//	HAL_Delay(80);
}

void dht20_read(uint16_t* value){
	dht20_start();
	uint8_t data[7];
	uint32_t Temper = 0, Humid = 0;
//	HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS_DHT20 | 0x01, (uint8_t*) data, 7, 0xFF);
	i2c_example_master_read_slave(I2C_NUM_0, (uint8_t*) data, 7);

	//Humid
	Humid = (Humid | data[1]) << 8;
	Humid = (Humid | data[2]) << 8;
	Humid = Humid | data[3];
	Humid = Humid >> 4;
    Humid = (Humid * 100 * 10 / 1024 / 1024);
    value[0] = Humid;

	//Temperature
    Temper = (Temper | data[3]) << 8;
    Temper = (Temper | data[4]) << 8;
    Temper = Temper | data[5];
    Temper = Temper & 0xfffff;
    Temper = Temper*200*10/1024/1024 - 500;
	value[1] = Temper;

}

//void init_reading(void){
//	dht20_init();
//	dht20_read(value_x10);
//}

//void reading_fsm_run(void){
//	switch(status){
//		case INIT:
//			setTimer1(1);
//			status = READ;
//			break;
//		case READ:
//			if (timer1_flag == 1){
//				dht20_read(value_x10);
//				//11011111 is degree character (manual)
//				sprintf(temp, "Temp:  %d.%d %cC",value_x10[1]/10,value_x10[1]%10 ,0b11011111);
//				sprintf(humid,"Humid: %01d.%d %%   ",value_x10[0]/10,value_x10[0]%10);
//				setTimer1(300);
//			}
//			break;
//		default:
//			break;
//	}
//	lcd_show_value();
//}

#endif /* MAIN_DHT_C_ */
