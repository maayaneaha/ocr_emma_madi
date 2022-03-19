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
void binarise(SDL_Surface* screen, int width, int height);


#endif
