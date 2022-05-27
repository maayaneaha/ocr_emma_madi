#ifndef LOADER_H
#define LOADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

SDL_Surface *load_img(char *path);

#endif
