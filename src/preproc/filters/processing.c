#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "filters.h"

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

void save(SDL_Surface *image_surface)
{
    SDL_SaveBMP(image_surface, "output.bmp");
}

int main(int argc, char *array[])
{
    if (argc == 0)
        return EXIT_FAILURE;

    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    init_sdl();
    image_surface = load_image(array[1]);

    int width = image_surface->w;
    int height = image_surface->h;

    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    gamma_sdl(image_surface,width,height);
    grayscale(image_surface, width, height);
    contrast(image_surface, width, height);
    //otsu(image_surface);
    //noiseReduction(image_surface,width,height); //RESULTS ARE NOT AS EXPECTED
    //blur(image_surface,width,height);



    binarise(image_surface, width, height, 220);
    //noiseReduction(image_surface,width,height); //RESULTS ARE NOT AS EXPECTED
    //otsu(image_surface);
    //noiseReduction(image_surface,width,height); //RESULTS ARE NOT AS EXPECTED





    // END FILTERS

    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    // SAVING

    save(image_surface);

    // END SAVING

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
