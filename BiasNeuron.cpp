#include "BiasNeuron.h"
#include "NeuronWithInputs.h"
BiasNeuron::BiasNeuron(vector<Neuron*> next_layer)
{
	input = 1;
	output = 1;
	for (Neuron* n:next_layer)
	{
		NeuronWithInputs* output = dynamic_cast<NeuronWithInputs*>(n);
		output->bindWithInputNeuron(this);
	}
}
