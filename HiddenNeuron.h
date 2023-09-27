#pragma once
#include "NeuronWithInputs.h"
#include "NeuronWithOutputs.h"
class HiddenNeuron:public NeuronWithOutputs, public NeuronWithInputs
{
public:
	HiddenNeuron(IFuncActivation* func);
	void calculateErrorCoefficient();
	
	Neuron* toBase();
	static HiddenNeuron* fromBase(Neuron* n);
	~HiddenNeuron() override;
};

