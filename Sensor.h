#ifndef SENSOR_H_
#define SENSOR_H_

#include "Pins.h"

void ISR_echo_1(void* callback_arg, cyhal_gpio_event_t event)
{
    uint32_t current_time = cyhal_timer_read((cyhal_timer_t*) callback_arg);

    if (event == CYHAL_GPIO_IRQ_RISE)
    {
        echo_start_time_1 = current_time;
    }
    else if (event == CYHAL_GPIO_IRQ_FALL)
    {
        echo_duration_1 = current_time - echo_start_time_1;
    }
}

void ISR_echo_2(void* callback_arg, cyhal_gpio_event_t event)
{
    uint32_t current_time = cyhal_timer_read((cyhal_timer_t*) callback_arg);

    if (event == CYHAL_GPIO_IRQ_RISE)
    {
        echo_start_time_2 = current_time;
    }
    else if (event == CYHAL_GPIO_IRQ_FALL)
    {
        echo_duration_2 = current_time - echo_start_time_2;
    }
}

float GetSensorDistance1()
{

    return echo_duration_1 / 0.58f; // Converteer tijd naar afstand
}

float GetSensorDistance2()
{
    return echo_duration_2 / 0.58f; // Converteer tijd naar afstand
}

cyhal_gpio_callback_data_t cb_data_1 = {
    .callback = ISR_echo_1,
    .callback_arg = &timer_obj_Sensor
};

cyhal_gpio_callback_data_t cb_data_2 = {
    .callback = ISR_echo_2,
    .callback_arg = &timer_obj_Sensor
};

void Sensor_Trigger(void)
{
	cyhal_gpio_write(TRIG_PIN_Sensor1 , 1);
	cyhal_system_delay_us(10);
	cyhal_gpio_write(TRIG_PIN_Sensor1 , 0);
	cyhal_gpio_write(TRIG_PIN_Sensor2 , 1);
	cyhal_system_delay_us(10);
	cyhal_gpio_write(TRIG_PIN_Sensor2 , 0);
}

void ISR_timer_Sensor(void* callback_arg, cyhal_timer_event_t event)
{
	if (event == CYHAL_TIMER_IRQ_TERMINAL_COUNT)
	{
		Sensor_Trigger();
	}
}

//Init for all sensor pins and timer
void InitSensor()
{
	printf("InitSensor...\n\r");
    cyhal_gpio_init(TRIG_PIN_Sensor1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_gpio_init(ECHO_PIN_Sensor1, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    cyhal_gpio_init(TRIG_PIN_Sensor2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    cyhal_gpio_init(ECHO_PIN_Sensor2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);

    cyhal_gpio_register_callback(ECHO_PIN_Sensor1, &cb_data_1);
    cyhal_gpio_enable_event(ECHO_PIN_Sensor1, CYHAL_GPIO_IRQ_BOTH, 1, true);
    cyhal_gpio_register_callback(ECHO_PIN_Sensor2, &cb_data_2);
    cyhal_gpio_enable_event(ECHO_PIN_Sensor2, CYHAL_GPIO_IRQ_BOTH, 1, true);

    const cyhal_timer_cfg_t timer_cfg2 = {
       .compare_value = 0,
       .period = 999,
       .direction = CYHAL_TIMER_DIR_UP,
       .is_compare = false,
       .is_continuous = true,
       .value = 0
    };

    cyhal_timer_init(&timer_obj_Sensor, NC, NULL);
    cyhal_timer_configure(&timer_obj_Sensor, &timer_cfg2);
    cyhal_timer_set_frequency(&timer_obj_Sensor, 10000); // 10 kHz for 100 ms intervals
	cyhal_timer_register_callback(&timer_obj_Sensor, ISR_timer_Sensor, NULL);
	cyhal_timer_enable_event(&timer_obj_Sensor, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3, true);
	cyhal_timer_start(&timer_obj_Sensor);
}

#endif /* SENSOR_H_ */
