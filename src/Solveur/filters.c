#include "filters.h"

SDL_Surface *Grayscale(SDL_Surface *img)
{
   int height = img->h;
   int width = img->w;
   SDL_Surface *out = SDL_CreateRGBSurface(0, width, height, 32,0,0,0,0);
   Uint8 r, g, b;
   for (int i = 0; i < height; i++)
   {
       for (int j = 0; j < width; j++)
       {
           Uint32 pixel = get_pixel(img, j, i);
           SDL_GetRGB(pixel, img->format, &r, &g, &b);
           Uint8 average = (r + b + g)/3;
           pixel = SDL_MapRGB(img->format, average, average, average);
           put_pixel(out, j, i, pixel);
       }
   }
   return out;
}

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Block-matching noise reduction
SDL_Surface *image_noise_reduction(SDL_Surface *img)
{
    int delta[8][2] =
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    SDL_Surface *output = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);

    for (int h = 0; h < img->h; h++)
    {
        for (int w = 0; w < img->w; w++)
        {
            int sum = 0;
            for (int dir = 0; dir < 8; dir++)
            {
                int nh = h + delta[dir][0];
                int nw = w + delta[dir][1];
                if (nh >= 0 && nh < img->h && nw >= 0 && nw < img->w)
                {
                    Uint8 r, g, b;
                    Uint32 pixel = get_pixel(img, w, h);
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    sum += (r == 255 && g == 255 && b == 255);
                }
            }

            Uint32 white_pixel = SDL_MapRGB(img->format, 255, 255, 255);
            Uint32 black_pixel = SDL_MapRGB(img->format, 0, 0, 0);
            if (sum > 4)
                put_pixel(output, w, h, white_pixel);
            else
                put_pixel(output, w, h, black_pixel);
        }
    }

    return output;
}

SDL_Surface *Thresholding(SDL_Surface *img, int threshold)
{
    int height = img->h;
    int width = img->w;
    SDL_Surface *out = SDL_CreateRGBSurface(0, width, height, 32,0,0,0,0);
    Uint8 r, g, b;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            if (r < threshold)
                r = 0;
            else
                r = 255;

            pixel = SDL_MapRGB(img->format, r, r, r);
            put_pixel(out, j, i, pixel);
        }
    }
    return out;
}

int Otsu(SDL_Surface *img)
{
    int threshold;
    Uint8 r, g, b;
    int total_pixels = 0;
    int *histo = calloc(256, sizeof(int));
    
    int height = img->h;
    int width = img->w;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            histo[r]++;
            total_pixels++;
        }
    }
    
    double sum = 0;
    for(int t = 0; t<256; t++)
        sum += t * histo[t];

    double sumB = 0;
    double wB = 0;
    double wF = 0;

    double varMax = 0;
    threshold = 0;

    for(int t = 0; t < 256; t++)
    {
        wB += histo[t];
        if (wB == 0)  
            continue;
        wF = total_pixels - wB;

        sumB +=  t * histo[t];

        double mB = sumB / wB; //Mean Background
        double mF = (sum - sumB) / wF; //Mean Foreground
        
        //Calculate Between Class Variance
        double varBetween = wB * wF * ((mB - mF) * (mB - mF));

        //Check if new maximum found
        if (varBetween > varMax) 
        {
            varMax = varBetween;
            threshold = t;
        }
    }

    free(histo);

    return threshold;
}

SDL_Surface *Integral(SDL_Surface *img)
{
    int height = img->h;
    int width = img->w;
    int s = width/8;
    int s2 = s/2;
    double t = 0.15;
    SDL_Surface *out =  SDL_CreateRGBSurface(0, width, height,32,0,0,0,0);
    Uint32 pixel;
    Uint8 r, g, b;
    unsigned long **intImg;
    intImg = calloc(width, sizeof *intImg);
    for (int i=0; i < width; i++)
        intImg[i] = calloc(height, sizeof *(intImg[i]));
    for(int i = 0; i < width; i++)
    {
        long sum = 0;
        for(int j = 0; j < height; j++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            sum += r;
            if (i == 0)
                intImg[i][j] = sum;
            else
                intImg[i][j] = intImg[i-1][j] + sum;
        }
    }
    
    for(int i = 0; i < width; i++)
    {
        int x1 = i - s2;
        int x2 = i + s2;

        if (x1 < 0)
            x1 = 0;
        if (x2 >= width)
            x2 = width-1;

        for(int j = 0; j < height; j++)
        {
            int y1 = j - s2;
            int y2 = j + s2;

            if (y1 < 0)
                y1 = 0;
            if (y2 >= height)
                y2 = height-1;

            int count = (x2 - x1) * (y2 - y1);
            long sum = intImg[x2][y2] - intImg[x1][y2] - intImg[x2][y1] + intImg[x1][y1];
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r,&g, &b);
            if((long)(r*count) < (long)(sum *(1.0-t)))
            {
                pixel = SDL_MapRGB(img->format, 0, 0, 0);
                put_pixel(out,i,j,pixel);
            }
            else
            {
                pixel = SDL_MapRGB(img->format, 255, 255, 255);
                put_pixel(out,i,j,pixel);
            }
        }
    }

    free(intImg);
    
    return out;
}

SDL_Surface* preProcessing(char* path)
{
    SDL_Surface *img = load_img(path);
    img = Grayscale(img);
    SDL_SaveBMP(img, "tmp/1-Grayscaled.bmp");
    img = Integral(img);
    SDL_SaveBMP(img, "tmp/2-BinarizedByIntegral.bmp");
    img = image_noise_reduction(img);
    SDL_SaveBMP(img, "tmp/3-NoiseReduction.bmp");
    return img;
}

/*int main(int argc, char * argv[])
{
    if(argc != 2)
        errx(1,"Error: one argument is required.");
    
    char* arg = argv[1];

    SDL_Surface *img = load_img(arg);
    img = Grayscale(img);
    SDL_SaveBMP(img, "tmp/grayscaled.bmp");
    SDL_Surface *thresholded = Thresholding(img, Otsu(img));
    SDL_SaveBMP(thresholded, "tmp/thresholded.bmp");
    SDL_Surface *integral = Integral(img);
    SDL_SaveBMP(integral, "tmp/integralImage.bmp");

    SDL_FreeSurface(img);
    SDL_FreeSurface(thresholded);
    SDL_FreeSurface(integral);

    return EXIT_SUCCESS;
}*/
