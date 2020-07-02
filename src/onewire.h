#include <stdint.h>

#define SEARCH_ROM 	0xF0
#define READ_ROM		0x33
#define MATCH_ROM		0x55
#define SKIP_ROM		0xCC
#define ALARM_SEARCH 0xEC
#define	CONVERT_T		 0x44
#define WRITE_SCRATCHPAD 0X4E
#define READ_SCRATCHPAD  0xBE
#define COPY_SCRATCHPAD  0x48
#define RECALL_E2				 0xB8
#define READ_POWER_SUPPLY 0xB4

void onewire_init(void);

uint8_t onewire_transaction_init(void);

uint8_t onewire_read(void);

void onewire_write(uint8_t bit);

uint8_t onewire_read_byte(void);

void onewire_write_byte(uint8_t byte);


