#include "pixel.h"
#include <Arduino.h>
#include "TFT_eSPI.h"
#include <stdlib.h>
#include "stdint.h"


uint8_t* initializePixels() {
	uint8_t* pixels =  (uint8_t*) malloc(sizeof(uint8_t)*resolutionx*resolutiony);
	assert(pixels != NULL);
	for(int i=0;i<resolutionx*resolutiony;i++) {
		pixels[i] = WHITE; //Set color to white
	}
	return pixels;
}

int* getPos(int index) {
	int* arr = (int*) malloc(sizeof(int)*2);
	arr[0] = (int) (index/resolutionx)*4;
	arr[1] = (index%resolutionx)*4;
	return arr;
}

int getIndex(int x, int y) {
	return x*(resolutiony) + y;
}

void clearPixel(uint8_t* pixels) {
	for(int i=0;i<resolutionx*resolutiony;i++) {
		pixels[i] = WHITE;
	}
}