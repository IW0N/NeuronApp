#include <iostream>
#include "ReLU.h"

#include "NeuronNet.h"
int main()
{
    vector<int> layer_counts = {3,4,3};
    ReLU* activator=new ReLU;
    NeuronNet* n=new NeuronNet(layer_counts, activator);
    double *input = new double[3]{ .1, .2, .3 };
    double output_size;
    double* data=n->getData(input, output_size);
    for (int i = 0; i < output_size;i++) 
    {
        cout << data[i]<<' ';
    }
    cout << endl;
    delete[3] input;
    delete[3] data;
    delete activator;
    delete n;
    return 0;
}

