#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <err.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <gtk/gtk.h>



//Filters functions:

void otsu(SDL_Surface *image_surface);
void binarize(SDL_Surface *img, int a, int);


//Tools functions:

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface *screen, SDL_Surface *image);
void array_swap(int array[], size_t i, size_t j);
void array_select_sort(int array[], size_t len);
void process(char* path, int binarise_value, int contrast_value, int otsu_value);


Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint8 max_pixel(SDL_Surface *image_surface, int width, int height);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);


int StayOnInterval256(int n);
void rotate(double degree, char* path);


#endif
