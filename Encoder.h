
#ifndef ENCODER_H_
#define ENCODER_H_
#include "Pins.h"


void ISR_EncoderA(void* handler_arg, cyhal_gpio_event_t event)
{
	Count_EncoderA++;

}
void ISR_EncoderB(void* handler_arg, cyhal_gpio_event_t event)
{
	Count_EncoderB++;

}

//Interrupts
cyhal_gpio_callback_data_t cb_A =
	{
	    .callback     = ISR_EncoderA
	};

cyhal_gpio_callback_data_t cb_B =
	{
	    .callback     = ISR_EncoderB
	};


void InitEncoders()
{
	printf("InitEncoders...\n\r");
    //Encoder A
    cyhal_gpio_init(EncoderA, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE, 0);
    cyhal_gpio_register_callback(EncoderA, &cb_A);
    cyhal_gpio_enable_event(EncoderA, CYHAL_GPIO_IRQ_RISE, 3, true);
    //Encoder B
    cyhal_gpio_init(EncoderB, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE, 0);
    cyhal_gpio_register_callback(EncoderB, &cb_B);
    cyhal_gpio_enable_event(EncoderB, CYHAL_GPIO_IRQ_RISE, 3, true);

}

void SetEncoderCountersZero()
{
	Count_EncoderA = 0;
	Count_EncoderB = 0;
}



#endif /* ENCODER_H_ */
