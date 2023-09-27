#include "HiddenNeuron.h"
#include "ReLU.h"
#include "OutputNeuron.h"
#include "HiddenLocalGradientCalculator.h"
HiddenNeuron::HiddenNeuron(IFuncActivation* func):NeuronWithInputs(func) {
	this->local_grad_calculator = new HiddenCalculator(outputs);
	//auto hid = (HiddenCalculator*)local_grad_calculator;
}
void HiddenNeuron::calculateErrorCoefficient() {
	double local_gradient = activation->differentiate(Neuron::input);
	double error_coef = 0;
	for (Connection* con : outputs) {
		Neuron* _output = con->output;
		NeuronWithInputs* output = (NeuronWithInputs*)_output;
		double out_eror_coef = output->getErrorCoefficient();
		double weight = con->weight;
		error_coef += out_eror_coef*weight;
	}
	this->error_coefficient = error_coef;
}

HiddenNeuron* HiddenNeuron::fromBase(Neuron* n) {
	NeuronWithInputs* parent0 = (NeuronWithInputs*)n;
	HiddenNeuron* root = (HiddenNeuron*)parent0;
	return root;
}
Neuron* HiddenNeuron::toBase() {
	NeuronWithInputs* parent0 = (NeuronWithInputs*)this;
	Neuron* base = parent0;
	return base;
}
HiddenNeuron::~HiddenNeuron(){
}