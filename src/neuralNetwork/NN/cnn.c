#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// functions
// soft max function
float *softmax(const size_t size, float *vect){
	float buff[size];
	float sum = 0.0;
	for (int i = 0; i < size; i++){
		sum += exp(*(vect + i));
	}
	for (int i = 0; i < size; i++){
		buff[i] = exp(*(vect + i))/sum;
	}
	return &buff;
}
//// soft max derivative function
float *Dsoftmax(const size_t size, float *vect){
	float buff[size];
	float *act = softmax(size, vect);
	for (int i = 0; i < size; i++){
		buff[i] = (*(act + i))*(1 - *(act + i));
	}
	return buff
}

//// tanh activation function
float *act_tanh(const size_t size, float *mat){
	float buffer[size*size];
	for (size_t i = 0; i < size; i++){
		for (size_t j = 0; j < size; j++){
			buffer = tanh(*(mat + 2*i + j));
		}
	}

       
	return &buffer;
}

//// tanh derivative function
float *Dtanh(const size_t size, float *mat){
	float *buff[size*size];
	float x = 0.0;
	for (size_t i = 0; i < size; i++){
		for (size_t j = 0; j < size; j++){
			x = tanh(*(vect + 2*i + j));
			buffer[i] = 1 - (x*x);
		}
	}

       
	return &buffer;
}

//// Convolution
float convolute(float *mat1, float *mat2, const size_t size1){
	float sum = 0.0;

       
	for (size_t i = 0; i < size; i++){
		for (size_t j = 0; j < size; j++){
			sum += (*(mat1 + i*2 + j))*(*(mat2 + i*2 + j));
		}
	}

      
	return sum;
}

// Convolution layer
float *conv(const size_t imgsize, const size_t filtersize, const size_t padding, const size_t stride, , float *img, float *filter){
	const size_t ftmapsize = ((imgsize+(2*padding)-filtersize)/stride) + 1;
	float buffer[ftmapsize * ftmapsize];
	for (size_t i = 0; i < ftmapsize; i++){
		for (size_t j = 0; j < ftmapsize; i++){
			buffer[2*i + j] = convolute(img, filter, filtersize);
		}
	}


  	return act_tanh(ftmapsize, &buffer);
 
}

// Pooling

// Dense Layer
