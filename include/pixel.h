#include "stdint.h"

#ifndef pixel_h
#define pixel_h

#define resolutionx 300         //orientasion of x is long side -->320 pixels
#define resolutiony 240          //therefore y is short side -->240 pixels

#define BLACK 0x00
#define WHITE 0xFF

/*
 * This function allocates memory for the pixel buffer and fills every pixel with the color white
*/
uint8_t* initializePixels();

/*
 * This function calculates the position of a pixel from the given index.
 * It returns an int array of size 2 (x and y coordinate)
 *
 * Issue: Memory has to be freed after retrieving the x and y coordinate
 */
int* getPos(int index);


/*
 * This function gets the x and y coordinate and returns the associated index of our pixels array
 *
 * It does NOT check if the returned index is out of bounds of the pixels array!
 */
int getIndex(int x, int y);


/*
 * Clear the pixel buffer
*/
void clearPixel(uint8_t* x);

#endif
