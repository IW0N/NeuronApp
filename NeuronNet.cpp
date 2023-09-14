#pragma once
#include "NeuronNet.h"
using namespace std;

//private
double** NeuronNet::generateRandomLayerWeights(int input_count, int output_count)
{
	srand(time(NULL));
	double** weight_layer = new double* [output_count];
	for (int y = 0; y < output_count; y++)
	{
		double* weight_arr = new double[input_count];
		for (int x = 0; x < input_count; x++)
			weight_arr[x] = rand();
		weight_layer[y] = weight_arr;
	}
	return weight_layer;
}
int* NeuronNet::copyData(vector<int> _layer_sizes) {
	int count = _layer_sizes.size();
	int* sizes = new int[count];
	for (int i = 0; i < count; i++) {
		sizes[i] = _layer_sizes[i];
	}
	return sizes;
}

void NeuronNet::initInputVector(double* data) {
	double* input_vector = neuron_vectors_norm[0];
	int input_count = layer_sizes[0];
	for (int y = 0; y < input_count; y++) {
		input_vector[y] = data[y];
	}
}
void NeuronNet::sumVector(int layer)
{
	int m = layer_sizes[layer - 1];
	int n = layer_sizes[layer];

	//Здесь -1, поскольку layer-это индекс слоя нейронов, а индекс слоя весов сдвинут на 
	//единицу назад относительного слоя нейронов
	double** weight_matrix = weights[layer - 1];
	//А здесь единицу отнимаю, поскольку беру входящий слой, а не выходящий(его индекс=layer)
	double* input_vector = neuron_vectors_norm[layer - 1];
	double* output_vector = neuron_vectors_sum[layer - 1];
	for (int y = 0; y < n; y++)
	{
		double* row = weight_matrix[y];
		output_vector[y] = 0;
		for (int x = 0; x < m; x++)
		{
			double summand = row[x] * input_vector[x];
			output_vector[y] += summand;
		}
	}

}
void NeuronNet:: normalizeVector(int layer)
{
	double* sum_vector = neuron_vectors_sum[layer-1];
	double* norm_vector = neuron_vectors_norm[layer];
	int count = layer_sizes[layer];
	for (int y = 0; y < count; y++) {
		norm_vector[y] = func->calculate(sum_vector[y]);
	}

}
void NeuronNet::updateVector(int layer)
{
	sumVector(layer);
	normalizeVector(layer);
}

double NeuronNet::getTotalError(double* desiredAnswer) {
	int last_index = layers_count - 1;
	int last_count =  layer_sizes[last_index];
	double* output = neuron_vectors_norm[last_index];
	double error = 0;
	for (int y = 0; y < last_count;y++) 
	{
		double delta = output[y]-desiredAnswer[y];
		double summand = pow(delta,2);
		error += summand;
	}
	return error;
}
double NeuronNet::getDifferentialForLastWeight(double* desired_vector,int in_index,int out_index) 
{
	int nnli = layers_count - 1;//normalized neuron last index
	int wli = nnli-1;//weights last index
	int snli = wli;//summand neurons last index
	int last_neuron_layer_size = layer_sizes[nnli];

	double* prev_layer = neuron_vectors_norm[nnli-1];
	double input = prev_layer[in_index];
	
	double* real_outputs_norm = neuron_vectors_norm[nnli];
	double* real_outputs_sum = neuron_vectors_sum[snli];
	
	double real_value = real_outputs_norm[out_index];
	double real_sum = real_outputs_sum[out_index];
	double desired_value=desired_vector[out_index];

	double result = 2*(real_value-desired_value)*func->differentiate(real_sum)*input;
	return result;
}
void NeuronNet::updateLastWeights(double* desiredAnswer) 
{
	
}


void NeuronNet::disposeWeights()
{
	int constexpr double_size = sizeof(double);
	//wli-weight layer index
	for (int wli = 0; wli < weights_count; wli++)
	{
		double** weight_layer = weights[wli];
		int m = layer_sizes[wli];
		int n = layer_sizes[wli + 1];
		

		for (int y = 0; y < n; y++)
		{
			double* matrix_row = weight_layer[y];
			delete[m] matrix_row;
			int a = 0;
		}
		delete weight_layer;
	}
	delete weights;
}
void NeuronNet::disposeNormNeuronVectors() {
	//nli-neuron layer index
	for (int nli = 0; nli < layers_count; nli++)
	{
		double* neuron_vector = neuron_vectors_norm[nli];
		int layer_count = layer_sizes[nli];
		
		delete[layer_count] neuron_vector;
	}
}
void NeuronNet::disposeSumNeuronVectors() {
	for (int nli = 0; nli < nvs_count; nli++)
	{
		double* neuron_vector = neuron_vectors_sum[nli];
		int layer_count = layer_sizes[nli + 1];
		delete[layer_count] neuron_vector;
	}
}
void NeuronNet::disposeLayerSizes() {
	
	delete[layers_count] layer_sizes;
}

void NeuronNet::printArr(int* arr, const int count) {
	cout << "arr: ";
	for (int i = 0; i < count; i++)
		cout << arr[i]<<" ";
}

//public
NeuronNet::NeuronNet(vector<int> _layer_sizes, IFuncActivation* func)
{
	this->func = func;
	layers_count = _layer_sizes.size();
	this->layer_sizes = this->copyData(_layer_sizes);
	//printArr(layer_sizes,layers_count);

	weights_count = layers_count - 1;
	weights = new double** [weights_count];

	nvs_count = weights_count;
	neuron_vectors_sum = new double* [nvs_count];

	neuron_vectors_norm = new double* [layers_count];

	for (int i = 0; i < layers_count; i++)
	{
		int in_count = layer_sizes[i];
		neuron_vectors_norm[i] = new double[in_count];
		if (i < weights_count) {
			int out_count = layer_sizes[i + 1];
			weights[i] = generateRandomLayerWeights(in_count, out_count);
		}
		if(i>=1)
			neuron_vectors_sum[i-1] = new double[in_count];
	}
		
}
double* NeuronNet::getData(double* input, double& output_size)
{
	initInputVector(input);
	for (int layer = 1; layer < layers_count; layer++)
		updateVector(layer);
	output_size = layer_sizes[layers_count - 1];
	int bytes_count = sizeof(double) * output_size;
	double* norm_last = neuron_vectors_norm[layers_count - 1];
	double* dist = new double[output_size];
	memcpy(dist, norm_last, bytes_count);
	return dist;
};
NeuronNet::~NeuronNet() {
	disposeWeights();
	disposeNormNeuronVectors();
	disposeSumNeuronVectors();
	disposeLayerSizes();
};
