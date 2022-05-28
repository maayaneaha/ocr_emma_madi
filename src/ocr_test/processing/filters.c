#include "SDL/SDL.h"
#include "tools.h"

void grayscale(SDL_Surface *image_surface, int width, int height)
{
    Uint8 r, g, b;
    double val;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            val = 0.3 * (double)r + 0.59 * (double)g + 0.11 * (double)b;
            // or : val = (r + g + b)/3;

            r = val;
            g = val;
            b = val;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

void invert(SDL_Surface *image_surface, int width, int height)
{
    Uint8 r, g, b;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

void contrast_1(SDL_Surface *img, int delta)
{
    double factor = (259 * (delta + 255)) / (255.0 * (259.0 - delta));
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int w = img->w;
    int h = img->h;

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
            r = StayOnInterval256(factor * (r - 128) + 128);
            g = StayOnInterval256(factor * (g - 128) + 128);
            b = StayOnInterval256(factor * (b - 128) + 128);
            pixel = SDL_MapRGB(img->format, r, g, b);
            put_pixel(img, i, j, pixel);
        }
    }
}

void edges_detection(SDL_Surface *image_surface, int threshold, int width, int height)
{
    Uint8 r_old, g_old, b_old;
    Uint8 r_left, g_left, b_left;
    Uint8 r_bottom, g_bottom, b_bottom;

    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 1; x < width; x++)
        {
            Uint32 old_pixel = get_pixel(image_surface, x, y);
            Uint32 left_pixel = get_pixel(image_surface, x - 1, y);
            Uint32 bottom_pixel = get_pixel(image_surface, x, y + 1);
            SDL_GetRGB(old_pixel, image_surface->format, &r_old, &g_old, &b_old);
            SDL_GetRGB(left_pixel, image_surface->format, &r_left, &g_left, &b_left);
            SDL_GetRGB(bottom_pixel, image_surface->format, &r_bottom, &g_bottom, &b_bottom);

            int oldValue = (r_old + g_old + b_old) / 3;
            int leftValue = (r_left + g_left + b_left) / 3;
            int bottomValue = (r_bottom + g_bottom + b_bottom) / 3;

            if (abs(oldValue - leftValue) <= threshold || abs(oldValue - bottomValue) <= threshold)
            {
                Uint32 pixel;
                pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                put_pixel(image_surface, x, y, pixel);
            }
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
                r = 255;
            }
            else
            {
                r = 0;
            }
            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}