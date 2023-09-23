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

void NeuronNet::setInputData(double* data) {
	double* input_vector = neuron_vectors_norm[0];
	int input_count = layer_sizes[0];
	for (int y = 0; y < input_count; y++) {
		input_vector[y] = data[y];
	}
}
void NeuronNet::initSizes(vector<int> _layer_sizes) {
	this->layers_count = _layer_sizes.size();
	this->layer_sizes = copyData(_layer_sizes);
}
void NeuronNet::initNeuronVectors() {
	this->nvs_count = layers_count-1;
	double** layers = new double*[layers_count];
	double** sum_layers = new double*[nvs_count];
	
	for (int l = 0; l < layers_count;l++) 
	{
		int layer_count = layer_sizes[l];
		layers[l] = new double[layer_count];
		if (l >= 1)
			sum_layers[l-1] = new double[layer_count];
	}
	this->neuron_vectors_norm = layers;
	this->neuron_vectors_sum = sum_layers;
}
void NeuronNet::initWeights() 
{
	srand(time(NULL));
	weights_count = layers_count-1;
	double*** weights = new double**[weights_count];
	for (int l=1;l<layers_count;l++)
	{
		int output_count = layer_sizes[l];
		int input_count = layer_sizes[l-1];
		double** matrix = new double*[output_count];
		for (int y = 0; y < output_count;y++) 
		{
			matrix[y] = new double[input_count];
			for (int x = 0; x < input_count;x++) 
				matrix[y][x] = rand();
		}
		weights[l] = matrix;
	}
	this->weights = weights;
}
void NeuronNet::initWeightGradients() {
	double*** gradients = new double** [weights_count];
	for (int l = 1; l < layers_count; l++)
	{
		int output_count = layer_sizes[l];
		int input_count = layer_sizes[l - 1];
		double** matrix = new double* [output_count];
		for (int y = 0; y < output_count; y++)
		{
			matrix[y] = new double[input_count];
		}
		gradients[l-1] = matrix;
	}
	this->weight_gradients = gradients;
}
void NeuronNet::initLocalGradients() {
	local_gradients = new double*[layers_count-1];
	for (int i = 1;i<layers_count;i++) 
	{
		int layer_count = layer_sizes[i];
		double* local_grads = new double[layer_count];
		local_gradients[i - 1] = local_grads;
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
void NeuronNet::normalizeVector(int layer)
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
void NeuronNet::updateLastLocalGrads(double* desired_answer) {
	int lnli = layers_count-1;//lnli-last neuron layer index
	int last_count = layer_sizes[lnli];
	double* outputs = neuron_vectors_norm[lnli - 1];
	double* last_gradients = local_gradients[lnli-2];
	double* last_sums = neuron_vectors_sum[lnli-2];
	
	for (int j = 0; j < last_count;j++)
	{
		double output = outputs[j];
		double desired = desired_answer[j];
		double sum_output = last_sums[j];
		double grad = 2 * (output - desired) * this->func->differentiate(sum_output);
		last_gradients[j] = grad;
	}
}
void NeuronNet::updateLastWeights() {
	int last_count = layer_sizes[layers_count-1];
	int prev_last_count = ;
}
void NeuronNet::updateHiddenLocalGrads(int norm_layer_index)
{
	double* sum_neurons = neuron_vectors_sum[norm_layer_index-1];
	int neurons_count = layer_sizes[norm_layer_index];
	double* grads = local_gradients[norm_layer_index-1];
	for (int i = 0; i < neurons_count;i++)
	{
		double sum = sum_neurons[i];
		double grad = this->func->differentiate(sum);
		grads[i] = grad;
	}
}

void NeuronNet::updateLastGradMatrix(double* desired_answer) 
{
	int last_layer = layers_count-1;

	int input_count=layer_sizes[last_layer - 1];
	int output_count = layer_sizes[last_layer];
	double** last_matrix = weight_gradients[weights_count-1];
	for (int j = 0; j < output_count;j++)
	{
		double* outputs = neuron_vectors_norm[last_layer];
		double* sum_outputs = neuron_vectors_sum[last_layer-1];
		double* inputs = neuron_vectors_norm[last_layer-1];
		for (int i = 0; i < input_count;i++) 
		{
			double sum=sum_outputs[j];
			double real = outputs[j];
			double input = inputs[i];
			double delta = func->differentiate(sum)*(desired_answer[j]-real)*input;
			last_matrix[j][i] = 2*delta;
		}
	}
}
void NeuronNet::updateHiddenGradMatrix(int weights_layer_index) 
{
	double** matrix = this->weights[weights_layer_index];
	int in_layer_index = weights_layer_index;
	int out_layer_index = in_layer_index+1;
	int out_count = layer_sizes[out_layer_index];
	int in_count = layer_sizes[in_layer_index];
	for (int j = 0; j < out_count;j++) 
	{
		for (int i = 0; i < in_count;i++) 
		{

		}
	}
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
	initSizes(_layer_sizes);
	initNeuronVectors();
	initLocalGradients();
	initWeights();
	initWeightGradients();
	this->func = func;
}
double* NeuronNet::getData(double* input, double& output_size)
{
	setInputData(input);
	for (int layer = 1; layer < layers_count; layer++)
		updateVector(layer);
	output_size = layer_sizes[layers_count - 1];
	constexpr double double_size = sizeof(double);
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
