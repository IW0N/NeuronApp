#include <stdint.h>
#include "TrainPair.h"
#include <vector>
#pragma once
//Error-Orientation Train Way Parametres
class EOTWP
{
public:
	int max_iterations = INT32_MAX;
	vector<TrainPair>* dataSet;
	double train_speed;
	double min_error;
	int check_frequency = 10000;
	//index, that will sample for measuring error in each epoch
	//default value is 0
	int sample_index=0;
	~EOTWP() {
		dataSet->clear();
		delete dataSet;
	}
};

