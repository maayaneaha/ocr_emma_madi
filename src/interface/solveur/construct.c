#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>
#pragma GCC diagnostic pop
#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include "pixel_operations.h"
#include "newreader.h"

// Function: Result_construct
// Description: Constructs a the png with the grid solved
// Inputs: char *grid - the solved grid
// Returns: SDL_Surface * - the solved grid

SDL_Surface *Result_construct(char *grid, char *basegrid)
{

	//char *current_path;
	//current_path=(char *)malloc(100*sizeof(char));
	//getcwd(current_path,100);
	//printf(current_path);
	SDL_Surface *result = IMG_Load("sudokuv.jpg");
	if (result == NULL)
		errx(1, "Not 2working");

	int x = 20;
	int y = 10;
	for(int i = 0; i < 81; i++)
	{
		if(grid[i] != '.' && grid[i] != '0')
		{
			SDL_Rect rect = {x, y, 0, 0};
			char *text = malloc(2 * sizeof(char));
			text[0] = grid[i];
			text[1] = '\0';
			char *path = malloc(50 * sizeof(char));
			strcpy(path, "num/num-");
			strcat(path, text);
			strcat(path, ".png");
			SDL_Surface *number = IMG_Load(path);
			free(path);
			free(text);
			if (number == NULL)
				errx(1, "not 1working");

			if(basegrid[i] == '.'){
				//chages the color of number to red
				for(int j = 0; j < number->w; j++){
					for(int k = 0; k < number->h; k++){
						Uint32 pixel = get_pixel(number, j, k);
						if(pixel == 0x00000000){
							// changes the color to RGB : 128,237,153 
							uint8_t r = pixel >> 16 & 0xFF;
							uint8_t g = pixel >> 8 & 0xFF;
							uint8_t b = pixel & 0xFF;
							r = 255;
							g = 0;
							b = 0;
							pixel = (r << 16) | g | b;
							put_pixel(number, j, k, pixel);
						}
						else{
							put_pixel(number, j, k, 0xFFFFFFFF);
						}
					}
				}
			}
			SDL_BlitSurface(number, NULL, result, &rect);
			SDL_FreeSurface(number);
		}
		x += 100;
		if(x == 920)
		{
			x = 20;
			y += 100;
		}
	}
	return result;
}

int main(int argc, char* argv[])
{
	if(argc < 3)
		return 0;
	char grid1[81];
	char grid2[81];
	reader(argv[1],grid1);
	reader(argv[2],grid2);
	SDL_Surface *image = Result_construct(grid1, grid2);
	SDL_SaveBMP(image, "resolu.bmp");
	SDL_FreeSurface(image);
}
