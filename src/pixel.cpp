#include "pixel.h"
#include <Arduino.h>
#include "TFT_eSPI.h"
#include <stdlib.h>
#include "stdint.h"


uint8_t* initializePixels() {
	//int* pixels =  (int*) malloc(sizeof(int)*resolutionx*resolutiony);
	//Serial.print("pixel.cpp: initializePixels 1\n");
	uint8_t* pixels =  (uint8_t*) malloc(sizeof(uint8_t)*resolutionx*resolutiony);
	//Serial.print("pixel.cpp: initializePixels 2\n");
	if(pixels == NULL){
		//Serial.print("pixel.cpp: initializePixels 3\n");
	}
	for(int i=0;i<resolutionx*resolutiony;i++) {

		pixels[i] = WHITE; //Set color to white
		//Serial.print("Free Heap: ");
		//Serial.println(ESP.getFreeHeap());
		//Serial.print("Flash CHip Size: ");
		//Serial.println(ESP.getFlashChipSize());
	}
	return pixels;
}

int* getPos(int index) {
	//Serial.print("pixel.cpp: getPos 1\n");
	int* arr = (int*) malloc(sizeof(int)*2);
	//Serial.print("pixel.cpp: getPos 2\n");
	arr[0] = (int) (index/resolutionx)*4;
	arr[1] = (index%resolutionx)*4;
	return arr;
}

int getIndex(int x, int y) {
	//Serial.print("pixel.cpp: getIndex\n");
	return x*(resolutiony) + y;
}

void clearPixel(uint8_t* pixels) {
	//Serial.print("pixel.cpp: clearPixel\n");
	for(int i=0;i<resolutionx*resolutiony;i++) {
		pixels[i] = WHITE;
	}
}