#ifndef __MODEL_H__
#define __MODEL_H__

#define __FLOATING_POINT_MODEL__ 1
#define __PRINT__ 0

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#if __FLOATING_POINT_MODEL__
    #pragma message("Floating point model")
    #include <cmath>
typedef float F_TYPE;
typedef float W_TYPE;
    #define abs(x) std::abs(x)
#else
    #pragma message("Fixed point model")
    #include "ap_fixed.h"
    #include "hls_math.h"
    #define FIXED_TYPE_F ap_fixed<32, 10>
    #define FIXED_TYPE_W ap_fixed<32, 10>
typedef FIXED_TYPE_F F_TYPE;
typedef FIXED_TYPE_W W_TYPE;
    #define abs(x) hls::abs(x)
#endif

#if __PRINT__
    #define PRINT(x) x
#else
    #define PRINT(x)
#endif

#define INPUT_SIZE 12544
#define OUTPUT_SIZE 1024

void linear_model(F_TYPE input[INPUT_SIZE],
                  F_TYPE output[OUTPUT_SIZE],
                  F_TYPE weight[OUTPUT_SIZE][INPUT_SIZE],
                  W_TYPE bias[OUTPUT_SIZE]);

#endif
