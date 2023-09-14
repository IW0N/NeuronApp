#pragma once
#include<ctime>
#include "IFuncActivation.h"
#include <stdlib.h>
#include<iostream>
#include<vector>
using namespace std;
class NeuronNet {
private:
	int* layer_sizes;
	int layers_count;

	int weights_count;
	double*** weights;
	double** neuron_vectors_norm;
	//кол-во векторов на один меньше, чем у neuron_vectors_norm, т.к не берём в расчёт входящий вектор нейронов
	double** neuron_vectors_sum;
	int nvs_count;//nvs-neuron vectors sum
	IFuncActivation* func;

	//методы для инициализации объекта
	double** generateRandomLayerWeights(int input_count, int output_count);
	int* copyData(vector<int> _layer_sizes);
	
	//методы для расчёта выходного результата
	void initInputVector(double* data);
	void sumVector(int layer);
	void normalizeVector(int layer);
	void updateVector(int layer);

	//методы для тренировки нейронной сети
	double getTotalError(double* desiredAnswer);
	double getDifferentialForLastWeight(double* desired_out,int input_index,int output_index);
	void updateLastWeights(double* desiredAnswer);


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