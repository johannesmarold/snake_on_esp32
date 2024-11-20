#include "snake.h"
#include "Arduino.h"
#include "pixel.h"
#include "esp_random.h"
#include <stdlib.h>
#include "stdint.h"
#include "TFT_eSPI.h"

void snakeDraw(snake* s, uint8_t* pixels) {
	pos* p = s->body;
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
	pos* p = s->body;
	while(p->next) {
		p = p->next;
	}
	return p;
}

snake* initSnake(){
	snake* s = (snake*) malloc(sizeof(snake));
	s->body = (pos*)malloc(sizeof(pos));
	s->body->x = (resolutionx/2) - (resolutionx/2)%pixelResolution; // start position always in the middle
	s->body->y = (resolutiony/2) - (resolutiony/2)%pixelResolution;
	s->body->next = NULL;
	s->body->color= BLACK;
	s->orientation = 2; // 0: runs left; 1: runs up; 2: runs right; 3: runs down

	return s;
}

void freeSnake(snake* s) {
	pos* p = s->body;
	pos* p2 = s->body;
	while(p->next) {
		p2 = p->next;
		free(p);
		p = p2;
	}
	free(p); // Note: The last element does not get free'd without this line !
	free(s); // Finally free snake
}

void snakeGrow(snake* s) {
	pos* body = s->body;
	pos* newElem = (pos *) malloc(sizeof(pos));
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

	while(pixels[getIndex(x,y)] == BLACK) // This function assumes snake is black !
	{
		x = esp_random()%((resolutionx-1)/pixelResolution);
		y = esp_random()%((resolutiony-1)/pixelResolution);

		x = (x*pixelResolution);
		y = (y*pixelResolution);
	}
	a->body->x = x;
	a->body->y = y;

	appleDraw(a, pixels);
}

void snakeMove(snake* s, uint8_t* pixels, bool* game) {
	pos* body = s->body;
	if(body->next != NULL){
	while(body->next) {
		body->x = body->next->x;
		body->y = body->next->y;
		body = body->next;
		
	}}
	// Note: We purposefully skip the head of the snake, because only the movement of the head
	// is determined by the orientation
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
	if(getSnakeHead(s)->x < 0 || getSnakeHead(s)->y < 0 || getSnakeHead(s)->x >= resolutionx || getSnakeHead(s)->y >= resolutiony){
		*game = false;
	}
	return;
}
