#include "grid_result.h"

SDL_Surface *to_img(char* filepath)
{
    SDL_Surface *out = SDL_CreateRGBSurface(0, 452, 452, 32,0,0,0,0);

    FILE* initial  = fopen(filepath,"r");
    int size = strlen(filepath) + 8;
    char* resfilepath = (char *) malloc(size);
    strcat( strcpy(resfilepath, filepath), ".output" );
    FILE* result = fopen(resfilepath, "r");
    free(resfilepath);
    
    int x = 2;
    int y = 2;

    if(initial != NULL)
    {
        char c1 = 0;
        char c2 = 0;

        //bool used to escape the format char in textfile (' ' or '\n')
        int hasJumped = 1; 

        for(int i = 0; i < 9; i++)
        {
            if(i%3==0 && !hasJumped)
            {
                c1 = fgetc(initial);
                c2 = fgetc(result);
                hasJumped = 1;
                i--;
                continue;
            }
            for(int j = 0; j < 9; j++)
            {
                c1 = fgetc(initial);
                c2 = fgetc(result);
                if(j%3==0 && c1 == ' ')
                {
                    j--;
                    continue;
                }
                if((c1 >='1' && c1 <= '9') && (c1 == c2))
                {
                    insert_case_img(out, c1, x, y, 'N');
                }
                else if(c1 == '.')
                {
                    insert_case_img(out, c2, x, y, 'R');
                }
                x+=50;
            }
            c1 = fgetc(initial);
            c2 = fgetc(result);//escape the '\n' at end of line
            hasJumped = 0;
            y+=50;
            x = 2;
        }
    }

    return out;
}

void insert_case_img(SDL_Surface* result, char number, int x, int y, char letter)
{
    char number_img_path[] = "numbers/1B.bmp";
    number_img_path[8] = number;
    number_img_path[9] = letter;
    
    SDL_Surface* number_img = load_img(number_img_path);
    int tmpy = y;
    int tmpx = x;
    for(int i = 2; i < 50; i++)
    {
        for(int j = 2; j < 50; j++)
        {
            Uint32 pixel = get_pixel(number_img, i, j);
            put_pixel(result, tmpx,tmpy, pixel);
            tmpy++;
        }
        tmpx++;
        tmpy = y;
    }
}

int main(void)
{
    SDL_Surface* out = to_img("grid_00.result");
    SDL_SaveBMP(out, "output/result.bmp");

    return 0;
}
