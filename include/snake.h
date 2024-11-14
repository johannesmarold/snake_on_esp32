#ifndef snake_h
#define snake_h

#define BLACK 0x00
#define WHITE 0xFF

#define pixelResolution 8

#include "pixel.h"

typedef struct pos {
	int x,y;
	uint8_t color;
	pos* next;
} pos;

typedef struct snake {
	int orientation; // 0 up, 1 left, 2 down, 3 right
	pos* body; // Linked list of positions of pixels starting from the END of the snake.
} snake;

typedef struct apple {
	pos* body;
} apple;

/*
 * Make snake bigger
 */
void snakeGrow(snake* s);

/*
 * Move snake one step
 */
void snakeMove(snake* s, uint8_t* pixels, bool* game);

/*
 * Get pointer to the head of snake
 */
pos* getSnakeHead(snake* s);

/*
 * Fill the pixels-array (pixel.h) with the body of the snake
 */
void snakeDraw(snake* s, uint8_t* pixels);

void appleDraw(apple* a, uint8_t* pixels);

snake* initSnake();

void freeSnake(snake* s);

void moveApple(snake * s, apple * a, uint8_t* pixels);

#endif
