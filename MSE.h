#include "ILossFunction.h"
class MSE :public ILossFunction {
private:
	int output_count;
protected:
	double getIterError(double a, double b) override;
	void processIterError(double iter_error, double& error) override;
public:
	MSE(int output_count);
	double differentiate(double output_value, double answer) override;
};