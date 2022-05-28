#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "solver.h"

void Convert(FILE *Grille, int sudoku[9][9])
{
  char c = '0';
  int a = 0;
  int i = 0;
  int j = 0;
  while(i < 9)
  {
      while(j < 9)
      {
        fread(&c,1,1,Grille);
        a = c-48;
        if(c == ' ' || c == '\n')
        {}
        else
        {
          if(0 <= a && a <= 9)
          {
            sudoku[i][j] = a;
            //j++;
          }
          else
          {
            sudoku[i][j] = 0;
            //j++;
          }
          j++;
        }
      }
      j = 0;
      i++;
      //printf("\n");
    }

  fclose(Grille);

}

void solver(int Sudoku[9][9], FILE *Result)
{
    // PRINT RESULT IN THE FILE
    for(int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
        {
            fputc('\n', Result);
        }

        for(int j = 0; j < 9; j++)
        {
            if (j == 3 || j == 6)
                fputc(' ', Result);
            int r = Sudoku[i][j];
            char w = (char)(r + 48);
            fputc(w, Result);
        }
        fputc('\n', Result);
    }
    //fputc(EOF, Result);
    fclose(Result);
}

int main(int argc, char *argv[])
{
    int sudoku[9][9];

    FILE *Grille;
    FILE *Result;

    if(argc > 10000000)
    {
        return 0;
    }

    Grille = fopen(argv[1], "r");

    Convert(Grille, sudoku);

    SolveSudoku(sudoku, 0, 0);
    char * filename = strcat(argv[1],".result");
    Result = fopen(filename, "w");
    solver(sudoku, Result);

    return 0;
}
