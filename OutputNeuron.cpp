#include "OutputNeuron.h"
#include "OutputCalculator.h"

OutputNeuron::OutputNeuron(IFuncActivation* func,ILossFunction* loss):NeuronWithInputs(func)
{
	this->loss_func = loss;
	this->local_grad_calculator = new OutputCalculator(loss);
	this->predicted_output = 0;
}
void OutputNeuron::setPrdictedOutput(double predicted)
{
	this->predicted_output = predicted;
}
void OutputNeuron::updateErrorCoefficient()
{
	OutputCalculator* out_calc = (OutputCalculator*)local_grad_calculator;
	out_calc->desiredOutput = predicted_output;
	out_calc->output = output;
	NeuronWithInputs::updateErrorCoefficient();
}
OutputNeuron::~OutputNeuron() { }