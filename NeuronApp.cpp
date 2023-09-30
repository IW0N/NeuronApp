#include <iostream>
#include "ReLU.h"
#include "MSE.h"
#include "Sigmoida.h"
#include "NeuronNet.h"

vector<double> generateTrainInput() 
{
    vector<double> train_input;
    for (int i = 0; i < 3;i++) 
    {
        int color = rand()%256;
        
        double norm_color = color/255.0;
        train_input.push_back(norm_color);
    }
    return train_input;
}
vector <double> generateTrainOutput(vector<double> input) 
{
    vector<double> outputs;
    for (int i = 0; i < 3;i++) {
        double color = 1.0-input[i];
        outputs.push_back(color);
    }
    return outputs;
}
vector<TrainPair>* generateNegativeDataSet(int dataSet_size) 
{
    vector<TrainPair>* dataSet=new vector<TrainPair>;
    for (int i = 0; i < dataSet_size;i++) 
    {
        vector<double> input=generateTrainInput();
        vector<double> output = generateTrainOutput(input);
        TrainPair train(input,output);
        dataSet->push_back(train);
    }
    return dataSet;
}
vector<TrainPair>* generateXORDataSet() {
    vector<TrainPair>* dataSet = new vector<TrainPair>;
    bool a=false, b=false;
    for (int i = 0; i < 4; i++) {
        a = i>1;
        b = i%2;
        bool c = a^b;
        vector<double> input{ (double)a,(double)b };
        vector<double> output{ (double)c };
        TrainPair pair(input, output);
        dataSet->push_back(pair);
    }
    return dataSet;
    
}

int main()
{
    setlocale(LC_ALL,"ru-Ru");
    auto dataSet = generateNegativeDataSet(10000);
    const int output_count = 3;
    vector<int> layer_counts = {3,9,output_count};
    IFuncActivation* activator=new Sigmoida;
    ILossFunction* lossFunc = new MSE(output_count);
    NeuronNet nn(layer_counts, activator,lossFunc);
    nn.setBias(0);

    EOTWP params;
    params.dataSet = dataSet;
    params.min_error = 0.007;
    params.train_speed = 0.007;
    cout << endl<<"»дЄт обучение..."<<endl;
    nn.train(dataSet,30,0.007);
    
    cout <<"ќбучение завершено!"<<endl;
 


    //delete dataSet;

    vector<string> fields{"red: ","green: ", "blue: "};
    vector<double> colors{0,0,0};
    bool break_cycle = false;
    while (!break_cycle) {
        cout << "------Input colors------" << endl;
        for (int i = 0; i < 3; i++)
        {
            string field = fields[i];
            cout << field;
            int color;
            cin >> color;
            if (color == -1) {
                break_cycle = true;
                break;
            }
            colors[i] = color/255.0;
        }
        if (break_cycle)
            break;
        vector<double> output = nn.getData(colors);
        cout << "-----Result-----"<<endl;
        for (int i = 0; i < 3; i++) {
            double result = output[i] * 255.0;
            cout << fields[i]<<result<<endl;
        }
        cout << endl << endl;


    }
    /*cout << "»дЄт подсчЄт средней ошибки..." << endl;
    const int test_count = 600000;
    double totalError = 0;
    auto testSet = generateNegativeDataSet(test_count);
    for (TrainPair pair:testSet) {
        auto output=nn.getData(pair.input);
        double error=nn.getTotalError(pair.output);
        totalError += error;
    }
    double average = totalError/test_count;
    cout << "—редн€€ ошибка: " << average;*/
    delete activator;
    return 0;
}