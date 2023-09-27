#pragma once
class Neuron
{
protected:
	double input;
	double output;
public:
	double getOutput();
	virtual ~Neuron();
};