#include <stdio.h>
#include <gpio.h>
#include <timer.h>
#include "lcd.h"
#include "proximity.h"
#include "delay.h"
#include "leds.h"
#include "thermometer.h"

#define WAKEUP_PERIOD 1000 //in ms 

/* Change the following macros according to your preference */
#define MIN_DIST 30			//in cm
#define TEMP_LOW 10.0f	//in Celsius
#define TEMP_HI  28.5f	//in Celsius

volatile float average, prev_average, current;
volatile int step; 
char str[2][16];

void timer_handler(void)
{
	step = (step + 1)%(120000/WAKEUP_PERIOD);
}

int main()
{
	leds_init();
	thermometer_init();
	proximity_init();
	lcd_init();
	timer_init(WAKEUP_PERIOD*1000);
	timer_set_callback(timer_handler);
	
	// Initialize variables to print valid data on start-up
	current = get_temprature(); 
	average = 24*current;
	
	timer_enable();
	while(1)
	{		
		int time_ms = step*WAKEUP_PERIOD;
		
		// Get temprature measurements
		if(time_ms==0)
		{
			prev_average = average/24;
			average = 0;
		}
		if(time_ms%5000==0)
		{
			current = get_temprature();
			average += current;
		}
		// LEDs Set
		gpio_set(P_LED_B, current<TEMP_LOW );
		gpio_set(P_LED_R, current>TEMP_HI  );
		gpio_set(P_LED_G, average>TEMP_HI  );
		
		// LCD Prints
		lcd_clear();
		str[0][0] = str[1][0] = 0;
		if(current<TEMP_LOW)
		{
			sprintf(str[0], "Cur: %.2f C", current);
			sprintf(str[1], "Low Temperature");
		}
		else if(current>TEMP_HI)
		{
			sprintf(str[0], "Cur: %.2f C", current);
			sprintf(str[1], "High Temperature");
		}
		else if(get_distance()<MIN_DIST)
		{
			sprintf(str[0], "Avg: %.2f C", prev_average);
			sprintf(str[1], "Cur: %.2f C", current);
		}
		else if(time_ms < 10000)
			sprintf(str[0], "Avg: %.2f C", prev_average);

		lcd_set_cursor(0, 0);
		lcd_print(str[0]);
		lcd_set_cursor(0, 1);
		lcd_print(str[1]);
		
		__WFI();
	}
}
