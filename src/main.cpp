
#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
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
#include <Free_Fonts.h>
#include "iot_button.h"
#include "snake.h"
#include "eventHandler.h"
#include "draw.h"
#include "pixel.h"
#include "esp_random.h"
#include "parameter.h"
#include "stdint.h"


#define BLACK 0x00
#define WHITE 0xFF

int start,end,frequ=1000/2;
snake* s;
apple* a;
uint8_t pixels[resolutionx*resolutiony];  // NOW statical allocation !! 


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite object "spr" with pointer to "tft" object
TFT_eSprite spr_Score = TFT_eSprite(&tft);  // Sprite object "spr" with pointer to "tft" object


bool game;
int buttonStateLeft;
int buttonStateRight;
param* p;

int highscore = 0;
int score = 0;
char scoreString[50];

void setup(){
	// initialize screen and sprites
	Serial.begin(9600);
	tft.init();
	tft.setRotation(0);
	spr.setColorDepth(8);
	spr.createSprite(resolutiony, resolutionx);
	spr.fillScreen(TFT_WHITE);
	spr.pushSprite(0, 0);
	
	spr.setTextColor(TFT_BLACK, TFT_WHITE);
	spr.setFreeFont(&FreeSansBoldOblique18pt7b);
	spr.drawString("SNAKE",50,120,GFXFF);
	spr.pushSprite(0, 0);

	pinMode(leftKey, INPUT);
	pinMode(rightKey, INPUT);
	// Wait for user input to start the GAME LOOP
	while(1){
		buttonStateLeft = digitalRead(leftKey);
		buttonStateRight = digitalRead(rightKey);
		if(buttonStateLeft != digitalRead(leftKey) || buttonStateRight != digitalRead(rightKey)){
	    	break;
	    }
	}
	
	spr.fillScreen(TFT_WHITE);
	spr.pushSprite(0, 0);

	// Score Board
	spr_Score.setColorDepth(8);
	spr_Score.createSprite(240, 18);
	spr_Score.fillScreen(TFT_WHITE);
	spr_Score.pushSprite(0, 301);
	spr_Score.setTextColor(TFT_BLACK, TFT_WHITE);
	spr_Score.setFreeFont(FMB12);	
	snprintf(scoreString, 50, "Score: %d", score);
	spr_Score.drawString(scoreString, 0, 0,GFXFF);
	spr_Score.pushSprite(0, 301);
		
	s = initSnake();
	a = (apple*) malloc(sizeof(apple));
	a->body =(pos*) malloc(sizeof(pos));
	a->body->next = NULL;
	a->body->x = pixelResolution * 5;
	a->body->y = pixelResolution * 6;
	clearPixel(pixels); 
	game = true;
	p = (param *) malloc(sizeof(param));
	p->a = a;
	p->s = s;
	p->pixels = pixels;
	p->game = &game;
	buttonStateLeft = digitalRead(leftKey);
	buttonStateRight = digitalRead(rightKey);
}

void loop() {
	
	uint64_t uptime_ms = esp_timer_get_time() / 1000;

	// update successful food catch
	if(checkSnakeBite(p->s, p->a)) {
		snakeGrow(p->s);
		moveApple(p->s, p->a, p->pixels);
		score = score + 10;
		// Score Board update
		snprintf(scoreString, 50, "Score: %d", score);
		spr_Score.drawString(scoreString,0,0,GFXFF);
		spr_Score.pushSprite(0, 301);
	}

	// check if snake hit itself
	if(checkSnakeTouch(p->s)) {
		*(p->game) = false;
	}

	// check Button Input for left and right
	if(checkButtonLeft(leftKey, &buttonStateLeft)) {
		p->s->orientation--;
		if(p->s->orientation < 0) {
			p->s->orientation = 3;
		}
	}
	if(checkButtonRight(rightKey, &buttonStateRight)) {
		p->s->orientation++;
		if(p->s->orientation > 3) {
			p->s->orientation = 0;
		}
	}

	// GAME LOOP
	start = millis();
	clearPixel(pixels);

	snakeMove(s,pixels,p->game);
	
	snakeDraw(s,pixels);
	
	appleDraw(a,pixels);
	
	draw(pixels,&spr);
	
	if(!(*p->game)) {
		Serial.print("GAME OVER\nGAME OVER\nGAME OVER\nGAME OVER\nGAME OVER\nGAME OVER\n");
		//GAME OVER !
		*p->game = true;
		if (score > highscore) {
			highscore = score;
		}
		
		spr_Score.fillScreen(TFT_WHITE);
		spr_Score.pushSprite(0, 301);
		snprintf(scoreString, 50, "HighScore: %d", highscore);
		spr_Score.drawString(scoreString,0,0,GFXFF);
		spr_Score.pushSprite(0, 301);

		handleGameOver(&spr, buttonStateLeft, buttonStateRight, pixels, s);

		spr_Score.fillScreen(TFT_WHITE);
		spr_Score.pushSprite(0, 301);
		score = 0;
		snprintf(scoreString, 50, "Score: %d", score);
		spr_Score.drawString(scoreString, 0, 0,GFXFF);
		spr_Score.pushSprite(0, 301);
	}

	uint64_t uptime_ms_end = esp_timer_get_time() / 1000;
}
