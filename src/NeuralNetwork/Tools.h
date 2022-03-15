#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>

//========================================//
//	     Matrices Functions           //
//========================================//

// Matrix object declaration
struct Matrix
{
        // Size of the matrix
        int rows;
        int columns;

        // Pointer to the matrix
        double *mat;
};

// Function to change a value in a matrix
void ChangeMatrix(struct Matrix matrix, int x, int y, float val);

// Function to navigate in the matrix; return matrix(x, y)
double NavMatrix(struct Matrix matrix, int x, int y);

void InitMatrix(struct Matrix matrix);

void InitMatrixZero(struct Matrix matrix);
// Create a matrix; return the matrix
struct Matrix CreateMatrix(int rows, int colums);

// Print the matrix
void PrintMatrix(struct Matrix matrix);

//========================================//
//		 YET TO COME		  //
//========================================//

double Random();

#endif