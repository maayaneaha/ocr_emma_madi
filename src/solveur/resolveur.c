#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "auxiliaire_functions.c"

void convertisseur(FILE *Grille, int sudoku[9][9])
{
    while (!feof(Grille))
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9;)
            {
                int c = fgetc(Grille);
                if (c == ' ' || c == '\n')
                {}
                else
                {
                    if ( '0' <= c && c <= '9')
                    {
                        sudoku[i][j] = c - 48;
                        j++;
                    }
                    else
                    {
                        sudoku[i][j] = 0;
                        j++;
                    }
                }
            }
        }
        fclose(Grille);
    }

}

void result(int sudokuResolve[9][9], FILE *Result)
{
    for (int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
            fputc('\n', Result);
        for (int j = 0; j < 9; j++)
        {
            if (j == 3 || j == 6)
                fputc(' ', Result);
            int c = sudokuResolve[i][j];
            char c_char = c + '0';
            fputc(c_char, Result);
        }
        fputc('\n', Result);
    }
    //fputc(EOF , Result);
    fclose(Result);
}


// Main
int main (int argc, char *argv[])
{
    //initialisation de la grille
    int sudoku[9][9];

    // Initialisation des pointeurs de fichiers
    FILE *Grille;
    FILE *Result;

    if (argc == 1000)
    {}

    //Ouverture du fichier contenant la grille
    Grille = fopen(argv[1], "r");

    //Remplir le sudoku a resoudre a partir du fichier
    convertisseur(Grille, sudoku);

    //Resolvation du sudoku
    estValide(sudoku,0);

    // Creation du fichier .result
    Result = fopen(strcat(argv[1], ".result"), "w");
    result(sudoku, Result);

    return 0;
}
