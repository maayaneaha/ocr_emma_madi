// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)

#include <math.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"

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

/*
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

void contrast2(SDL_Surface *image_surface,int width, int height)
{
    Uint8 r, g, b;
    long average_red = 0;
    long average_green = 0;
    long average_blue = 0;

    // Gamma
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            average_red += (long)r;
            average_green += (long)g;
            average_blue += (long)b;
        }
    }

    average_red = average_red / (width * height);
    average_blue = average_blue / (width * height);
    average_green = average_green / (width * height);

    double factor_red = (259 * (average_red + 255)) / (255 * (259 - average_red));
    double factor_blue = (259 * (average_blue + 255)) / (255 * (259 - average_blue));
    double factor_green = (259 * (average_green + 255)) / (255 * (259 - average_green));

    //double factor = (factor_blue + factor_green + factor_red) / 3;
    //double average = (average_blue + average_green + average_red) / 3;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r = factor_red * (r - average_red) + average_red;
            g = factor_green * (g - average_green) + average_green;
            b = factor_blue * (b - average_blue) + average_blue;

            //r = factor * (r - average) + average;
            //g = factor * (g - average) + average;
            //b = factor * (b - average) + average;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}
*/

void gamma_sdl(SDL_Surface *image_surface, int width, int height)
{
    Uint8 r, g, b;
    double average_red = 0;
    double average_green = 0;
    double average_blue = 0;

    // Gamma
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            average_red += r;
            average_green += g;
            average_blue += b;
        }
    }

    average_red = (double) average_red / (double)(width * height);
    average_blue = (double)average_blue / (double)(width * height);
    average_green = (double)average_green / (double)(width * height);

    //double average = (average_blue + average_green + average_red)/3;

    double gamma_red = (double)2 / (double)(255 - (double)average_red);
    double gamma_green = (double)2 / (double)(255 - (double)average_green);
    double gamma_blue = (double)2 / (double)(255 - (double)average_blue);

    //double gamma = (gamma_blue + gamma_green + gamma_red)/3;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r = pow((double)r / (double)255, (double)gamma_red) * (double)255;
            g = pow((double)g / (double)255, (double)gamma_green) * (double)255;
            b = pow((double)b / (double)255, (double)gamma_blue) * (double)255;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

void noiseReduction(SDL_Surface *surface, int width, int height)
{
    int table[5];

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
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k);
                        table[2] = get_pixel(surface, i, k);
                        table[3] = get_pixel(surface, i, k + 1);
                        table[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                    if (k == height)
                    {
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k - 1);
                        table[2] = get_pixel(surface, i, k);
                        table[3] = get_pixel(surface, i, k);
                        table[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                    else
                    {
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k - 1);
                        table[2] = get_pixel(surface, i, k);
                        table[3] = get_pixel(surface, i, k + 1);
                        table[4] = get_pixel(surface, i + 1, k);
                        break;
                    }
                }
                if (i == width)
                {
                    if (k == 0)
                    {
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k);
                        table[2] = get_pixel(surface, i - 1, k);
                        table[3] = get_pixel(surface, i, k + 1);
                        table[4] = get_pixel(surface, i, k);
                        break;
                    }
                    if (k == height)
                    {
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k - 1);
                        table[2] = get_pixel(surface, i - 1, k);
                        table[3] = get_pixel(surface, i, k);
                        table[4] = get_pixel(surface, i, k);
                        break;
                    }
                    else
                    {
                        table[0] = get_pixel(surface, i, k);
                        table[1] = get_pixel(surface, i, k - 1);
                        table[2] = get_pixel(surface, i - 1, k);
                        table[3] = get_pixel(surface, i, k + 1);
                        table[4] = get_pixel(surface, i, k);
                        break;
                    }
                }
                if (k == 0)
                {
                    table[0] = get_pixel(surface, i, k);
                    table[1] = get_pixel(surface, i, k);
                    table[2] = get_pixel(surface, i - 1, k);
                    table[3] = get_pixel(surface, i, k + 1);
                    table[4] = get_pixel(surface, i + 1, k);
                    break;
                }
                if (k == height)
                {
                    table[0] = get_pixel(surface, i, k);
                    table[1] = get_pixel(surface, i, k - 1);
                    table[2] = get_pixel(surface, i - 1, k);
                    table[3] = get_pixel(surface, i, k);
                    table[4] = get_pixel(surface, i + 1, k);
                    break;
                }
                else
                {
                    table[0] = get_pixel(surface, i, k);
                    table[1] = get_pixel(surface, i, k - 1);
                    table[2] = get_pixel(surface, i - 1, k);
                    table[3] = get_pixel(surface, i, k + 1);
                    table[4] = get_pixel(surface, i + 1, k);
                    break;
                }
            }
            array_select_sort(table, 5);
            put_pixel(surface, i, j, table[2]);
        }
    }
}

void blur(SDL_Surface *img, int w, int h)
{
  int pixelTable[5];
  int x;

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
        x = 0;
        for (int li = i-1; li<=i+1; li+=2)
        {
            if (li >= 0 && li < h )
                pixelTable[x] = get_pixel(img, li, j);
            else
                pixelTable[x] = get_pixel(img, i, j);
            x += 1;

        }
        for(int col = j-1; col<= j+1; col+=2)
        {
            if (col >= 0 && col < w)
                pixelTable[x] = get_pixel(img, i, col);
            else
                pixelTable[x] = get_pixel(img, i, j);
            x += 1;
        }
      pixelTable[x] = get_pixel(img, i, j);   
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      put_pixel(img, i, j, med);
   }
 }
}

void contrast(SDL_Surface *image_surface, int width, int height)
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
    int factor = (259*(value + 255))/(255*(259- value));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r = factor * (r - value) + value;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
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
    value = (double)value * ((double)factor / (double) 100);
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

void init_hist(SDL_Surface *image_surface, int width, int height, float *hist)
{
    Uint8 r, g, b;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            hist[r]++;
        }
    }
}

void normalized(float *hist, int Pixels)
{
    for (int i = 0; i < 256; i++)
        hist[i] = hist[i] / Pixels;
}

Uint8 otsu_threshold(float *hist)
{
    float w0 = 0, w1 = 0, wT = 0;
    float u0 = 0, u1 = 0, uT = 0;
    float sum = 0, vk = 0, v_max = 0;
    float threshold = 0;

    for (size_t i = 0; i < 256; i++)
    {
        uT += i * hist[i];
        wT += hist[i];
    }

    for (size_t i = 0; i < 256; i++)
    {
        w0 += hist[i];
        w1 = wT - w0;

        sum += i * hist[i];
        u0 = sum / w0;
        u1 = (uT - sum) / w1;

        // Maximizing inter-class variance
        vk = w0 * w1 * (u0 - u1) * (u0 - u1);

        // Find max vk = Find threshold
        if (vk > v_max)
        {
            threshold = i;
            v_max = vk;
        }
    }

    return (Uint8)threshold;
}

void otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist);
    normalized(hist, width * height);

    Uint8 threshold = otsu_threshold(hist);
    Uint8 r, g, b;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if (r > threshold)
                r = 255;
            else
                r = 0;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}


// OTSU END
// FILTERS END