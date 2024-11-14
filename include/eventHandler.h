#include "snake.h"
#include <TFT_eSPI.h>
#include <Free_Fonts.h>


#ifndef event_h
#define event_h

#define Hz 5  // Frequency of event loop

#define rightKey 26

#define leftKey 33 // 25	// The buttons
#define inputDelay 200 // ms

/*
 * This is the general eventLoop, which checks if events are thrown at a given frequency
 *
 * It returns a char, which is handled as an 8 Bit unsigned integer error code.
 */
void eventLoop(void* );

/*
 * This checks if snake bites apple
 */
bool checkSnakeBite(snake* s, apple* a);

/*
 * This checks if snake touches itself
 */
bool checkSnakeTouch(snake* s);

/*
 * This checks if left button is pressed
 */
bool checkButtonLeft(int button, int* old); // ToDo better docu

/*
 * This checks if right button is pressed
 */
bool checkButtonRight(int button, int* old);


// Here are the handler functions 

void handleSnakeBite(void* v);

void handleSnakeTouch(void* v);

void handleButtonLeft(void* v);

void handleButtonRight(void* v);

void handleGameOver(TFT_eSprite* spr, int buttonStateLeft, int buttonStateRight, uint8_t* pixels, snake* s);

void printSnake(snake* s);

#endif
