
#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "Pins.h"


void ISR_Button(void* callback_arg, cyhal_gpio_event_t event)
{
	Motion_Detected = false;
	printf("Motion = %d\n\r",Motion_Detected);
}

cyhal_gpio_callback_data_t cb_Button = {
    .callback = ISR_Button,
    .callback_arg = NULL
};

//Init button P0_4
void InitButton(void)
{
    printf("InitButtons...\n\r");
    cyhal_gpio_init(Button, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE, 0);
	cyhal_gpio_register_callback(Button, &cb_Button);
	cyhal_gpio_enable_event(Button, CYHAL_GPIO_IRQ_RISE, 3, true);
}


#endif /* BUTTONS_H_ */
