#include <stdio.h>

//This function if we can assign num to the given [row][col]
//It it can, it will put num in the [row][col], otherwise, it will search
//recursively where num can be put

int isAvailable(int sudoku[][9], int row, int col, int num)
{
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;

    for(int i = 0; i < 9; ++i)
    {
        if (sudoku[row][i] == num)
        {
            return 0;
        }

        if (sudoku[i][col] == num)
        {
            return 0;
        }

        if (sudoku[rowStart + (i%3)][colStart + (i/3)] == num)
        {
            return 0;
        }
    }
    return 1;
}

//This function solve the given sudoku

int SolveSudoku(int sudoku[][9], int row, int col)
{
    if(row < 9 && col < 9)
    {
        if(sudoku[row][col] != 0)
        {
            if((col+1) < 9)
            {
                return SolveSudoku(sudoku, row, col+1);
            }

            else
            {
                if((row+1) < 9)
                {
                    return SolveSudoku(sudoku, row+1, 0);
                }

                else
                {
                    return 1;
                }
            }
        }
        else
        {
            for(int i = 0; i < 9; ++i)
            {
                if(isAvailable(sudoku, row, col, i+1))
                {
                    sudoku[row][col] = i+1;
                    if((col+1) < 9)
                    {
                        if(SolveSudoku(sudoku, row, col +1))
                        {
                            return 1;
                        }

                        else
                        {
                            sudoku[row][col] = 0;
                        }
                    }

                    else if((row+1) < 9)
                    {
                        if(SolveSudoku(sudoku, row+1, 0))
                        {
                            return 1;
                        }

                        else
                        {
                            sudoku[row][col] = 0;
                        }

                    }

                    else return 1;
                }
            }
        }

        return 0;
    }

    else return 1;
}
