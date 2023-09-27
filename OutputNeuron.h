#include "NeuronWithInputs.h"
#include "ILossFunction.h"
#pragma once
class OutputNeuron:public NeuronWithInputs
{
private:
	ILossFunction* loss_func;
	double predicted_output;
protected:
	void updateErrorCoefficient() override;
public:
	OutputNeuron(IFuncActivation* func,ILossFunction* loss_func);
	void setPrdictedOutput(double predicted);
	~OutputNeuron() override;
};

