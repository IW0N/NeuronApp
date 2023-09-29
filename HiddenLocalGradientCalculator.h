#pragma once
#include "ILocalGradientCalculator.h"
#include "Connection.h"
#include <vector>
class HiddenCalculator:public ILocalGradientCalculator
{
private:
	std::vector<Connection*> outputs;
public:
	HiddenCalculator(std::vector<Connection*> outputs);
	void setOutput(Connection* output);
	double calculateLocalGradient() override;
};