#pragma once
#include "NeuronNet.h"
#include "InputNeuron.h"
#include "HiddenNeuron.h"
#include "OutputNeuron.h"
using namespace std;
Neuron* NeuronNet::getNeuron(IFuncActivation* activation,int layer_index, int layers_count) 
{
	Neuron* n = nullptr;
	if (layer_index == 0)
		n = new InputNeuron();
	else if (layer_index < layers_count - 1) {
		HiddenNeuron* root=new HiddenNeuron(activation);
		n = root->toBase();
	}
	else
		n = new OutputNeuron(activation,loss_func);
	return n;
}

vector<Neuron*> NeuronNet::generateLayer(int layer_index,int layers_count,int size,IFuncActivation* func) 
{
	vector<Neuron*> layer;
	for (int i = 0; i < size;i++) 
	{
		Neuron* n = getNeuron(func,layer_index,layers_count);
		layer.push_back(n);
	}
	return layer;
}
void NeuronNet::bindLayerWithPrevious(vector<Neuron*> layer,int layer_index) {
	int prev_index = layer_index-1;
	vector<Neuron*> prev_layer = layers[layer_index - 1];
	for (int i = 0; i < layer.size(); i++) {
		Neuron* output = layer[i];
		bindWithLayer(output, prev_layer);
	}
}
void NeuronNet::initLayers(vector<int> layer_sizes, IFuncActivation* func)
{
	int layers_count = layer_sizes.size();
	for (int l = 0; l < layers_count; l++)
	{
		int layer_size = layer_sizes[l];
		vector<Neuron*> layer=generateLayer(l,layers_count,layer_size,func);
		if (l > 0)
			bindLayerWithPrevious(layer,l);
		layers.push_back(layer);
	}
}
void NeuronNet::bindWithLayer(Neuron* output, vector<Neuron*> prev_layer)
{
	NeuronWithInputs* inputable = static_cast<NeuronWithInputs*>(output);
	for (int j = 0; j < prev_layer.size(); j++)
	{
		Neuron* input = prev_layer[j];
		NeuronWithOutputs* casted_input = static_cast<NeuronWithOutputs*>(input);
		inputable->bindWithInputNeuron(casted_input);
	}
}

NeuronNet::NeuronNet(vector<int> layer_sizes, IFuncActivation* func,ILossFunction* loss)
{
	srand(time(NULL));
	this->loss_func = loss;
	initLayers(layer_sizes, func);
}
void NeuronNet::setInputs(vector<double> inputs)
{
	vector<Neuron*> firstLayer = layers[0];
	if (firstLayer.size() != inputs.size())
		throw "Некорректная длина входных данных!";
	for (int i = 0; i < inputs.size(); i++)
	{
		Neuron* neuron = firstLayer[i];
		InputNeuron* input = (InputNeuron*)neuron;
		double data = inputs[i];
		input->setInput(data);
	}
}
void NeuronNet::setPredictedOutputs(vector<double> outputs) 
{
	vector<Neuron*> lastLayer = layers.back();
	if (lastLayer.size() != outputs.size())
		throw "Некорректная длина выходных данных!";
	for (int i = 0; i < outputs.size(); i++)
	{
		Neuron* neuron = lastLayer[i];
		OutputNeuron* outNeuron = (OutputNeuron*)neuron;
		double data = outputs[i];
		outNeuron->setPrdictedOutput(data);
	}
}

vector<double> NeuronNet::getData(vector<double> inputs)
{
	setInputs(inputs);
	recalculateNeurons();
	vector<double> outputs = getOutput();
	return outputs;
}
void NeuronNet::recalculateNeurons()
{
	for (int l = 1; l < layers.size();l++) 
	{
		vector<Neuron*> layer = layers[l];
		for (int i = 0; i < layer.size();i++) 
		{
			Neuron* n = layer[i];
			NeuronWithInputs* neuron = (NeuronWithInputs*)n;
			neuron->recalculate();
		}
	}
}
vector<double> NeuronNet::getOutput() {
	vector<Neuron*> last = layers.back();
	vector<double> outputs;
	for (Neuron* n : last)
	{
		double out = n->getOutput();
		outputs.push_back(out);
	}
	return outputs;
}

void NeuronNet::train(vector<TrainPair> dataSet,int epoches,double train_speed) 
{
	double middle = 0;
	for (int epoch = 0; epoch < epoches;epoch++) {
		for (TrainPair pair:dataSet)
			learnTrainData(pair,train_speed);
		int index = rand()%10000;
		TrainPair pair = dataSet[123];
		vector<double> predicted=pair.output;
		double error = getTotalError(predicted);
		cout << error<<endl;
	}
}
void NeuronNet::learnTrainData(TrainPair pair,double train_speed)
{
	setInputs(pair.input);
	recalculateNeurons();
	setPredictedOutputs(pair.output);
	int last_index = layers.size()-1;
	for (int l = last_index;l>=1;l--) 
	{
		vector<Neuron*> layer = layers[l];
		for (Neuron* n:layer) {
			NeuronWithInputs* withInputs = (NeuronWithInputs*)n;
			withInputs->setInputWeightGradients();
		}
	}
	for (int l = last_index; l >= 1; l--) {
		vector<Neuron*> layer = layers[l];
		for (Neuron* n:layer) {
			NeuronWithInputs* withInputs = (NeuronWithInputs*)n;
			withInputs->updateInputWeights(train_speed);
		}
	}
}
double NeuronNet::getTotalError(vector<double> predicted) {
	vector<Neuron*> neurons = layers.back();
	auto func = [&neurons](int index)
	{
		return neurons[index]->getOutput();
	};
	double error=loss_func->calculateTotalError(func, predicted);
	return error;
}
NeuronNet::~NeuronNet() {
	int last_index = layers.size()-1;
	for (int l = last_index; l >= 0;l--) {
		vector<Neuron*> layer = layers[l];
		layer.clear();
	}
	delete this->loss_func;
}