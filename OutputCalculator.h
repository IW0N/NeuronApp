#pragma once
#include "ILocalGradientCalculator.h"
#include "ILossFunction.h"
class OutputCalculator : public ILocalGradientCalculator
{
private:
	ILossFunction* loss;
public:
	double output;
	double desiredOutput;
	OutputCalculator(ILossFunction* loss);
	double calculateLocalGradient() override;
};