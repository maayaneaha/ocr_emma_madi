#include "processing.h"

void binarize(SDL_Surface *img, int delta, int factor_)
{
    double factor = (259 * (delta + 255)) / (255.0 * (259.0 - delta));
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int w = img->w;
    int h = img->h;


    int value = 0;

    if (delta == 259)
    {
        delta = 258;
    }

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            r = factor * (r - 128) + 128;
            g = factor * (g - 128) + 128;
            b = factor * (b - 128) + 128;
            r = 0.3 * (double)r + 0.59 * (double)g + 0.11 * (double)b;
            value += r;
            pixel = SDL_MapRGB(img->format, r, r, r);
            put_pixel(img, i, j, pixel);
        }
    }

    value = value / (w * h);
    value = (double)value * ((double)factor_ / (double) 100);


     for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(img, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if (r <= value)
            {
                r = 0;
            }
            else
            {
                r = 255;
            }
            pixel = SDL_MapRGB(img->format, r, r, r);
            put_pixel(img, i, j, pixel);
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