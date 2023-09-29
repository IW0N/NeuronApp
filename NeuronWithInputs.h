#pragma once
#include <vector>
#include "Neuron.h"
#include "Connection.h"
#include "IFuncActivation.h"
#include "NeuronWithOutputs.h"
#include "ILossFunction.h"
#include "ILocalGradientCalculator.h"
class NeuronWithInputs:public Neuron
{
protected:
	std::vector<Connection*> inputs;
	double error_coefficient;
	//double local_gradient;
	IFuncActivation* activation;
	ILocalGradientCalculator* local_grad_calculator;
	virtual void updateErrorCoefficient();
public:
	NeuronWithInputs(IFuncActivation* func);
	double getErrorCoefficient();
	void bindWithInputNeuron(NeuronWithOutputs* neuron);
	virtual void recalculate();
	void setInputWeightGradients();
	void updateInputWeights(double train_speed);
	//void updateInputWeights(double train_speed);
	~NeuronWithInputs() override;
};
