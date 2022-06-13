#include "processing.h"

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

void SDL_FreeSurface(SDL_Surface *surface);

void process(char* path, int binarise_value, int contrast_value, int otsu_value)
{
    SDL_Surface *image_surface;
    //SDL_Surface *screen_surface;

    // initialise sdl
    init_sdl();
    // Load image into memory from path
    image_surface = load_image(path);

    // Save picture size


    // Display original image
    //screen_surface = display_image(image_surface);
    // Wait for user press a key
    //wait_for_keypressed();

    // Apply filters

    if (otsu_value == 1)
    {
        //grayscale(image_surface, width, height);
        otsu(image_surface);
        // binarise(image_surface, width, height, 300);
        //noiseReduction(image_surface, width, height);
        //invert(image_surface,width,height);

    }
    else
    {
        binarize(image_surface, contrast_value, binarise_value);
        //grayscale(image_surface,width,height);
        //to_binarise(image_surface,width,height,binarise_value);
        //invert(image_surface,width,height);
        //noiseReduction(image_surface, width, height);
    }

    // edges_detection(image_surface, 0, width,height);
    // Show processed image
    //screen_surface = display_image(image_surface);
    //wait_for_keypressed();

    SDL_SaveBMP(image_surface, "output_1.bmp");

    // Saving image with applied filters
    // clear and exit

    //SDL_FreeSurface(image_surface);
    //SDL_FreeSurface(screen_surface);
}

void rotate(double degree, char* path){
    SDL_Surface *image_surface;
    //SDL_Surface *screen_surface;

    // initialise sdl
    init_sdl();
    // Load image into memory from path
    image_surface = load_image(path);
    SDL_Surface *new_surface = rotozoomSurface(image_surface,degree,1,1);
    SDL_SaveBMP(new_surface, "output.bmp");
}
