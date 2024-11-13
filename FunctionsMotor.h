

#ifndef FUNCTIONSMOTOR_H_
#define FUNCTIONSMOTOR_H_
#include "Pins.h"
#include "cy_retarget_io.h"


void InitMotor()
{
	printf("InitMotor...\n\r");
    cyhal_pwm_init(&pwm_MotorA, MotorA, NULL);
    cyhal_pwm_init(&pwm_MotorB, MotorB, NULL);
    cyhal_pwm_init(&pwm_MotorC, MotorC, NULL);
    cyhal_pwm_init(&pwm_MotorD, MotorD, NULL);
}

void StartMotors()
{
	cyhal_pwm_start(&pwm_MotorA);
	cyhal_pwm_start(&pwm_MotorB);
	cyhal_pwm_start(&pwm_MotorC);
	cyhal_pwm_start(&pwm_MotorD);
}

void StopMotors()
{

	printf("Stopped\n\r");
    cyhal_pwm_set_duty_cycle(&pwm_MotorA, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorB, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorC, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorD, 0, Frequentie);
    cyhal_gpio_write(MotorA, 0);
    cyhal_gpio_write(MotorB, 0);
    cyhal_gpio_write(MotorC, 0);
    cyhal_gpio_write(MotorD, 0);
    VooruitActive = false;
}

void RechtsDraaien()
{
	printf("Rechts\n\r");
    cyhal_pwm_set_duty_cycle(&pwm_MotorA, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorB, 80, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorC, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorD, 80, Frequentie);
}

void LinksDraaien()
{
	printf("Links\n\r");
    cyhal_pwm_set_duty_cycle(&pwm_MotorA, 80, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorB, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorC, 80, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorD, 0, Frequentie);
}

void Vooruit()
{
	printf("Vooruit\n\r");
    cyhal_pwm_set_duty_cycle(&pwm_MotorA, Speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorB, Speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorC, Speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorD, Speed, Frequentie);
    VooruitActive = true;
}



#endif
