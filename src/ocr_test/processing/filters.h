#ifndef Filters_H_
#define FILTERS_H

#include <stdlib.h>
#include <SDL.h>

//Filters functions:

void grayscale(SDL_Surface* screen, int width, int height);
void contrast_1(SDL_Surface* img, int delta);
void invert(SDL_Surface *image_surface, int width, int height);
void edges_detection(SDL_Surface *image_surface, int threshold, int width, int height);
void noiseReduction(SDL_Surface *surface, int width, int height);
void otsu(SDL_Surface *image_surface);
void binarise(SDL_Surface *image_surface, int width, int height, int factor);


#endif
