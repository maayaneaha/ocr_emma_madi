#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>

int main(int argc, char *array[])
{
	if (argc > 2){
		double angle = atoi(array[2]);
		SDL_Surface *picture = NULL, *rot = NULL; //Surface de l'image
		picture =IMG_Load(array[1]); //load the picture
		rot = rotozoomSurface(picture, angle, 1, 1);
		//picture's transformation 
		SDL_SaveBMP(rot, "rotofpicture");
		SDL_FreeSurface(rot);									                SDL_FreeSurface(picture);
		SDL_Quit();
		return EXIT_SUCCESS;
	}	
	return EXIT_FAILURE;
}
