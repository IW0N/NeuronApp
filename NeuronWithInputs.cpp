#include "NeuronWithInputs.h"
NeuronWithInputs::NeuronWithInputs(IFuncActivation* func) {
	this->activation = func;
	this->error_coefficient = 0;
	Neuron::output = 0;
	Neuron::input = 0;
	local_grad_calculator = nullptr;
}
void NeuronWithInputs::bindWithInputNeuron(NeuronWithOutputs* neuron) {
	Connection* connection=new Connection(neuron, this);
	inputs.push_back(connection);
	neuron->setConnection(connection);
}
double NeuronWithInputs::getErrorCoefficient() {
	return error_coefficient;
}
void NeuronWithInputs::recalculate() 
{
	input = 0;
	for (Connection* con : inputs) 
	{
		Neuron* inputNeuron=con->input;
		input += inputNeuron->getOutput() * con->weight;
	}
	output = activation->calculate(input);
	
}

void NeuronWithInputs::setInputWeightGradients()
{
	updateErrorCoefficient();
	for (Connection* con:inputs)
		con->calculateWeightGradient();
}
void NeuronWithInputs::updateErrorCoefficient() {
	double local_gradient = local_grad_calculator->calculateLocalGradient();
	double diff_sum = activation->differentiate(input);
	double error_coef = diff_sum*local_gradient;
	error_coefficient = error_coef;
}
void NeuronWithInputs::updateInputWeights(double train_speed) 
{
	for (Connection* con : inputs)
		con->updateWeight(train_speed);
}
NeuronWithInputs::~NeuronWithInputs() {
	delete this->activation;
	this->inputs.clear();
}