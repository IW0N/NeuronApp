#pragma once
#include "IFuncActivation.h"
class Sigmoida:public IFuncActivation
{
public:
	double calculate(double v) override;
	double differentiate(double v) override;
};

