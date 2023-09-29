#include "Connection.h"
#include "NeuronWithInputs.h"
#include "NeuronWithOutputs.h"
Connection::Connection(Neuron* input, Neuron* output) {
	if (dynamic_cast<NeuronWithOutputs*>(input)!=nullptr) {
		if (dynamic_cast<NeuronWithInputs*>(output)!=nullptr) {
			this->input = input;
			this->output = output;
			weight = (rand() % 10000) / 10000.0;
			weight_gradient = 0;
		}
		else
			throw "output не является NeuronWithInputs";
	}
	else
		throw "input не является NeuronWithOutputs";
}
void Connection::calculateWeightGradient() 
{
	double input = this->input->getOutput();
	NeuronWithInputs* casted_output = dynamic_cast<NeuronWithInputs*>(output);
	double error_coeff = casted_output->getErrorCoefficient();
	weight_gradient = error_coeff*input;
}
void Connection::updateWeight(double train_speed) {
	weight -= train_speed*weight_gradient;
}