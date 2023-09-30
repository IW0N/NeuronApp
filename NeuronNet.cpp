#pragma once
#include "NeuronNet.h"
#include "InputNeuron.h"
#include "HiddenNeuron.h"
#include "OutputNeuron.h"
#include "EOTWP.h"
#include "BiasNeuron.h"
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
	NeuronWithInputs* casted_output = dynamic_cast<NeuronWithInputs*>(output);
	for (int j = 0; j < prev_layer.size(); j++)
	{
		Neuron* input = prev_layer[j];
		NeuronWithOutputs* casted_input = dynamic_cast<NeuronWithOutputs*>(input);
		casted_output->bindWithInputNeuron(casted_input);
		int a = 2;
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
		InputNeuron* input = dynamic_cast<InputNeuron*>(neuron);
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
		OutputNeuron* outNeuron = dynamic_cast<OutputNeuron*>(lastLayer[i]);
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
		for (Neuron* n:layer) 
		{
			NeuronWithInputs* neuron = dynamic_cast<NeuronWithInputs*>(n);
			neuron->recalculate();
			int a = 2;
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

void NeuronNet::train(vector<TrainPair>* dataSet,int epoches,double train_speed) 
{
	double middle = 0;
	int freq = 10000;
	int total_index = 0;
	for (int epoch = 0; epoch < epoches;epoch++) {
		for (TrainPair pair : *dataSet) {
			learnTrainData(pair, train_speed);
			total_index++;
			if (total_index % freq == 0) {
				TrainPair pair = (*dataSet)[1];
				vector<double> predicted = pair.output;
				double error = getTotalError(predicted);
				cout << error << endl;
			}
		}
		
	}
}
void NeuronNet::train(EOTWP parametres)
{
	vector<TrainPair>* dataSet = parametres.dataSet;
	int max_iters = parametres.max_iterations;
	int max_epochs = max_iters/dataSet->size();
	int iter = 0;
	vector<double> sample_prediction = (*dataSet)[parametres.sample_index].output;
	bool stop = false;
	for (int epoch = 0; epoch < max_epochs; epoch++)
	{
		for (TrainPair pair:*dataSet)
		{
			if (iter < parametres.max_iterations)
			{
				learnTrainData(pair, parametres.train_speed);
				if (iter % parametres.check_frequency == 0) 
				{
					double error = getTotalError(sample_prediction);
					cout << error << endl;
					if (error < parametres.min_error) {
						stop = true;
						break;
					}
				}
			}
			else
				break;
			iter++;
		}
		if (stop)
			break;
		
	}
}
void NeuronNet::setBias(int layer_index)
{
	if (layer_index == layers.size() - 1)
		throw "Нельзя добавить нейрон смещения в выходной слой!";
	vector<Neuron*> bindable_layer = layers[layer_index+1];
	BiasNeuron* bias = new BiasNeuron(bindable_layer);
	biases.push_back(bias);
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
			NeuronWithInputs* withInputs = dynamic_cast<NeuronWithInputs*>(n);
			withInputs->setInputWeightGradients();
		}
	}
	for (int l = last_index; l >= 1; l--) {
		vector<Neuron*> layer = layers[l];
		for (Neuron* n:layer) {
			NeuronWithInputs* withInputs = dynamic_cast<NeuronWithInputs*>(n);
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
	biases.clear();
	delete this->loss_func;
}