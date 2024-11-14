#include "draw.h"
#include "pixel.h"
#include <stdlib.h>
#include <TFT_eSPI.h>
#include "stdint.h"

void draw(uint8_t* pixels, TFT_eSprite * spr) {

	spr->pushImage(0, 0, resolutiony, resolutionx, (uint16_t*) pixels, 8);
	spr->pushSprite(0, 0);
}
