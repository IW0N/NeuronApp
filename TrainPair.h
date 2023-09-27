#pragma once
#include <vector>
using namespace std;
struct TrainPair
{
	vector<double> input;
	vector<double> output;
	TrainPair(vector<double> input, vector<double> output);
};