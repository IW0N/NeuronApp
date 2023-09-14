#pragma once

class IFuncActivation
{
public:
	double virtual calculate(double value)=0;
	double virtual differentiate(double value)=0;
};
