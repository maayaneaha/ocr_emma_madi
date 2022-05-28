#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <err.h>

#include "Network.h"


//Constants

#define nbInputs 2		//number of inputs
#define nbNeur1 2		//number of neurons in hidden layer
#define nbOutputs 1		//number of outputs
#define nbTS 4			//number of training sets
//#define nbGen 10000000		//number of training generations


//Training sets

float train_inputs[nbTS][nbInputs] = {{0, 0}, {1, 1}, {1, 0}, {0, 1}};
float train_outputs[nbTS][nbOutputs] = {{0}, {0}, {1}, {1}};


//Network variables

//Values
//input values
float inputVal[nbInputs];
//first hidden layer values
float Val1[nbNeur1];
//output value
float output[nbOutputs];

//Weights
//first layer weights
float weights0[nbInputs][nbNeur1];
//output weights
float Outweights[nbNeur1][nbOutputs];

//Biases
//first layer biases
float biases0[nbNeur1];
//output biases
float biases1[nbOutputs];


//Functions

//Activation
//sigmoid function
float sigmoid(float a){
	return 1 / (1 + (float)exp((double)(-a)));
}

//		derivative of sigmoid
float dsigmoid(float a){
	return a * (1 - a);
	}
		//Randomizations
	//	random weights and biases init between 0 and 1
float init(){
	return ((float)rand()) / ((float)RAND_MAX);
}
//	suffle an array
void suffle(int arr[], int size){
	if(size > 1){
		for(int i = 0; i < size; i++){

			int j = i + rand() / (RAND_MAX / (size - i) + 1);
			int temp = arr[j];
			arr[j] = arr[i];
			arr[i] = temp;
		}
	}
}

//Forward Pass
void forwardPass(int k){

	//compute hidden layer activation

	for(int l = 0; l < nbNeur1; l++){
		float activation = biases0[l];
		for(int m = 0; m < nbInputs; m++){
			activation += train_inputs[k][m] * weights0[m][l];
		}
		Val1[l] = sigmoid(activation);
	}


	//Compute output activation
	for(int l = 0; l < nbOutputs; l++){
		float activation = biases1[l];
	
		for(int m = 0; m < nbNeur1; m++){
			activation += Val1[m]*Outweights[m][l];
		}
		output[l] = sigmoid(activation);
	}
}


//Back Propagation
//»·····Using Stochastic Gradient Descent (SGD)
void backprop(int k){
	//Compute next gen output weights
	float deltaOut[nbOutputs];
	for(int i = 0; i < nbOutputs; i++){
		float dErr = train_outputs[k][i] - output[i];
		deltaOut[i] = dErr * dsigmoid(output[i]);
	}


	//Compute next gen hidden weights
	float deltaH[nbNeur1];

	for(int i = 0; i < nbNeur1; i++){
		float dErr = 0;
		for(int j = 0; j < nbOutputs; j++){
			dErr += deltaOut[j] * Outweights[i][j];
		}
		deltaH[i] = dErr * dsigmoid(Val1[i]);
	}


	//Apply next gen output weight

	for(int i = 0; i < nbOutputs; i++){

		biases1[i] += deltaOut[i] * 0.1;

		for(int j = 0; j < nbNeur1; j++){
			Outweights[j][i] += Val1[j] * deltaOut[i] * 0.1;

		}
	}

	//Apply next gen hidden weights
	for(int i = 0; i < nbNeur1; i++){
	
		biases0[i] += deltaH[i] * 0.1;
		for(int j = 0; j < nbInputs; j++){
			weights0[j][i] += train_inputs[k][j] * deltaH[i] * 0.1;

		}
	}
}


void rand_init(){
//initialize hidden layer weights
	for(int i = 0; i < nbInputs; i++){

		for(int j = 0; j < nbNeur1; j++){
			weights0[i][j] = init();
		}
	}

	//initialize output weights and hidden layer biases

	for(int i = 0; i < nbNeur1; i++){
		Val1[i] = init();
		for(int j = 0; j < nbOutputs; j++){
			Outweights[i][j] = init();
		}
	}

	//initialize output biases
	for(int i = 0; i < nbOutputs; i++){

		biases1[i] = init();

	}
}


void learn(int nbGen){

	for (int i = 0; i < nbGen; i++){

		//shuffle the training set

		//required by SGD
	
		int order[] = {0, 1, 2, 3};
		suffle(order, nbTS);



		//Train with suffled training set
		
		for(int j = 0; j < nbTS; j++){
			int k = order[j];
			forwardPass(k);
			backprop(k);
		}
	}
}

float processXOR(int input1, int input2){
	//basicaly the same as forwardPass(...)
			

	//compute hidden layer activation
	
	for(int i = 0; i < nbNeur1; i++){
		Val1[i] = sigmoid(biases0[i]
				+ (input1 * weights0[0][i])

				+ (input2 * weights0[1][i]));


	}



	//Compute output activation


	for(int i = 0; i < nbOutputs; i++){

		float activation = biases1[i];
		for(int j = 0; j < nbNeur1; j++){
			activation += Val1[j] * Outweights[j][i];
			output[i] = sigmoid(activation);
		}
	}


	return output[0];																																																																																		}

	int roundUp(float x){
		//to get a clean result
	if (x < 0.5) {return 0;}
	
	else {return 1;}
	
	}



void write(char *path)

{

	//write on the file

	FILE *fp = fopen(path, "w");



	//save values

	////first layer

	for(size_t i = 0; i<nbInputs; i ++)

	{

		for(size_t j = 0; j<nbNeur1; j++)

		{

			//write the weight in the file

			fprintf(fp, "%f ", weights0[i][j]);

		}

	}

	fputs("\n", fp); //separate weights and biases

	for(size_t i = 0; i< nbNeur1; i++)

	{

		//write the biases in the file

		fprintf(fp, "%f ", biases0[i]);

	}

	fputs("\n", fp); //separate the layers



	////output layer

	for(size_t i = 0; i<nbNeur1; i++)

	{

		for(size_t j = 0; j<nbOutputs; j++)

		{

			fprintf(fp, "%f ", Outweights[i][j]);

		}

	}



	fputs("\n", fp);//separate weights and biases

	for(size_t i = 0; i<nbOutputs; i++)

	{

		//write the biases

		fprintf(fp, "%f ", biases1[i]);

	}

	fputs("\n", fp);

	fclose(fp); //close the file

}



void load(char *path){

	FILE *fp = fopen(path, "r");

	if (fp == NULL){

		rand_init();

	}

	else{

		//get first layer weights



		float val00 = 0.0;

		float val01 = 0.0;

		float val10 = 0.0;

		float val11 = 0.0;

		fscanf(fp, "%f %f %f %f \n", &val00, &val01, &val10, &val11);



		weights0[0][0] = val00;

		weights0[0][1] = val01;

		weights0[1][0] = val10;

		weights0[1][1] = val11;



		// get first layer biases

		fscanf(fp, "%f %f ", &val00, &val11);

		biases0[0] = val00;

		biases0[0] = val11;


		// get output layer weights

		fscanf(fp, "%f %f ", &val00, &val11);

		Outweights[0][0] = val00;

		Outweights[1][0] = val11;



		// get output layer biases

		fscanf(fp, "%f ", &val00);

		biases1[0] = val00;



		fclose(fp);

	}

}

int main(int argc, char* argv[]){

	if (argc < 2){

		printf("Expected: ./Network nbGenerations");

		return 1;

	}

	else{

		const int nbGen = atoi(argv[1]);

		load("safe.txt");

		learn(nbGen);

		float o_o = processXOR(0, 0);

		float I_o = processXOR(1, 0);

		float I_I = processXOR(1, 1);

		float o_I = processXOR(0, 1);


		printf("\n0 xor 0 = %i (exactly = %f)\n", roundUp(o_o), o_o);

		printf("\n1 xor 0 = %i (exactly = %f)\n", roundUp(I_o), I_o);

		printf("\n1 xor 1 = %i (exactly = %f)\n", roundUp(I_I), I_I);

		printf("\n0 xor 1 = %i (exactly = %f)\n", roundUp(o_I), o_I);



		write("safe.txt");



		return 0;

	}

}
