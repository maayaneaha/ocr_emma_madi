// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)

#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
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

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
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

void update_surface(SDL_Surface *screen, SDL_Surface *image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

Uint8 max_pixel(SDL_Surface *image_surface, int width, int height)
{
    // This function return the value of the brightest pixel in a surface.
    Uint8 r, g, b;
    Uint8 val = 0;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (val < r)
            {
                val = r;
            }
        }
    }
    return val;
}

void array_swap(int array[], size_t i, size_t j)
{
    int element1 = array[i];
    int element2 = array[j];
    array[i] = element2;
    array[j] = element1;
}

void array_select_sort(int array[], size_t len)
{
    size_t i = 0;
    size_t j;
    int min_index;
    while(i<len)
    {
        /* Find Min Index */
        j= i;
        min_index = j;
        while(j<len)
        {
            if(array[j]<array[min_index])
            {
                min_index = j;
            }
            j+=1;
        }
        array_swap(array,i,min_index);
        i++;
    }
}

int StayOnInterval256(int n)
{
    if (n < 0)
        return 0;
    if (n > 255)
        return 255;
    return n;
}

// FILTERS

void grayscale(SDL_Surface *image_surface, int width, int height)
{
    Uint8 r, g, b;
    Uint8 averageg;

    // Grayscale
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            averageg = 0.3 * r + 0.59 * g + 0.11 * b;
            // averageg = (r + g + b)/3;

            r = averageg;
            g = averageg;
            b = averageg;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

void noiseReduction(SDL_Surface *surface, int width, int height)
{
    int pixelTable[5];

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {

            for (int k = j; k <= j + 4; k++)
            {
                if (i == 0)
                {
                    if (k == 0)
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k);
                        pixelTable[2] = get_pixel(surface, i, k);
                        pixelTable[3] = get_pixel(surface, i, k + 1);
                        pixelTable[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                    if (k == height)
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k - 1);
                        pixelTable[2] = get_pixel(surface, i, k);
                        pixelTable[3] = get_pixel(surface, i, k);
                        pixelTable[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                    else
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k - 1);
                        pixelTable[2] = get_pixel(surface, i, k);
                        pixelTable[3] = get_pixel(surface, i, k + 1);
                        pixelTable[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                }
                if (i == width)
                {
                    if (k == 0)
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k);
                        pixelTable[2] = get_pixel(surface, i - 1, k);
                        pixelTable[3] = get_pixel(surface, i, k + 1);
                        pixelTable[4] = get_pixel(surface, i, k);
                        break;
                    }
                    if (k == height)
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k - 1);
                        pixelTable[2] = get_pixel(surface, i - 1, k);
                        pixelTable[3] = get_pixel(surface, i, k);
                        pixelTable[4] = get_pixel(surface, i, k);
                        break;
                    }
                    else
                    {
                        pixelTable[0] = get_pixel(surface, i, k);
                        pixelTable[1] = get_pixel(surface, i, k - 1);
                        pixelTable[2] = get_pixel(surface, i - 1, k);
                        pixelTable[3] = get_pixel(surface, i, k + 1);
                        pixelTable[4] = get_pixel(surface, i, k);
                        break;
                    }
                }
                if (k == 0)
                {
                    pixelTable[0] = get_pixel(surface, i, k);
                    pixelTable[1] = get_pixel(surface, i, k);
                    pixelTable[2] = get_pixel(surface, i - 1, k);
                    pixelTable[3] = get_pixel(surface, i, k + 1);
                    pixelTable[4] = get_pixel(surface, i + 1, k);
                    break;
                }
                if (k == height)
                {
                    pixelTable[0] = get_pixel(surface, i, k);
                    pixelTable[1] = get_pixel(surface, i, k - 1);
                    pixelTable[2] = get_pixel(surface, i - 1, k);
                    pixelTable[3] = get_pixel(surface, i, k);
                    pixelTable[4] = get_pixel(surface, i + 1, k);
                    break;
                }
                else
                {
                    pixelTable[0] = get_pixel(surface, i, k);
                    pixelTable[1] = get_pixel(surface, i, k - 1);
                    pixelTable[2] = get_pixel(surface, i - 1, k);
                    pixelTable[3] = get_pixel(surface, i, k + 1);
                    pixelTable[4] = get_pixel(surface, i + 1, k);
                    break;
                }
            }
            array_select_sort(pixelTable, 5);
            put_pixel(surface, i, j, pixelTable[2]);
        }
    }
}

void contrast(SDL_Surface* img, int delta)
{
    double factor = (259 * (delta + 255)) / (255.0 * (259.0 - delta));
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int w = img -> w;
    int h = img -> h;

    if (delta == 259)
    {
        delta = 258;
    }

    for (int i = 0; i < w;i++)
    {
        for(int j = 0; j< h; j++)
        {
            pixel = get_pixel(img, i,j);
            SDL_GetRGB(pixel, img->format, &r,&g,&b);
            r = StayOnInterval256(factor * (r - 128) + 128);
            g = StayOnInterval256(factor * (g- 128) + 128);
            b = StayOnInterval256(factor * (b - 128) + 128);
            pixel = SDL_MapRGB(img -> format,r,g,b);
            put_pixel(img,i,j,pixel);
        }
    }
}

void binarise(SDL_Surface *image_surface, int width, int height, int factor)
{
    long value = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            value += (long)r;
        }
    }

    value = value / (width * height);
    value = (double)value * ((double)factor/100);
    if (value > 255)
    {
        value = 255;
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
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

// FILTERS END