#pragma once
#include "NeuronWithOutputs.h"
#include <vector>
using namespace std;
class BiasNeuron:public NeuronWithOutputs
{
public:
	BiasNeuron(vector<Neuron*> next_layer);
};

