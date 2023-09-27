#include "ILossFunction.h"
double ILossFunction::getError(function<double(int)> get_output, vector<double> desired_outputs) {
	double error = 0;
	for (int i = 0; i < desired_outputs.size();i++) {
		double output = get_output(i);
		double desired = desired_outputs[i];
		double iter_error = getIterError(output,desired);
		processIterError(iter_error, error);
	}
	return error;
}
double ILossFunction::calculateTotalError(vector<double> outputs, vector<double> desireds) {
	int p = 0;
	auto r = [&outputs](int index) {
		return outputs[index];
	};
	double error = getError(r,desireds);
	return error;
}
double ILossFunction::calculateTotalError(function<double(int)> get_output,vector<double> desired_outputs){
	return getError(get_output, desired_outputs);
}