#include "Sigmoida.h"
#include <math.h>
double Sigmoida::calculate(double value)
{
	return 1 / (1 + exp(-value));
}
double Sigmoida::differentiate(double value) {
	double s = calculate(value);
	double diff = s*(1-s);
	return diff;
}