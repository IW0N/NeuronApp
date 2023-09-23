#pragma once
#include "Neuron.h"
#include <cstdlib>
#include <math.h>
class Connection
{
private:
	Neuron* input;
	Neuron* output;
	double weight;
public:
	Connection(Neuron* input, Neuron* output) {
		this->input = input;
		this->output = output;
		weight = rand() / 10000.0;
	}
};

