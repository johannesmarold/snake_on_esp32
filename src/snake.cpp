#include "snake.h"
#include "Arduino.h"
#include "pixel.h"
#include "esp_random.h"
#include <stdlib.h>
#include "stdint.h"
#include "TFT_eSPI.h"

void snakeDraw(snake* s, uint8_t* pixels) {
	//Serial.print("snake.cpp: snakeDraw 1\n");
	pos* p = s->body;
	//Serial.print("snake.cpp: snakeDraw 2\n");
	while(p) {

		for (size_t i = 0; i < pixelResolution; i++)
		{
			for (size_t j = 0; j < pixelResolution; j++)
			{
				// coloring vertical edges
				if (j > 0 && j < pixelResolution-1 && (i == 0 || i == pixelResolution-1)) {
					pixels[getIndex(p->x+j,p->y+i)] = BLACK;
				}
				// coloring horizontal edges
				if ((j == 0 || j == pixelResolution-1) && i > 0 && i < pixelResolution-1) {
					pixels[getIndex(p->x+j,p->y+i)] = BLACK;
				}
			}
		}
		
		p = p->next;
	}
	
}

pos* getSnakeHead(snake* s) {
	//Serial.print("snake.cpp: getSnakeHead 1\n");
	pos* p = s->body;
	//Serial.print("snake.cpp: getSnakeHead 2\n");
	while(p->next) {
		p = p->next;
	}
	return p;
}

snake* initSnake(){
	snake* s = (snake*) malloc(sizeof(snake));
	//Serial.print("snake.cpp: initSnake \n");
	s->body = (pos*)malloc(sizeof(pos));
	s->body->x = (resolutionx/2) - (resolutionx/2)%pixelResolution; // start position always in the middle
	s->body->y = (resolutiony/2) - (resolutiony/2)%pixelResolution;
	s->body->next = NULL;
	s->body->color= BLACK;
	s->orientation = 2; // 0: runs left; 1: runs up; 2: runs right; 3: runs down

	return s;
}

void freeSnake(snake* s) {
	//Serial.print("snake.cpp: freeSnake 1\n");
	pos* p = s->body;
	pos* p2 = s->body;
	//Serial.print("snake.cpp: freeSnake 2\n");
	while(p->next) {
		p2 = p->next;
		free(p);
		p = p2;
	}
	//Serial.print("snake.cpp: freeSnake 3\n");
	free(p); // Note: The last element does not get free'd without this line !
	free(s); // Finally free snake
}

void snakeGrow(snake* s) {
	//Serial.print("snake.cpp: snakeGrow 1\n");
	pos* body = s->body;
	pos* newElem = (pos *) malloc(sizeof(pos));
	//Serial.print("snake.cpp: snakeGrow 2\n");
	switch (s->orientation) // 0 up, 1 left, 2 down, 3 right
	{
	case 0:
		newElem->x = body->x;
		newElem->y = body->y+1;
		break;
	case 1:
		newElem->x = body->x+1;
		newElem->y = body->y;
		break;
	case 2:
		newElem->x = body->x;
		newElem->y = body->y-1;
		break;
	case 3:
		newElem->x = body->x-1;
		newElem->y = body->y;
		break;
	default:
		break;
	}
	newElem->color = BLACK;
	newElem->next = body;
	////Serial.print("snake.cpp: snakeGrow 3\n");
	s->body = newElem;
}

void appleDraw(apple* a, uint8_t* pixels) {

	for (size_t i = 0; i < pixelResolution; i++)
	{
		for (size_t j = 0; j < pixelResolution; j++)
		{
			// coloring vertical edges
			if ((i == 0 || i == pixelResolution-1) && (j == 0 || j == pixelResolution-1)) {
				continue;
			}
			pixels[getIndex(a->body->x+j,a->body->y+i)] = BLACK;
		}
	}
}

void moveApple(snake* s, apple* a, uint8_t* pixels) {
	int x = esp_random()%((resolutionx-1)/pixelResolution);
	int y = esp_random()%((resolutiony-1)/pixelResolution);

	x = (x*pixelResolution);
	y = (y*pixelResolution);

	////Serial.print("snake.cpp: moveApple 1\n");
	while(pixels[getIndex(x,y)] == BLACK) // This function assumes snake is black !
	{
		x = esp_random()%((resolutionx-1)/pixelResolution);
		y = esp_random()%((resolutiony-1)/pixelResolution);

		x = (x*pixelResolution);
		y = (y*pixelResolution);
	}
	//Serial.print("snake.cpp: moveApple 2\n");
	a->body->x = x;
	a->body->y = y;

	//Serial.print("snake.cpp: moveApple 3\n");
	appleDraw(a, pixels);
}

void snakeMove(snake* s, uint8_t* pixels, bool* game) {
	//Serial.print("snake.cpp: snakeMove 1\n");
	pos* body = s->body;
	//Serial.printf("snake.cpp: snakeMove 2: x: %d, y: %d\n", body->x, body->y);
	if(body->next != NULL){
	while(body->next) {
		body->x = body->next->x;
		body->y = body->next->y;
		body = body->next;
		
	}}
	//Serial.print("snake.cpp: snakeMove 3\n");	
	// Note: We purposefully skip the head of the snake, because only the movement of the head
	// is determined by the orientation
	//Serial.print("snake.cpp: snakeMove 4\n");
	switch(s->orientation) {
	case 0:
		//getSnakeHead(s)->y--;
		getSnakeHead(s)->y = getSnakeHead(s)->y-pixelResolution;
		break;
	case 1:
		//getSnakeHead(s)->x--;
		getSnakeHead(s)->x = getSnakeHead(s)->x-pixelResolution;
		break;
	case 2:
		//getSnakeHead(s)->y++;
		getSnakeHead(s)->y = getSnakeHead(s)->y+pixelResolution;
		break;
	case 3:
		//getSnakeHead(s)->x++;
		getSnakeHead(s)->x = getSnakeHead(s)->x+pixelResolution;
		break;
	default:
		*game = false;
		return;
	}
	//Serial.printf("snakeHeadPosition x: %d, y: %d\n", getSnakeHead(s)->x, getSnakeHead(s)->y);
	if(getSnakeHead(s)->x < 0 || getSnakeHead(s)->y < 0 || getSnakeHead(s)->x >= resolutionx || getSnakeHead(s)->y >= resolutiony){
		//Serial.print("Game Over snakeHeadPosition\n");
		*game = false;
	}
	return;
}
