#include "InputNeuron.h"
void InputNeuron::setInput(double value) {
	input = value;
	output = input;
}
InputNeuron::~InputNeuron() { }