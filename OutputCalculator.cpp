#include "OutputCalculator.h"

OutputCalculator::OutputCalculator(ILossFunction* loss)
{
	this->loss = loss;
	output = 0;
	desiredOutput = 0;
}

double OutputCalculator::calculateLocalGradient()
{
	double result = loss->differentiate(output,desiredOutput);
	return result;
}
