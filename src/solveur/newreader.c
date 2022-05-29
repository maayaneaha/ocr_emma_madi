#include <stdio.h>
#include "newreader.h"
#include <stdlib.h>

void reader(char argv[], char grid[81]){
	FILE *inputFile;
	inputFile = fopen(argv,"r");
	char ch;
	for(int i = 0; i < 81; i++)
	{
		ch = fgetc(inputFile);
		if (ch != '\n' && ch != 32)
		{
			grid[i] = ch;
		}
		else
			i--;
	}
	fclose(inputFile);
}
