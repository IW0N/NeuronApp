#pragma once
#include "IFuncActivation.h"
class ReLU:public IFuncActivation
{
public:
	double calculate(double value) override;
	double differentiate(double value) override;
};

