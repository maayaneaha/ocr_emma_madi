#ifndef NETWORK_H
#define NETWORK_H

float sigmoid(float a);
float dsigmoid(float a);
float init();
void suffle(int arr[], int size);
void forwardPass(int k);
void backprop(int k);
void learn();
float processXOR(int input1, int input2);
int roundUp(float x);

#endif
