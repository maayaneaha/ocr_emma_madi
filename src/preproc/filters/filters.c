// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)

#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
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

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

void grayscale(SDL_Surface* image_surface, int width, int height)
{
    Uint8 r, g, b;
	Uint8 averageg;

	//Grayscale
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			averageg = 0.3*r + 0.59*g + 0.11*b;
			
			r = averageg;
			g = averageg;
			b = averageg;

			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, i, j, pixel);
		}
	}
}

void binarise(SDL_Surface* image_surface, int width, int height)
{
    long value = 0;
    for (int i=0 ; i<width ; i++)
	{
		for (int j=0 ; j<height ; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
		    value += (long)r;
		}
	}

    value = value / (width * height);
    
    for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if (r <= value)
			{
				r = 0;
			}
			else
			{
				r = 255;
			}
			pixel = SDL_MapRGB(image_surface->format, r, r, r);
			put_pixel(image_surface, i, j, pixel);
		}
	}
}
