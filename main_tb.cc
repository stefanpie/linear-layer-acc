#include "model.h"


int main() {
    F_TYPE input[INPUT_SIZE];
    F_TYPE weight[OUTPUT_SIZE][INPUT_SIZE];
    F_TYPE output[OUTPUT_SIZE];
    F_TYPE golden_output[OUTPUT_SIZE];

    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = random() / (F_TYPE)RAND_MAX;
        PRINT(printf("input[%d] = %f\n", i, input[i]));
    }

    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            weight[i][j] = random() / (F_TYPE)RAND_MAX;
            PRINT(printf("weight[%d][%d] = %f\n", i, j, weight[i][j]));
        }
    }

    // compute golden output
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        golden_output[i] = 0;
        for (int j = 0; j < INPUT_SIZE; j++) {
            golden_output[i] += weight[i][j] * input[j];
        }
    }

    linear_model(input, output, weight);

    int correct = 1;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        float diff = abs(output[i] - golden_output[i]);
        if (diff > 0.001) {
            correct = 0;
            printf("output[%d] = %f, golden_output[%d] = %f, diff = %f\n", i, (float)output[i], i, (float)golden_output[i], diff);
        }
    }

    if (correct) {
        std::cout << "Model Correct" << std::endl;
    }else{
        std::cout << "Model Wrong" << std::endl;
    }
    return 0;
}