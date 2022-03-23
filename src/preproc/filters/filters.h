#ifndef Filters_H_
#define FILTERS_H

#include <stdlib.h>
#include <SDL.h>

//Usefull functions : 

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

//Filters functions:

void grayscale(SDL_Surface* screen, int width, int height);
void noiseReduction(SDL_Surface *img, int width, int height);
void contrast(SDL_Surface* img, int width, int height);

void binarise(SDL_Surface* screen, int width, int height, int factor);
void otsu(SDL_Surface *image_surface);


void gamma_sdl(SDL_Surface *surface, int width, int height);

void blur(SDL_Surface *img, int width, int height);


#endif
