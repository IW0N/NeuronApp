#pragma once
#include <ctime>
#include "IFuncActivation.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Neuron.h"
#include "TrainPair.h"
#include "ILossFunction.h"
using namespace std;
class NeuronNet {
private:
	ILossFunction* loss_func;
	vector<vector<Neuron*>> layers;
	//инициализация слоёв нейронов
	void initLayers(vector<int> layer_sizes,IFuncActivation* func);
	void bindLayerWithPrevious(vector<Neuron*> layer,int layer_index);
	void bindWithLayer(Neuron* output,vector<Neuron*> previous_layer);
	vector<Neuron*> generateLayer(int layer_size,int layers_count,int layer_index,IFuncActivation* func);
	Neuron* getNeuron(IFuncActivation* activation,int layer_index,int layers_count);
	
	//getData
	void setInputs(vector<double> input);
	void recalculateNeurons();
	vector<double> getOutput();

	//train
	void learnTrainData(TrainPair pair,double train_speed);
	void setPredictedOutputs(vector<double> outputs);
	
public:
	NeuronNet(vector<int> layer_sizes, IFuncActivation* func,ILossFunction* loss);
	vector<double> getData(vector<double> input);
	void train(vector<TrainPair> dataSet,int epoches,double train_speed);
	double getTotalError(vector<double> predicted);
	~NeuronNet();
};
