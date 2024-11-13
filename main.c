
#if defined (CY_USING_HAL)
#include "cyhal.h"
#endif
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cycfg_capsense.h"
#include "FunctionsMotor.h"
#include "Sensor.h"
#include "Pins.h"
#include "MCU.h"
#include "Encoder.h"
#include "CapSense.h"
#include "Buttons.h"

int main(void)
{
    cybsp_init();
    __enable_irq();
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

	InitSensor();
	InitMotor();
	InitI2C();
	mpu6050_enable_interrupts();
	InitButton();
	InitEncoders();
    InitCapsenseTuner();
    InitCapsense();

    Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
    StartMotors();

    for (;;)
    {

		while ((ArrayIndex < ArraySize) && !Start) //blijven loopen tot de start is ingedrukt.
		{
		  if (capsense_scan_complete)
		  {
		    Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);
		    process_touch();
		    Cy_CapSense_RunTuner(&cy_capsense_context);
		    Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
		    capsense_scan_complete = false;
		   }
		}

		printf("Configurations Done! \n\r");
		Motion_Detected = false;
		cyhal_system_delay_ms(3000);


		for ( int i = 0 ;  i < ArraySize ; i++)
		{
			switch (MainArray[i])
			{
			case '0' :
				Vooruit();
				while( ((Count_EncoderB + Count_EncoderA)/2) <  One_Meter_EncoderCount)
				{
					//Sensor Check
					if(GetSensorDistance1() <= Distance_Max || GetSensorDistance2() <= Distance_Max)
					{
						StopMotors();
						while (GetSensorDistance1() <= Distance_Max || GetSensorDistance2() <= Distance_Max);
						Vooruit();
					}

					//MPU Check
					if(Motion_Detected)
					{
						StopMotors();
						while(Motion_Detected)
						{
							printf("Engine Stopped , Press Button (P0_4) to keep going.\n\r");
							cyhal_system_delay_ms(2000);
						}
						Vooruit();
					}

				    cyhal_system_delay_ms(5);
				}
				SetEncoderCountersZero();
				cyhal_system_delay_ms(5);
				break;
			case '1' :
				LinksDraaien();
				while(Count_EncoderB < Turn_90_Degrees )
				{
					if(Motion_Detected)
					{
						StopMotors();
						while(Motion_Detected)
						{
							printf("Engine Stopped , Press Button (P0_4) to keep going.\n\r");
							cyhal_system_delay_ms(2000);
						}
						LinksDraaien();
					}
				}
				SetEncoderCountersZero();
				cyhal_system_delay_ms(5);
				break;
			case '2' :
				RechtsDraaien();
				while(Count_EncoderA < Turn_90_Degrees )
				{
					if(Motion_Detected)
					{
						StopMotors();
						while(Motion_Detected)
						{
							printf("Engine Stopped , Press Button (P0_4) to keep going.\n\r");
							cyhal_system_delay_ms(2000);
						}
						RechtsDraaien();
					}
				}
				SetEncoderCountersZero();
				cyhal_system_delay_ms(5);
				break;
			case '3' :
				StopMotors();
				cyhal_system_delay_ms(5);
				break;
			default :
				printf("\rError Switch Case.\n\r");
				StopMotors();
				break;
			}
		}

		StopMotors();
		EndOfProgram();
		cyhal_system_delay_ms(1000);

    }

}

