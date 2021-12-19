#include <iostream>
#include <random>
#include <math.h>

float sigmod(float x){
    /*squishes the value to be between 0 and 1*/
    return 1 / (1 + std::exp(-x));
}

float* dotProduct(float a[4][3], float b[4][1]){

    
}

int main(){

    float trainingInput [4][3] = {  {0,0,1},
                                    {1,1,1},
                                    {1,0,1},
                                    {0,1,1}}; 

    float trainingOutput [4][1] = {{0},
                                   {1},
                                   {1},
                                   {0}};

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1,0); 

    float synapticWeights [3][1] = {{2*distribution(generator)},
                                    {2*distribution(generator)},
                                    {2*distribution(generator)}};

    std::cout << "ranodm starting synpatic weights: " << synapticWeights << std::endl;

    for (int i = 0; i < 1; i++){
         float output [4][1];
    }

    return 0;
}

