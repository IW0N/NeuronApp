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
		NeuronWithInputs* output = dynamic_cast<NeuronWithInputs*>(con->output);
		double out_eror_coef = output->getErrorCoefficient();
		double weight = con->weight;
		error_coef += out_eror_coef*weight;
	}
	this->error_coefficient = error_coef;
}

void HiddenNeuron::recalculate()
{
	NeuronWithInputs::recalculate();
	NeuronWithOutputs::input = NeuronWithInputs::input;
	NeuronWithOutputs::output = NeuronWithInputs::output;
}

void HiddenNeuron::setConnection(Connection* con)
{
	HiddenCalculator* calc = static_cast<HiddenCalculator*>(local_grad_calculator);
	calc->setOutput(con);
	NeuronWithOutputs::setConnection(con);

}

HiddenNeuron* HiddenNeuron::fromBase(Neuron* n) {
	NeuronWithOutputs* parent0 = (NeuronWithOutputs*)n;
	HiddenNeuron* root = (HiddenNeuron*)parent0;
	return root;
}
Neuron* HiddenNeuron::toBase() {
	NeuronWithOutputs* parent0 = (NeuronWithOutputs*)this;
	Neuron* base = parent0;
	return base;
}
HiddenNeuron::~HiddenNeuron(){
}