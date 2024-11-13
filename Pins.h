

//This header will be used for :
//- To define all of our pins and globals in one place.
//- Most Initialization.

#ifndef PINS_H_
#define PINS_H_
#include "cy_retarget_io.h"
#include "cycfg.h"
#include "cycfg_capsense.h"
#include "cy_syslib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//Pins
//#define MotorA 					(P5_3)
//#define MotorB 					(P5_4)
//#define MotorC 					(P5_6)
//#define MotorD 					(P5_7)
#define MotorA 					(P10_2)
#define MotorB 					(P10_4)
#define MotorC 					(P10_3)
#define MotorD 					(P10_0)
#define TRIG_PIN_Sensor1 		(P9_1)
#define ECHO_PIN_Sensor1 		(P9_2)
#define TRIG_PIN_Sensor2 		(P9_4)
#define ECHO_PIN_Sensor2 		(P9_7)
#define SCL_PIN 				(P6_0)
#define SDA_PIN					(P6_1)
#define EncoderA 				(P12_0)
#define EncoderB 				(P12_1)
#define Button					(P0_4)
#define MPU_interrupt 			(P6_2)
//Adress MPU
#define MPU6050_ADDR			 0x68
#define PWR_MGMT_1   			 0x6B
#define ACCEL_XOUT_H 			 0x3B
#define ACCEL_YOUT_H        	 0x3D
#define ACCEL_ZOUT_H        	 0x3F
#define WHO_AM_I_REGISTER     	 0x75      //WHO_AM_I register voor MPU6050
#define EXPECTED_WHO_AM_I        0x68      //Verwachte waarde van het WHO_AM_I register
#define INT_PIN_CFG      		 0x37  	   // Interrupt pin configuration register
#define MOT_THRESHOLD    		 0x1F  	   // Motion threshold register
#define MREG_INT_ENABLE  		 0x38      // Interrupt enable register
#define MPU6050_MOTION_DETECT_EN 0x40     // Waarde om motion detectie interrupt in te schakelen (bit 6 in INT_ENABLE)

//Global defines
#define Frequentie				(10u)
#define CAPSENSE_INTR_PRIORITY  (7u)
#define EZI2C_INTR_PRIORITY     (6u)
#define ArraySize				 10
#define Distance_Max 			 25		   //Sensor Distance
#define OBSTACLE_DETECT_THRESHOLD  1       //Sensor precision
#define MPU_OUT_OF_RANGE_THRESHOLD 3 	   // Aantal opeenvolgende metingen buiten bereik voordat actie wordt ondernomen
#define MPU_IN_RANGE_THRESHOLD 	   2       // Aantal opeenvolgende metingen binnen bereik voordat motor opnieuw start
#define One_Meter_EncoderCount     235	   //235 = 1 Meter for the encoder
#define Turn_90_Degrees 		   130	   //130 = 90 degree turn
#define TILT_THRESHOLD_G     	   0.55	   //Threshold in g
#define NUM_MEASUREMENTS     	   5  	   //Aantal metingen voor een gemiddelde

//Globals
cyhal_pwm_t		pwm_MotorA;
cyhal_pwm_t 	pwm_MotorB;
cyhal_pwm_t 	pwm_MotorC;
cyhal_pwm_t 	pwm_MotorD;
cyhal_timer_t 	timer_obj_Sensor;
cyhal_i2c_t 	i2c;

//Motors
uint8_t Speed = 80;
bool VooruitActive = false;

//MPU
int16_t accel_x, accel_y, accel_z;
int16_t accel_x_sum = 0, accel_y_sum = 0, accel_z_sum = 0;
int measurement_count = 0;
volatile bool Motion_Detected = false;

//Encoders
uint32_t Count_EncoderA = 0;
uint32_t Count_EncoderB = 0;

//Main loop
uint8_t ArrayIndex = 0;
char MainArray[10];
bool Start = false;

//CapSense
cy_stc_scb_ezi2c_context_t ezi2c_context;
cyhal_ezi2c_t sEzI2C;
cyhal_ezi2c_slave_cfg_t sEzI2C_sub_cfg;
cyhal_ezi2c_cfg_t sEzI2C_cfg;
volatile bool capsense_scan_complete = false;

//Sensors
volatile uint32_t echo_start_time_1 = 0;
volatile uint32_t echo_duration_1 = 0;
volatile uint32_t echo_start_time_2 = 0;
volatile uint32_t echo_duration_2 = 0;


//End of program function
void EndOfProgram()
{
	printf("-------------------------------------------\n\r");
	//Zet hele array op '3' = case 3 = stopmotors.
	memset(MainArray, '3', sizeof(MainArray));
	ArrayIndex = 0;
	Start = false;
}



#endif /* PINS_H_ */


/*

_S1___S2_
|       |
A       B
|       |
|       |
C       D
|       |
---------

Links   : B & D
Rechts  : A & C

Volgorde op bordje : (boven naar onder)

Speed sensors
MCU
D
C
B
A
S1
S2


*/
