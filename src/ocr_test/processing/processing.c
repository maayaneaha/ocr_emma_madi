#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "filters.h"
#include <stdlib.h>
#include <string.h>

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
             img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
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
    } while (event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

<<<<<<< HEAD:src/Processing/filters/processing.c
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void save(SDL_Surface *image_surface, char* n)
{
    printf("%s",concat("output/",n));
    SDL_SaveBMP(image_surface, concat("output/",n));
}

int main(int argc, char *array[])
{
    if (argc != 3)
	{
	printf("ERROR: NO INPUT FILE\n");
        return EXIT_FAILURE;
	}
=======
int main(int argc, char *array[])
{
    if (argc != 5)
    {
        printf("Usage: ./processing [PATH_TO_FILE] [CONTRAST_VALUE] [BINARISE_VALUE] [OTSU ENABLE 1/0]\n");
        return EXIT_FAILURE;
    }

>>>>>>> ce8a602b9d12b37d902453da05bab7ff1eecc1f0:src/ocr_test/processing/processing.c
    SDL_Surface *image_surface;
    //SDL_Surface *screen_surface;

    // initialise sdl
    init_sdl();
    // Load image into memory from path
    image_surface = load_image(array[1]);

    int contrast_value = atoi(array[2]);
    int binarise_value = atoi(array[3]);

    // Save picture size
    int width = image_surface->w;
    int height = image_surface->h;

    // Display original image
    //screen_surface = display_image(image_surface);
    // Wait for user press a key
    //wait_for_keypressed();

    // Apply filters

    if (*array[4] == 49)
    {
        contrast_1(image_surface, contrast_value);
        grayscale(image_surface, width, height);
        otsu(image_surface);
        // binarise(image_surface, width, height, 300);
        noiseReduction(image_surface, width, height);
        //invert(image_surface,width,height);
    }
    else
    {
        contrast_1(image_surface, contrast_value);
        grayscale(image_surface,width,height);
        binarise(image_surface,width,height,binarise_value);
        //invert(image_surface,width,height);
        //noiseReduction(image_surface, width, height);
    }

    // edges_detection(image_surface, 0, width,height);
    // Show processed image
    //screen_surface = display_image(image_surface);
    //wait_for_keypressed();

    SDL_SaveBMP(image_surface, "output.bmp");

    // Saving image with applied filters
    // clear and exit

<<<<<<< HEAD:src/Processing/filters/processing.c
    // END FILTERS

    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    // SAVING

    save(image_surface,array[2]);

    // END SAVING

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
=======
    //SDL_FreeSurface(image_surface);
    //SDL_FreeSurface(screen_surface);
>>>>>>> ce8a602b9d12b37d902453da05bab7ff1eecc1f0:src/ocr_test/processing/processing.c

    return 0;
}
