#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

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

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
	//recupere les pixels
	int bpp = surf->format->BytesPerPixel;
	return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
	//set les pixels a l'aide de pixel_ref
	Uint8 *p = pixel_ref(surface, x, y);

	switch (surface->format->BytesPerPixel)
	{
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;
	}

	return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
	//remettre un pixel
	Uint8 *p = pixel_ref(surface, x, y);

	switch(surface->format->BytesPerPixel)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
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

	// return the screen for further uses
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

void setlines(SDL_Surface *image, int *x, int *y)
{
	Uint32 pixel = SDL_MapRGB(image->format, 255, 62, 181);
	for(int i = 0; i < 10; i++)
	{
		int xx = y[i];
		for(int j = 0; j < image->w;j++)
		{
			put_pixel(image,j,xx,pixel);
		}
	}
	for(int i = 0; i < 10; i++)
	{
		int xx = x[i];
		for(int j = 0; j < image->h;j++)
		{
			put_pixel(image,xx,j,pixel);
		}
	}
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
	if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void getlines(SDL_Surface *image, int *x, int *y)
{
	int width = image->w;
	int height = image->h;
	int *linesx = malloc(height * sizeof(int));
	int *linesy = malloc(width * sizeof(int));
	int nbpixelx = 0;
	int nbpixely = 0;

	for(int y = 0; y < height ; y++) //nombre pixel sur hori
	{
		for(int x = 0; x < width ; x++)
		{
			Uint32 pixel = get_pixel(image,x,y);
			Uint8 r,g,b;
			SDL_GetRGB(pixel, image->format, &(r), &(g), &(b));
			if((r<150)&(g<150)&(b<150))
				nbpixelx += 1;
		}
		linesx[y] = nbpixelx;
		nbpixelx = 0;
	}

	for(int x = 0; x < width; x++) //verti
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image,x,y);
			Uint8 r,g,b;
			SDL_GetRGB(pixel, image->format, &(r), &(g), &(b));
			if((r<150)&(g<150)&(b<150))
				nbpixely += 1;
		}
		linesy[x] = nbpixely;
		nbpixely = 0;
	}

	for(int i = 0; i < 10;i++)
	{
		int ind = 0;
		int max = 0;
		for(int x = 0; x < height; x++)
		{
			if(linesx[x] > max)
			{
				max = linesx[x];
				ind = x;
			}
		}
		for(int j = ind - 15; j < ind + 15; j++)
		{
			if(j > -1 && j < height)
				linesx[j] = 0;
		}
		y[i] = ind;
	}


	for(int i = 0; i < 10;i++)
	{
		int ind = 0;
		int max = 0;
		for(int y = 0; y < width; y++)
		{
			if(linesy[y] > max)
			{
				max = linesy[y];
				ind = y;
			}
		}
		for(int j = ind - 15; j < ind + 15; j++)
		{
			if(j > - 1 && j < width)
				linesy[j] = 0;
		}
		x[i] = ind;
	}

	free(linesx);
	free(linesy);
}

int compare_function(const void *a,const void *b) 
{
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

void segmentation(SDL_Surface *image)
{
	int *x = malloc(10 * sizeof(int));
	int *y = malloc(10 * sizeof(int));

	getlines(image,x,y);

	qsort(x,10,sizeof(int),compare_function);
	qsort(y,10,sizeof(int),compare_function);
	display_image(image);
	setlines(image,x,y);
	display_image(image);
	wait_for_keypressed();

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			SDL_Rect rectangle;
			rectangle.x = x[j];
			rectangle.y = y[i];
			rectangle.w = x[j+1] - x[j];
			rectangle.h = y[i+1] - y[i];

			SDL_Surface *croped = SDL_CreateRGBSurface(SDL_HWSURFACE, rectangle.w, rectangle.h, 32, 0, 0, 0, 0);
			SDL_BlitSurface(image,&rectangle,croped,NULL);

			char buffer[100];
			snprintf(buffer, sizeof(buffer), "./%d.bmp", i*10 + j);

			if(SDL_SaveBMP(croped, buffer) != 0)
			{
				printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
			}

			SDL_FreeSurface(croped);
		}
	}
	free(x);
	free(y);
	SDL_FreeSurface(image);
}


int main(int argc, char **argv)
{
	if (argc != 2){
		errx(1,"the segmentation function takes 2 parameters");
		return EXIT_FAILURE;
	}
	SDL_Surface* image_surface;
	image_surface = load_image(argv[1]);
	segmentation(image_surface);

	return EXIT_SUCCESS;
}
