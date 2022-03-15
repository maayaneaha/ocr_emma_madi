#include "Tools.h"

int CheckCoord(struct Matrix matrix, int x, int y);

//========================================//
//	           	Start Matrices	    	  //
//========================================//

// Create a matrix; return the matrix
struct Matrix CreateMatrix(int rows, int columns)
{
	struct Matrix matrix;
       	
	matrix.rows = rows;
	matrix.columns = columns;

	matrix.mat = malloc(sizeof(double)*columns*rows);

	return matrix;
}

// Testing if given coordinates are correct
int CheckCoord(struct Matrix matrix, int x, int y)
{
	return (x < matrix.rows &&  y < matrix.columns); 
}


// Function to navigate in the matrix; return matrix(x, y)
double NavMatrix(struct Matrix matrix, int x, int y)
{
	if (!CheckCoord(matrix, x, y))
	{
		printf("====================\n");
		printf("Error in coordinates\n");
		printf("====================\n");
	}
	int index = x * matrix.columns + y;
	return *(matrix.mat + index);
}

// Function to change a value in a matrix
void ChangeMatrix(struct Matrix matrix, int x, int y, float val)
{
	if (!CheckCoord(matrix, x, y))
	{
		printf("====================\n");
        printf("Error in coordinates\n");
        printf("====================\n");
	}
    *(matrix.mat + x * matrix.columns + y) = val;
}

// Print the matrix
void PrintMatrix(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
	{
		for(int j = 0; j < matrix.columns; j++)
		{
			printf("%f ", NavMatrix(matrix, i, j));
		}
		printf("\n");
	}
}

void InitMatrix(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
    {
        for(int j = 0; j < matrix.columns; j++)
        {
            ChangeMatrix(matrix, i, j, Random());
        }
    }    
}

// Initialize the matrix with zeros
void InitMatrixZero(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
    {
        for(int j = 0; j < matrix.columns; j++)
        {
            ChangeMatrix(matrix, i, j, 0);
        }
    }

}
//========================================//
//              End Matrices              //
//========================================//

double Random()
{
    return (double)rand()/(double)RAND_MAX;
}