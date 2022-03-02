#include "model.h"


F_TYPE input_local[INPUT_SIZE];
F_TYPE weight_local[OUTPUT_SIZE][INPUT_SIZE];
F_TYPE bias_local[OUTPUT_SIZE];
F_TYPE output_local[OUTPUT_SIZE];




// function to copy data between 1d array
template <int M, typename T>
void copy_1d(T from[M], T to[M]) {
#pragma HLS INLINE off
#pragma HLS PIPELINE off
    for (int i = 0; i < M; i++) {
        to[i] = from[i];
    }
}

// function to copy data between 2d array
template <int M, int N, typename T>
void copy_2d(T from[M][N], T to[M][N]) {
#pragma HLS INLINE off
#pragma HLS PIPELINE off
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            to[i][j] = from[i][j];
        }
    }
}

// linear layer
template<const int in_size, const int out_size>
void linear(F_TYPE input[in_size],
            F_TYPE output[out_size],
            F_TYPE weight[out_size][in_size],
            F_TYPE bias[out_size]) {
#pragma HLS INLINE off

    const int BLOCK_SIZE_OUT = 8;
    const int BLOCK_SIZE_IN = 8;

#pragma HLS ARRAY_PARTITION variable = input cyclic factor = BLOCK_SIZE_IN dim = 1
#pragma HLS ARRAY_PARTITION variable = output cyclic factor = BLOCK_SIZE_OUT dim = 1

#pragma HLS ARRAY_PARTITION variable = weight cyclic factor = BLOCK_SIZE_OUT dim = 1
#pragma HLS ARRAY_PARTITION variable = weight cyclic factor = BLOCK_SIZE_IN dim = 2

#pragma HLS ARRAY_PARTITION variable = bias cyclic factor = BLOCK_SIZE_OUT dim = 1

    // block parallel linear layer
    // use temp sum
    F_TYPE temp_sum[BLOCK_SIZE_OUT];
    #pragma HLS ARRAY_PARTITION variable = temp_sum complete

    BLOCK_OUT:
    for(int i =0; i<out_size; i+=BLOCK_SIZE_OUT){
        BLOCK_IN:
        for(int j =0; j<in_size; j+=BLOCK_SIZE_IN){

            #pragma HLS PIPELINE
            // zero temp sum
            TEMP_SUM_ZERO_LOOP:
            for(int k =0; k<BLOCK_SIZE_OUT; k++){
                temp_sum[k] = 0;
            }
            // compute temp sum
            SUM_OUTER:
            for(int k =0; k<BLOCK_SIZE_OUT; k++){
                SUM_INNER:
                for(int l =0; l<BLOCK_SIZE_IN; l++){
                    temp_sum[k] += weight[i+k][j+l] * input[j+l];
                }
            }

            // write temp sum to output
            // aslo add bias to output
            WRITE_LOOP:
            for(int k =0; k<BLOCK_SIZE_OUT; k++){
                output[i+k] += temp_sum[k];
                if (j == 0) {
                    output[i+k] += bias[i+k];
                }
            }

        }
    }
    
}

void linear_model(F_TYPE input[INPUT_SIZE],
                  F_TYPE output[OUTPUT_SIZE],
                  F_TYPE weight[OUTPUT_SIZE][INPUT_SIZE],
                  W_TYPE bias[OUTPUT_SIZE]) {
#pragma HLS INTERFACE m_axi port = input offset = slave bundle = b0
#pragma HLS INTERFACE m_axi port = output offset = slave bundle = b1
#pragma HLS INTERFACE m_axi port = weight offset = slave bundle = b2
#pragma HLS INTERFACE m_axi port = bias offset = slave bundle = b3

// assign local memory as 2-port bram
#pragma HLS BIND_STORAGE variable = input_local type = ram_2p impl = bram
#pragma HLS BIND_STORAGE variable = weight_local type = ram_2p impl = bram
#pragma HLS BIND_STORAGE variable = output_local type = ram_2p impl = bram
#pragma HLS BIND_STORAGE variable = bias_local type = ram_2p impl = bram

    // copy input to local memory
    copy_1d<INPUT_SIZE, F_TYPE>(input, input_local);
    // copy weight to local memory
    copy_2d<OUTPUT_SIZE, INPUT_SIZE, F_TYPE>(weight, weight_local);
    // copy bias to local memory
    copy_1d<OUTPUT_SIZE, F_TYPE>(bias, bias_local);

    // linear layer
    linear<INPUT_SIZE, OUTPUT_SIZE>(input_local, output_local, weight_local, bias_local);

    // copy output_local to output
    copy_1d<OUTPUT_SIZE, F_TYPE>(output_local, output);
}
