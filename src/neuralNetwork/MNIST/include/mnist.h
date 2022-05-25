#ifndef MNIST_H
#define MNIST_H
#include "neural_network.h"

int compute(uint8_t **param,neural_network_t * network);
void save(neural_network_t* network);
void load(neural_network_t* network, float ** activations);
#endif
