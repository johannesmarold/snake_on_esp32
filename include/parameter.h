
#ifndef INCLUDE_PARAMETER_H_
#define INCLUDE_PARAMETER_H_
#include "snake.h"
#include "stdint.h"

// The FreeRTOS function "xTaskCreate()" only allows one void pointer (meaning one pointer to "data")
// to be passed. That's why we construct a struct, which holds all parameters (only pointers) to save some memory

typedef struct param {
	uint8_t* pixels;
	snake* s;
	apple* a;
	bool* game;
} param;





#endif /* INCLUDE_PARAMETER_H_ */
