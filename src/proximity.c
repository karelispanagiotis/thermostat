#include "proximity.h"
#include "delay.h"
#include <platform.h>
#include <gpio.h>

void proximity_init(void)
{
	gpio_set_mode(P_PROX_TRIG, Output);
	gpio_set(P_PROX_TRIG, 0);
	gpio_set_mode(P_PROX_ECHO, Input);
}

float get_distance(void)
{
	gpio_set(P_PROX_TRIG, 0);
	delay_us(2);
	gpio_set(P_PROX_TRIG, 1);
	delay_us(10);
	gpio_set(P_PROX_TRIG, 0);
	
	while (!gpio_get(P_PROX_ECHO));
	
	/* We measure the pulse width with 60us resolution, which
	 * translates to aproximately 10mm error in distance measurement.
	 * 60us = 960 cycles*/
	int steps=0;
	while (gpio_get(P_PROX_ECHO))
	{
		steps++;
		delay_cycles(960);
	}
	
	// Return distance in cm
	return (float)steps*1.02f;
}
