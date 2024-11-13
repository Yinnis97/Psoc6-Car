
#ifndef MCU_H_
#define MCU_H_

#include "Pins.h"

//Init I2C
void InitI2C()
{
	printf("InitI2C...\n\r");

	cyhal_i2c_cfg_t i2c_cfg = {
	    .is_slave = false,
	    .address = 0,
	    .frequencyhal_hz = 400000
	};

    cyhal_i2c_init(&i2c, SDA_PIN, SCL_PIN , NULL);
    cyhal_i2c_configure(&i2c, &i2c_cfg);

    uint8_t data[2];

    // Wake up the MPU6050 (PWR_MGMT_1 naar 0x00)
    data[0] = PWR_MGMT_1;
    data[1] = 0x00;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, data, 2, 0, true);
}

void ISR_mpu6050(void *callback_arg, cyhal_gpio_event_t event)
{
	StopMotors();
	Motion_Detected = true;
	printf("ISR activated\n\r");
}

cyhal_gpio_callback_data_t MPU6050_cb_data = {
    .callback = ISR_mpu6050,
    .callback_arg = NULL
};

void mpu6050_enable_interrupts()
{
    uint8_t buffer[2];
    uint8_t motion_threshold = 15;

    // Reset alle interne registers
    buffer[0] = 0x68;
    buffer[1] = 0x07;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Configureer interrupt pin gedrag
    buffer[0] = INT_PIN_CFG;
    buffer[1] = 0x10;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Stelt accelerometergevoeligheid in
    buffer[0] = 0x1C;
    buffer[1] = 0x01;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Stel bewegingsdrempel in
    buffer[0] = MOT_THRESHOLD;
    buffer[1] = motion_threshold;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Stel duur van bewegingsdetectie in op 10ms
    buffer[0] = 0x20;
    buffer[1] = 10;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Instellen van motion decrement en accelerometer opstartvertraging
    buffer[0] = 0x69;
    buffer[1] = 15;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Schakel motion interrupt in
    buffer[0] = 0x38;
    buffer[1] = 0x40;
    cyhal_i2c_master_write(&i2c, MPU6050_ADDR, buffer, sizeof(buffer), 0, true);

    // Configureer GPIO voor interrupt pin
    cyhal_gpio_init(MPU_interrupt, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE,0);
    cyhal_gpio_register_callback(MPU_interrupt, &MPU6050_cb_data);
    cyhal_gpio_enable_event(MPU_interrupt, CYHAL_GPIO_IRQ_RISE, 3, true);
}

#endif /* MCU_H_ */
