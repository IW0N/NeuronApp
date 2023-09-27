#pragma once
#include <vector>
#include<iostream>
#include <functional>
using namespace std;

class ILossFunction
{
private:
	double getError(function<double(int)> get_output, vector<double> desired_output);
protected:
	virtual double getIterError(double output,double desired)=0;
	virtual void processIterError(double iter_error,double &error)=0;
public:
	double calculateTotalError(vector<double> output,vector<double> desired_output);
	double calculateTotalError(function<double(int)> get_output, vector<double> desired_output);
	virtual double differentiate(double output,double desired_answer)=0;
};