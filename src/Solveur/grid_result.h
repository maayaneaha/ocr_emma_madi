#ifndef GRID_RESULT_H
#define GRID_RESULT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "filters.h"
#include "pixel_operations.h"
#include "loader.h"

SDL_Surface *to_img(char* filepath);
void insert_case_img(SDL_Surface* result, char number, int x, int y, char letter);
int main(void);

#endif
