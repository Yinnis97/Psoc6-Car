
#ifndef CAPSENSE_H_
#define CAPSENSE_H_
#include "Pins.h"
#include <stdlib.h>
#include "cy_capsense_structure.h"
extern cy_stc_capsense_context_t cy_capsense_context;

static void capsense_isr(void)
{
    Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);
}


void capsense_callback(cy_stc_active_scan_sns_t * ptrActiveScan)
{
    capsense_scan_complete = true;
}


void process_touch(void) {
    uint32_t button0_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON0_WDGT_ID, CY_CAPSENSE_BUTTON0_SNS0_ID, &cy_capsense_context);
    uint32_t button1_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON1_WDGT_ID, CY_CAPSENSE_BUTTON1_SNS0_ID, &cy_capsense_context);

    static uint32_t button0_status_prev = 0;
    static uint32_t button1_status_prev = 0;

    cy_stc_capsense_touch_t *slider_touch_info = Cy_CapSense_GetTouchInfo(CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);
    uint8_t slider_touch_status = slider_touch_info->numPosition;
    uint16_t slider_pos = slider_touch_info->ptrPosition->x;
    uint16_t xResolution = cy_capsense_context.ptrWdConfig[CY_CAPSENSE_LINEARSLIDER0_WDGT_ID].xResolution;

    static uint8_t last_slider_position = 0;  // Holds the previous slider position
    static bool  flick_detected = false;  // Indicates if a slide gesture is in progress

    //Drive Straight
    if((button0_status != 0U) && (button0_status != button0_status_prev))
    {
		MainArray[ArrayIndex] = '0';
		printf("\rClick : drive 1 meter at position %d , code for 1 meter: %c\n\r",ArrayIndex,MainArray[ArrayIndex]);
		ArrayIndex++;
    }

    //Start
    if((button1_status != 0U) && (button1_status != button1_status_prev))
    {
    	for (int i = 0; i < sizeof(MainArray); i++)
    	{
    	  MainArray[ArrayIndex+i] = '3';
    	}
    	Start = true;
    }

    if (slider_touch_status != 0U)
    {
        if (!flick_detected)
        {
            // Detect beginning of swipe based on starting position
            if (slider_pos < (xResolution / 3))
            {
            	flick_detected = true;
                last_slider_position = 2;  // Left edge detected
            }
            else if (slider_pos > (2 * xResolution / 3))
            {
            	flick_detected = true;
                last_slider_position = 3;  // Right edge detected
            }
        }
        else
        {
            // Swipe completed from left to right
            if (last_slider_position == 2 && slider_pos > (2 * xResolution / 3))
            {
				MainArray[ArrayIndex] = '2';
				printf("\rFlick to right at position %d, code: %c\n\r", ArrayIndex, MainArray[ArrayIndex]);
				ArrayIndex++;
				flick_detected = false;
            }
            // Swipe completed from right to left
            else if (last_slider_position == 3 && slider_pos < (xResolution / 3))
            {
                MainArray[ArrayIndex] = '1';
                printf("\rFlick to left at position %d, code: %c\n\r", ArrayIndex, MainArray[ArrayIndex]);
                ArrayIndex++;
                flick_detected = false;
            }
        }
    }
    else
    {
    	 flick_detected = false;
    }

    button0_status_prev = button0_status;
    button1_status_prev = button1_status;
}


void InitCapsense(void)
{
	printf("\rInitCapsense...\r\n");
    /* CapSense interrupt configuration parameters */
    static const cy_stc_sysint_t capSense_intr_config =
    {
        .intrSrc = csd_interrupt_IRQn,
        .intrPriority = CAPSENSE_INTR_PRIORITY,
    };

    /* Capture the CSD HW block and initialize it to the default state. */
    Cy_CapSense_Init(&cy_capsense_context);

    /* Initialize CapSense interrupt */
    cyhal_system_set_isr(csd_interrupt_IRQn, csd_interrupt_IRQn, CAPSENSE_INTR_PRIORITY, &capsense_isr);
    NVIC_ClearPendingIRQ(capSense_intr_config.intrSrc);
    NVIC_EnableIRQ(capSense_intr_config.intrSrc);

   Cy_CapSense_Enable(&cy_capsense_context);

    /* Assign a callback function to indicate end of CapSense scan. */
    Cy_CapSense_RegisterCallback(CY_CAPSENSE_END_OF_SCAN_E, capsense_callback, &cy_capsense_context);

}


static void InitCapsenseTuner(void)
{
	printf("\rInitCapsenseTuner...\r\n");

    /* Configure Capsense Tuner as EzI2C Slave */
    sEzI2C_sub_cfg.buf = (uint8 *)&cy_capsense_tuner;
    sEzI2C_sub_cfg.buf_rw_boundary = sizeof(cy_capsense_tuner);
    sEzI2C_sub_cfg.buf_size = sizeof(cy_capsense_tuner);
    sEzI2C_sub_cfg.slave_address = 8U;

    sEzI2C_cfg.data_rate = CYHAL_EZI2C_DATA_RATE_400KHZ;
    sEzI2C_cfg.enable_wake_from_sleep = false;
    sEzI2C_cfg.slave1_cfg = sEzI2C_sub_cfg;
    sEzI2C_cfg.sub_address_size = CYHAL_EZI2C_SUB_ADDR16_BITS;
    sEzI2C_cfg.two_addresses = false;

    cyhal_ezi2c_init(&sEzI2C, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL, &sEzI2C_cfg);
}

#endif /* CAPSENSE_H_ */



