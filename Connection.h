#pragma once
#include "Neuron.h"
#include <cstdlib>
#include <math.h>
class Connection
{
private:
	double weight_gradient;
public:
	Neuron* input;
	Neuron* output;
	double weight;
	Connection(Neuron* _input, Neuron* _output);
	void calculateWeightGradient();
	void updateWeight(double train_speed);
};

