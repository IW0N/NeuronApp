#include "ReLU.h"
double ReLU::calculate(double value) {
	return value < 0 ? 0 : value;
};
double ReLU::differentiate(double value) {
	return value < 0 ? 0 : 1;
}