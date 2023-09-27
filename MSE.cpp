#include "MSE.h"
MSE::MSE(int output_count) {
	this->output_count = output_count;
}
void MSE::processIterError(double e,double &total_error) { total_error += e; }
double MSE::getIterError(double output,double desired) {
	double delta = output-desired;
	double iter_error = delta*delta/output_count;
	return iter_error;
}
double MSE::differentiate(double output_value,double answer) 
{
	return 2.0 / output_count * (output_value - answer);
}