#include "thermometer.h"
#include "onewire.h"

void thermometer_init()
{
	onewire_init();
}

float get_temprature()
{
	onewire_transaction_init();
	onewire_write_byte(SKIP_ROM);
	onewire_write_byte(CONVERT_T);
	
	while(!onewire_read())
		;
	
	onewire_transaction_init();
	onewire_write_byte(SKIP_ROM);
	onewire_write_byte(READ_SCRATCHPAD);
	
	int8_t scratchpad[9];
	for(int i=0; i<9; i++)
		scratchpad[i] = onewire_read_byte();
	
	int16_t val = *(int16_t *)(&scratchpad[0]);
	return val * 0.0625;
}
