#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "include/mnist_file.h"
#include "include/neural_network.h"

#define STEPS 15 // defalut is 1000
#define BATCH_SIZE 100

/**
 * Downloaded from: http://yann.lecun.com/exdb/mnist/
 */
const char * train_images_file = "data/train-images-idx3-ubyte";
const char * train_labels_file = "data/train-labels-idx1-ubyte";
const char * test_images_file = "data/t10k-images-idx3-ubyte";
const char * test_labels_file = "data/t10k-labels-idx1-ubyte";

/**
 * Calculate the accuracy of the predictions of a neural network on a dataset.
 */
float calculate_accuracy(mnist_dataset_t * dataset, neural_network_t * network)
{
    float activations[MNIST_LABELS], max_activation;
    int i, j, correct, predict;

    // Loop through the dataset
    for (i = 0, correct = 0; i < (int)dataset->size; i++) {
        // Calculate the activations for each image using the neural network
        neural_network_hypothesis(&dataset->images[i], network, activations);

        // Set predict to the index of the greatest activation
        for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }

        // Increment the correct count if we predicted the right label
        if (predict == dataset->labels[i]) {
            correct++;
        }
    }
   // SAVE ACTIVATIONS VECTOR
   FILE * activations_file = fopen("save/activation","wb");
   for(int i = 0; i < MNIST_LABELS; i++)
   {
	   fwrite(&activations[i],sizeof(float),1,activations_file);
   }
   fclose(activations_file);
   // Return the percentage we predicted correctly as the accuracy
   return ((float) correct) / ((float) dataset->size);
}

int compute(uint8_t **param,neural_network_t * network)
{
    printf("Compute the digit with the network\n");
    float * activations = (float*)calloc(MNIST_LABELS,sizeof(float));
    mnist_image_t image;
    for(int i = 0; i < MNIST_IMAGE_SIZE; i++)
    {
        image.pixels[i] = (*param)[i];
    }
    neural_network_hypothesis(&image,network,activations);
    int predict = 0;
    float max_activation = activations[0];
    printf(" ------------------ COMPUTE ----------------------\n");
    for(int i = 0; i < (int)MNIST_LABELS; i++)
    {
        if(max_activation < activations[i])
        {
            max_activation = activations[i];
            predict = i;
	        printf("predict becomes %i\n",predict);
	        printf("max_activation is : %f\n",max_activation);
        }

    	printf("activations[%i] is %f\n",i,activations[i]);
    }

    return predict;
}

void save(neural_network_t* network)
{
    printf("Saving the network's weights and biases\n");
    printf("Save function()\n");
    printf("Etape 01\n");
    FILE * bias_file = fopen("save/bias","wb"); //change
    if(!bias_file)
    {
        printf("bias file does not exist.\n");
        exit(0);
    }
    for(int i = 0; i < MNIST_LABELS; i++)
    {
        fwrite(&(network->b[i]),sizeof(float),1,bias_file);
    }
    fclose(bias_file);
    FILE * weight_file = fopen("save/weights","wb"); //change
    if(!weight_file)
    {
        printf("weight file does not exist.\n");
        exit(0);
    }
    printf("Etape 02\n");
    for(int i = 0; i < MNIST_LABELS; i++)
    {
        for(int j = 0; j < MNIST_IMAGE_SIZE; j++)
        {
            fwrite(&(network->W[i][j]),sizeof(float),1,weight_file);
        }
    }
    fclose(weight_file);
}

void load(neural_network_t* network, float ** activations)
{
    printf("Loading the network's weights and biases\n");
    printf("Load function()\n");
    FILE * bias_file = fopen("save/bias","rb"); //change
    //rewind(bias_file);

    float r = 0;
    printf("Etape 1 Load\n");
    for(int i = 0; i < MNIST_LABELS; i++)
    {
	    printf(" i = %i\n",i);
        fread(&r,sizeof(float),1,bias_file);
	    printf("r = %f",r);
        printf("expected %f, received %f\n",network->b[i],r);
        network->b[i] = r;
    }
    fclose(bias_file);
    printf("Etape 2 Load\n");
    FILE * weight_file = fopen("save/weights","rb"); //change
    //rewind(weight_file);
    int i = 0;
    int j = 0;
    int cpt = 0;
    while(i < MNIST_LABELS)
    {
	    j = 0;    
        while(j <  MNIST_IMAGE_SIZE)
        {
	//	printf("Step is : %i and cpt = %i\n",i*MNIST_IMAGE_SIZE+j,cpt);
            if(cpt < 7840)
	        {
	            fread(&r,sizeof(float),1,weight_file);
                	//printf("expected %f, received %f\n",network->W[i][j],r);
  	     	    if(!isnan(r))
	     	    {
      //      		printf("r = %f at step %i \n",r,i*MNIST_IMAGE_SIZE+j);
             		network->W[i][j] = r;
	     	    }
	        }

	     j++;
	     cpt++;
        }
	    i++;
    }
    fclose(weight_file);

    FILE * activation_file = fopen("save/activation","rb"); //change
    rewind(activation_file);
    float f = 0;
    for(int i = 0; i < 10; i++)
    {
	    printf(" load : i is %i\n",i);
	    fread(&f,sizeof(float),1,activation_file);
	    (*activations)[i] = f;
    }
    fclose(activation_file);

}

int main(int argc, char *argv[])
{
    printf("Step is set to : %i should be 1000 \n",STEPS);
    mnist_dataset_t * train_dataset, * test_dataset;
    mnist_dataset_t batch;
    neural_network_t network;
    float loss, accuracy;
    int i, batches;

    // Read the datasets from the files
    train_dataset = mnist_get_dataset(train_images_file, train_labels_file);
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);

    // Initialise weights and biases with random values
    neural_network_random_weights(&network);

    // Calculate how many batches (so we know when to wrap around)
    batches = train_dataset->size / BATCH_SIZE;

    for (i = 0; i < STEPS; i++) {
        // Initialise a new batch
        mnist_batch(train_dataset, &batch, 100, i % batches);

        // Run one step of gradient descent and calculate the loss
        loss = neural_network_training_step(&batch, &network, 0.5);

        // Calculate the accuracy using the whole test dataset
        accuracy = calculate_accuracy(test_dataset, &network);

        printf("Step %04d\tAverage Loss: %.2f\tAccuracy: %.3f\n", i, loss / batch.size, accuracy);
    }
    save(&network);
    //load(&network);
    // Cleanup
    mnist_free_dataset(train_dataset);
    mnist_free_dataset(test_dataset);

    return 0;
}

