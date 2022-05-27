#ifndef FILTERS_H
#define FILTERS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "pixel_operations.h"
#include "loader.h"

SDL_Surface *Grayscale(SDL_Surface *img);
void insertionSort(int arr[], int n);
SDL_Surface *image_noise_reduction(SDL_Surface *img);
SDL_Surface *NoiseReduction(SDL_Surface *img);
SDL_Surface *Thresholding(SDL_Surface *img, int threshold);
int Otsu(SDL_Surface *img);
SDL_Surface *Integral(SDL_Surface *img);
SDL_Surface* preProcessing(char* path);

#endif
