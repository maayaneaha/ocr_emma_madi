#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_sdl()
{
	// Init only the video part.
	// If it fails, die with an error message.
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
	SDL_Surface *img;

        // Load an image using SDL_image with format detection.
	// If it fails, die with an error message.
	img = IMG_Load(path);
	if (!img)
		errx(3, "can't load %s: %s", path, IMG_GetError());
	
	return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
	SDL_Surface *screen;

	// Set the window to the same size as the image
	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
	if (screen == NULL)
	{
		// error management
		errx(1, "Couldn't set %dx%d video mode: %s\n",
			img->w, img->h, SDL_GetError());
	}
	// Blit onto the screen surface
	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());
		
        // Update the screen
	SDL_UpdateRect(screen, 0, 0, img->w, img->h);
	
	//return the screen for further uses
	return screen;
}

void wait_for_keypressed()
{
	SDL_Event event;

	// Wait for a key to be down.
	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYDOWN);

	// Wait for a key to be up.
	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYUP);
}

long moy(char* path)
{
	SDL_Surface* image_surface;
	init_sdl();
	image_surface = load_image(path);

	int width = image_surface->w;
	int height = image_surface->h;
	long rtotal = 0;
	long gtotal = 0;
	long btotal = 0;
	long total_pixel = width * height;
	for (int i=0 ; i<width ; i++)
	{
		for (int j=0 ; j<height ; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		rtotal += (long)r;
		gtotal += (long)g;
		btotal += (long)b;
		}
	}
	rtotal /= total_pixel;
	gtotal /= total_pixel;
	btotal /= total_pixel;
	SDL_FreeSurface(image_surface);
	return ((rtotal + gtotal + btotal)/3);
	}
	void SDL_FreeSurface(SDL_Surface *surface);

int main(int argc, char *array[])
{
	if (argc == 0)
		return EXIT_FAILURE;
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

	init_sdl();

	image_surface = load_image(array[1]);

	screen_surface = display_image(image_surface);
	wait_for_keypressed();
	//Getting width and height of image
	
	int width = image_surface->w;
	int height = image_surface->h;

	//Initialising RBG values of a pixel
	
	Uint8 r, g, b;

	Uint8 averageg;

	//Grayscale
	
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			//getting pixel at i, j
			Uint32 pixel = get_pixel(image_surface, i, j);

			//getting pixel's RGB values
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			
			//calculating the gray value
			averageg = 0.3*r + 0.59*g + 0.11*b;

			//applying our gray to each color
			r = averageg;
			g = averageg;
			b = averageg;

			//create a new pixel, and putting it in place
			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, i, j, pixel);		
		}
	}
	



	long average = moy(array[1]);
	
	//binarize
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			//getting pixel at i, j
			Uint32 pixel = get_pixel(image_surface, i, j);

			//getting pixel's RGB values
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			
			if (r <= average)
			{
				r = 0;
			}
			else
			{
				r = 255;
			}
			

			//create a new pixel, and putting it in place
			pixel = SDL_MapRGB(image_surface->format, r, r, r);
			put_pixel(image_surface, i, j, pixel);		
		}
	}
	//redraw the surfaces
	update_surface(screen_surface, image_surface);
	wait_for_keypressed();

	//free the surfaces
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);

	//printf("%lu", moy(array[1]));
	return 0;
}
