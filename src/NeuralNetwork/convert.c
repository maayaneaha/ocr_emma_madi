/*
 * =====================================================================================
 *
 *       Filename:  convert.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/03/2021 03:39:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/neural_network.h"
#include "../ImageProcessing/color.h"
#include "mnist.h"
void binarize2(SDL_Surface * image, uint8_t * array)
{
    const int WHITE = 0;
    const int BLACK = 255;
    Uint8 red = 0; // if red > 0, the pixel is white else black
    Uint8 blue = 0;
    Uint8 green = 0;
    Uint32 * pixels = (Uint32 *)image->pixels;
    // Remove borders
/*    for (Uint32 h = 0; h < (Uint32)(image->w);h++)
    {
        for(Uint32 w = 0; w < (Uint32)(image->w);w++)
        {
            if( h < 3 || h > (Uint32)(image->h) - 3 || w < 3 || w > (Uint32)(image->w) - 3)
            {
                array[w*image->w+h] = WHITE;
            }
        }
    }
    */
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
              //printf("%i ",(red+blue+green)/3);
        }
       // printf("\n");
    }
    for (int h = 0; h < image->h; h++)
    {
        for(int w = 0; w < image->w; w++)
         {  if(array[h * image->h +w] == BLACK)
                printf("1");
            else
                printf("0");
         }
         printf("\n");
     }

}
void convert()
{
    neural_network_t network;
    //neural_network_random_weights(&network);
    //save(&network);
    float *activation = (float *)malloc(10*sizeof(float));
    load(&network,&activation);
    activation = (float *)malloc(10*sizeof(float));
    char filename[100];
    int result;
    SDL_Surface * img;
    //uint8_t array[MNIST_IMAGE_SIZE] = {0};
    uint8_t *array = (uint8_t *)calloc(784,sizeof(uint8_t));
    FILE * gridF = fopen("../ImageProcessing/grid","w");
    for(int i = 0; i < 81; i++)
    {
        snprintf(filename,100,"../ImageProcessing/Boxes/box_%i.bmp",i);
        img = SDL_LoadBMP((char *)filename);
        binarize2(img,array);
        result = compute(&array, &network);
        printf("result is : %i \n",result);
    }
    
}
