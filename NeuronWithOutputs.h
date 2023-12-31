#include "Neuron.h"
#include <vector>
#include "Connection.h"
#pragma once
class NeuronWithOutputs:public Neuron
{
protected:
	std::vector<Connection*> outputs;
public:
	virtual void setConnection(Connection* c);
	~NeuronWithOutputs() override;
};

