#include "Connection.h"
#include "OutputNeuron.h"
#include "HiddenNeuron.h"
Connection::Connection(Neuron* input, Neuron* output) {
	this->input = input;
	this->output = output;
	const double max = (double)INT32_MAX;
	weight = rand()/max;
	weight_gradient = 0;
}
void Connection::calculateWeightGradient() 
{
	double input = this->input->getOutput();
	NeuronWithInputs* out_with_inputs = (NeuronWithInputs*)output;
	double error_coeff = out_with_inputs->getErrorCoefficient();
	weight_gradient = error_coeff*input;
}
void Connection::updateWeight(double train_speed) {
	weight -= train_speed*weight_gradient;
}