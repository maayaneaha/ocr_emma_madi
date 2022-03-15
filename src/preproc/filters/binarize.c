#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void init_SDL()
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
	} while(event.type != SDL_KEYUP);

	// Wait for a key to be up.
	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYUP);
}

void SDLFreeSurface(SDL_Surface *surface);

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
	

	//int width = image_surface->w;
	//int height = image_surface->h;

	//Initialising RBG values of a pixel
	

	Uint32 pxl;
	Uint8 r, g, b;
	int W = image_surface->w, H = image_surface->h;
	for(int i = 0; i < W; i++)
	{
		for(int j = 0; j < H; j++)
		{
			pxl = get_pixel(image_surface, i, j);
	           	SDL_GetRGB(pxl, image_surface->format, &r, &g, &b);
		        r = r >= 10 ? 255 : 0;
			g = g >= 10 ? 255 : 0;
		        b = b >= 10 ? 255 : 0;
			pxl = SDL_MapRGB(image_surface->format, r, g, b);
		        put_pixel(image_surface, i, j, pxl);
		}
	}

	//redraw the surfaces
	update_surface(screen_surface, image_surface);
	wait_for_keypressed();

	//free the surfaces
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);

	return 0;
}
