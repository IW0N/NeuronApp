#pragma once
#include <vector>
#include "IFuncActivation.h"
using namespace std;
class Neuron
{
private:
	vector<Connection> inputs;
	vector<Connection> outputs;
	double local_gradient;
	double sum_value;
	double norm_value;
	IFuncActivation* func;
public:
	Neuron(IFuncActivation* func) {
		this->func = func;
	}
	void bindWithOutNeuron(Neuron* neuron) {
		Connection connection(this,neuron);
		outputs.push_back(connection);
		neuron->inputs.push_back(connection);
	}
};

