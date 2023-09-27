#pragma once
#include "NeuronWithOutputs.h"
class InputNeuron:public NeuronWithOutputs
{
public:
	void setInput(double value);
	~InputNeuron() override;
};

