#include "HiddenLocalGradientCalculator.h"
#include "NeuronWithInputs.h"
HiddenCalculator::HiddenCalculator(std::vector<Connection*> outputs)
{
	this->outputs = outputs;
}

double HiddenCalculator::calculateLocalGradient()
{
	double local_gradient = 0;
	for (Connection* con : outputs) {
		Neuron* _output = con->output;
		NeuronWithInputs* output = (NeuronWithInputs*)_output;
		double out_eror_coef = output->getErrorCoefficient();
		double weight = con->weight;
		local_gradient += out_eror_coef * weight;
	}
	return local_gradient;
}

