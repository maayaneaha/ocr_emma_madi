#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/neural_network.h"
//#include "../ImageProcessing/color.h" //CHANGE
#include "include/mnist.h"
void binarize2(SDL_Surface * image, uint8_t * array)
{
    const int WHITE = 0;
    const int BLACK = 255;
    Uint8 red = 0; // if red > 0, the pixel is white else black
    Uint8 blue = 0;
    Uint8 green = 0;
    Uint32 * pixels = (Uint32 *)image->pixels;
    for (Uint32 h = 3; h < (Uint32)(image->h) - 3; h++)
    {
        for(Uint32 w= 3; w < (Uint32)(image->w) - 3; w++)
        {
            red = pixels[h * image->h + w] >> 16 & 0xff;
	        green = pixels[h*image->h +w] >> 8 & 0xff;
	        blue = pixels[h*image->h+w] & 0xff;
            if(red > 200 && green > 200 && blue > 200)
                array[h * image->h + w] = WHITE;
            else
                array[h * image->h + w] = BLACK;
        }
    }for (int h = 0; h < image->h; h++)                                          
    {                                                                           
        for(int w = 0; w < image->w; w++)                                       
        {                                                                       
            printf("%i",array[h*image->h+w]);                                   
        }                                                                       
        printf("\n");                                                           
    }
}
void convert()
{
    neural_network_t network;
    //neural_network_random_weights(&network);
    //save(&network);
    float *activation = malloc(10*sizeof(float));
    load(&network,&activation);
    char filename[100];
    int result;
    SDL_Surface * img;
    //uint8_t array[MNIST_IMAGE_SIZE] = {0};
    uint8_t *array = calloc(784,sizeof(uint8_t));
    for(int i = 0; i < 81; i++)
    {
	printf("i = %i\n",i);
        snprintf(filename,100,"../../Initialization/Boxes/box_%i.bmp",i); //CHANGE
        img = SDL_LoadBMP((char *)filename);
        binarize2(img,array);//change
        result = compute(&array, &network);
        printf("result is : %i \n",result);
    }
}
