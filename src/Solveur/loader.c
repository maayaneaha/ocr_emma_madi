#include "loader.h"

// Load an image
SDL_Surface *load_img(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
    {
       errx(3, "can't load %s: %s", path, IMG_GetError()); 
    }

    return img;
}
