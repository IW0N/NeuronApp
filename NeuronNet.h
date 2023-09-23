#pragma once
#include <ctime>
#include "IFuncActivation.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
class NeuronNet {
private:
	int* layer_sizes;
	int layers_count;

	int weights_count;
	double*** weights;
	double*** weight_gradients;

	double** local_gradients;
	double** neuron_vectors_norm;
	//кол-во векторов на один меньше, чем у neuron_vectors_norm, т.к не берём в расчёт входящий вектор нейронов
	double** neuron_vectors_sum;
	int nvs_count;//nvs-neuron vectors sum
	IFuncActivation* func;

	//методы для инициализации объекта
	double** generateRandomLayerWeights(int input_count, int output_count);
	void initSizes(vector<int> _layer_sizes);
	void initNeuronVectors();
	void initWeights();
	void initWeightGradients();
	void initLocalGradients();
	int* copyData(vector<int> _layer_sizes);
	
	//методы для расчёта выходного результата
	void setInputData(double* data);
	void sumVector(int layer);
	void normalizeVector(int layer);
	void updateVector(int layer);

	//методы для тренировки нейронной сети
	double getTotalError(double* desiredAnswer);
	void updateLastLocalGrads(double* desiredAnswer);
	void updateHiddenLocalGrads(int norm_layer_index);
	void updateLastWeights();
	
	void updateLastGradMatrix(double* desired_out);
	void updateHiddenGradMatrix(int weight_layer_index);
	void updateTotalHiddenWeights();

	void updateWeights(int weights_layer_index,double train_speed);

	//методы высвобождения нейронной сети из оперативной памяти компьютера
	void disposeWeights();
	void disposeNormNeuronVectors();
	void disposeSumNeuronVectors();
	void disposeLayerSizes();
	
	//вспомогательные методы
	void printArr(int* arr, const int count);

public:
	NeuronNet(vector<int> _layer_sizes, IFuncActivation* func);
	double* getData(double* input, double& output_size);
	~NeuronNet();
};