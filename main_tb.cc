#include "model.h"

// inputs
float input_float[INPUT_SIZE];
F_TYPE input_fixed[INPUT_SIZE];

// weights
float weight_float[OUTPUT_SIZE][INPUT_SIZE];
F_TYPE weights_fixed[OUTPUT_SIZE][INPUT_SIZE];

// bias
float bias_float[OUTPUT_SIZE];
F_TYPE bias_fixed[OUTPUT_SIZE];

// computed outputs
float output_float[OUTPUT_SIZE];
F_TYPE output_fixed[OUTPUT_SIZE];

// expected outputs
float golden_output_float[OUTPUT_SIZE];

int main() {

    std::srand(0);
    

    printf("INPUT_SIZE: %d\n", INPUT_SIZE);
    printf("OUTPUT_SIZE: %d\n", OUTPUT_SIZE);

    // inputs
    for (int i = 0; i < INPUT_SIZE; i++) {
        input_float[i] = (std::rand() / (float)RAND_MAX)*2-1;
    }
    for (int i = 0; i < INPUT_SIZE; i++) {
        input_fixed[i] = (F_TYPE)input_float[i];
    }

    // weights
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            weight_float[i][j] = (std::rand() / (float)RAND_MAX) * 2 - 1;
        }
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            weights_fixed[i][j] = (F_TYPE)weight_float[i][j];
        }
    }

    // bias
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        bias_float[i] = (std::rand() / (float)RAND_MAX) * 2 - 1;
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        bias_fixed[i] = (F_TYPE)bias_float[i];
    }

    // golden output
    float sum;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        sum = bias_float[i];
        for (int j = 0; j < INPUT_SIZE; j++) {
            sum += weight_float[i][j] * input_float[j];
        }
        golden_output_float[i] = sum;
    }

    linear_model(input_fixed, output_fixed, weights_fixed, bias_fixed);

    // convert computed output to floating point
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output_float[i] = (float)output_fixed[i];
    }

    int correct = 1;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        float diff = abs(output_float[i] - golden_output_float[i]);
        if (diff > 0.01) {
            correct = 0;
            printf("output[%i] = %f, golden_output[%i] = %f, diff = %f\n", i, output_float[i], i, golden_output_float[i], diff);
        }
    }

    if (correct) {
        std::cout << "Model Correct" << std::endl;
    }else{
        std::cout << "Model Wrong" << std::endl;
    }
    return 0;
}