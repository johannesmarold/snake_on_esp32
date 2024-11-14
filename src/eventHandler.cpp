#include "eventHandler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "freertos/semphr.h"
#include "snake.h"
#include "pixel.h"
#include "parameter.h"
#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "iot_button.h"
#include "stdint.h"

int lastTimePressedLeft = 0;
int lastTimePressedRight = 0;


void eventLoop(void* v) {
	param* p = (param*) v;
	int	oldButtonLeft=digitalRead(leftKey), oldButtonRight=digitalRead(rightKey);
	pinMode(leftKey, INPUT);
	pinMode(rightKey, INPUT);
	TickType_t lastWakeTime = xTaskGetTickCount();
	const TickType_t delta = pdMS_TO_TICKS(1000/(Hz));
	//Serial.print("eventHandler.cpp: eventLoop 1\n");
	while(1) {
		TickType_t loopStartTime = xTaskGetTickCount();
		if(checkSnakeBite(p->s, p->a)) {
			xTaskCreate(handleSnakeBite,"Snake Bite Handler",5000,v,10,NULL);
		}
		//Serial.print("eventHandler.cpp: eventLoop 2\n");
		if(checkSnakeTouch(p->s)) {
			*(p->game) = false;
		}
		//Serial.print("eventHandler.cpp: eventLoop 3\n");
		if(checkButtonLeft(leftKey, &oldButtonLeft)) {
			xTaskCreate(handleButtonLeft,"Snake Bite Handler",5000,v,10,NULL);
		}
		//Serial.print("eventHandler.cpp: eventLoop 4\n");
		if(checkButtonRight(rightKey, &oldButtonRight)) {
			xTaskCreate(handleButtonRight,"Snake Bite Handler",5000,v,10,NULL);
		}
		//Serial.print("eventHandler.cpp: eventLoop 5\n");
		
		TickType_t loopEndTime = xTaskGetTickCount();

		TickType_t remainingTime = 200/portTICK_PERIOD_MS;//delta - (loopEndTime-loopStartTime);
		if(remainingTime > 0) {
			vTaskDelay(remainingTime);
		}
	}
}

bool checkSnakeBite(snake* s, apple* a) {
	//Serial.print("eventHandler.cpp: checkSnakeBite 1\n");
	//printSnake(s);

	if(getSnakeHead(s)->x == a->body->x && getSnakeHead(s)->y == a->body->y) {
		return true;
	} else {
		return false;
	}

}

bool checkSnakeTouch(snake* s) {
	//Serial.print("eventHandler.cpp: checkSnakeTouch 1\n");
	pos* head = getSnakeHead(s);
	pos* body = s->body;//->next;
	//Serial.print("eventHandler.cpp: checkSnakeTouch 2\n");
	//printSnake(s);
	while(body->next) {
		if(head->x == body->x && head->y == body->y) {
			//Serial.print("snakeTouech true\n");
			return true;
		}
		body = body->next;
	}
	//Serial.print("eventHandler.cpp: checkSnakeTouch 3\n");
	if(getSnakeHead(s)->x == resolutionx) { // Check if snake hits right border
		return true;
	}
	//Serial.print("eventHandler.cpp: checkSnakeTouch 4\n");
	if(getSnakeHead(s)->y == resolutiony) { // Check if snake hits bottom/upper border
		return true;
	}

	// NOTE: We do not check for x=-1 or y=-1. We call handleSnakeTouch immediately, to prevent drawing on negative positions (snakeMove already checks for us :) 
	return false;
}

bool checkButtonLeft(int button, int* old) {
	//Serial.print("eventHandler.cpp: checkButtonLeft 1\n");
	int currentTime = millis();

	if(digitalRead(button) && currentTime - lastTimePressedLeft > inputDelay) {
		//Serial.print("eventHandler.cpp: checkButtonLeft 2\n");
		*old = digitalRead(button);
		lastTimePressedLeft = currentTime;
		return true;
	}
	return false;
}

bool checkButtonRight(int button, int* old) {
	//Serial.print("eventHandler.cpp: checkButtonRight 1\n");
	int currentTime = millis();

	if(digitalRead(button) && currentTime - lastTimePressedRight > inputDelay) {
		////Serial.print("eventHandler.cpp: checkButtonRight 2\n");
		*old = digitalRead(button);
		lastTimePressedRight = currentTime;
		return true;
	}
	return false;
}

void handleSnakeBite(void* v) {
	////Serial.print("eventHandler.cpp: handleSnakeBite 1\n");
	param* p = (param*)v;
	////Serial.print("eventHandler.cpp: handleSnakeBite 2\n");
	snakeGrow(p->s);
	//Serial.print("eventHandler.cpp: handleSnakeBite 3\n");
	moveApple(p->s, p->a, p->pixels);
	//Serial.print("eventHandler.cpp: handleSnakeBite 4\n");
	vTaskDelete( NULL ); // Task kills itself
}

void handleButtonLeft(void* v) {
	param* p = (param*) v;
	//Serial.print("eventHandler.cpp: handleButtonLeft 1\n");
	p->s->orientation++;
	//Serial.print("eventHandler.cpp: handleButtonLeft 2\n");
	if(p->s->orientation > 3) {
		p->s->orientation = 0;
	}
	vTaskDelete( NULL );
}

void handleButtonRight(void* v) {
	param* p = (param*)v;
	//Serial.print("eventHandler.cpp: handleButtonRight 1\n");
	p->s->orientation--;
	//Serial.print("eventHandler.cpp: handleButtonRight 2\n");
	if(p->s->orientation < 0) {
		p->s->orientation = 3;
	}
	vTaskDelete( NULL );
}

void handleGameOver(TFT_eSprite * spr, int buttonStateLeft, int buttonStateRight, uint8_t* pixels, snake* s) {
		spr->fillScreen(TFT_WHITE);

		spr->setTextColor(TFT_BLACK, TFT_WHITE);
		spr->setFreeFont(&FreeSansBoldOblique18pt7b);
		spr->drawString("GAME OVER",10,120,GFXFF);
		spr->pushSprite(0, 0);
		while(1){
		buttonStateLeft = digitalRead(leftKey);   //left
		buttonStateRight = digitalRead(rightKey);
			if(buttonStateLeft != digitalRead(leftKey) || buttonStateRight != digitalRead(rightKey)){
				break;
			}
		}
		// start new Game
		clearPixel(pixels);
		freeSnake(s);
		s = initSnake();
}


void printSnake(snake* s) {
	pos* head = getSnakeHead(s);
	//pos* body = s->body->next;
	pos* body = s->body;
	Serial.printf("Snake Head x: %d, y: %d\t", head->x, head->y);
	while(body) {
		Serial.printf("Snake body x: %d, y: %d\t", body->x, body->y);
		body = body->next;
	}
	Serial.print("\n");
}
