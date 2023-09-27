#include "NeuronWithOutputs.h"
#include "HiddenNeuron.h"
void NeuronWithOutputs::setConnection(Connection* connection) {
	outputs.push_back(connection);
}
NeuronWithOutputs::~NeuronWithOutputs() {
	outputs.clear();
}