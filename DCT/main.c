#include <stdio.h>
#include <math.h>

#define N 8 //Block size

//Quantization table
int quant_table[N][N] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

void forward_DCT(int input[N][N], double output[N][N]){
    for (int u = 0; u < N; u++){
        for (int v = 0; v < N; v++){
            double sum = 0.0;
            for (int x = 0; x < N; x++){
                for (int y = 0; y < N; y++){
                    sum += input[x][y] *
                        cos((2*x + 1) * u * 3.14 / (2*N)) *
                        cos((2*y + 1) * v * 3.14 / (2*N));
                }
                double C_u = (u==0) ? 1.0/sqrt(2) : 1.0;
                double C_v = (v==0) ? 1.0/sqrt(2) : 1.0;
                output[u][v] = 0.25 * C_u * C_v * sum;
            }
        }
    }
}

// Quantization
void quantize(double input[N][N], int output[N][N], int quant_table[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            output[i][j] = round(input[i][j] / quant_table[i][j]);
        }
    }
}

// De-quantization
void dequantize(int input[N][N], double output[N][N], int quant_table[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output[i][j] = input[i][j] * quant_table[i][j];
        }
    }
}

// Inverse DCT
void inv_DCT(double input[N][N], int output[N][N]){
    for (int x = 0; x < N; x++){
        for (int y = 0; y < N; y++){
            double sum = 0.0;
            for (int u = 0; u < N; u++){
                for (int v = 0; v < N; v++){
                    double C_u = (u==0) ? 1/sqrt(2) : 1.0;
                    double C_v = (v==0) ? 1/sqrt(2) : 1.0;
                    sum += C_u * C_v * input[u][v] *
                        cos((2*x + 1) * u * 3.14 / (2*N)) *
                        cos((2*y + 1) * v * 3.14 / (2*N));
                }
            }
            output[x][y] = round(0.25 * sum);
        }
    }
}

// Print matrix for debugging
void print_matrix(int matrix[N][N], const char* name) {
    printf("%s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrix_double(double matrix[N][N], const char* name) {
    printf("%s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%7.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Input block (8x8) from the source image
    int input[N][N] = {
        {139, 144, 149, 153, 155, 155, 155, 155},
        {144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156},
        {159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155},
        {161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157},
        {162, 162, 161, 161, 163, 158, 158, 158}
    };
    print_matrix(input, "Input matrix");


    // Output buffers
    double dct_output[N][N];
    int quantized_output[N][N];
    double dequantized_output[N][N];
    int reconstructed_output[N][N];

    // Step 1: Forward DCT
    forward_DCT(input, dct_output);
    print_matrix_double(dct_output, "Forward DCT Coefficients");

    // Step 2: Quantization
    quantize(dct_output, quantized_output, quant_table);
    print_matrix(quantized_output, "Quantized Coefficients");

    // Step 3: De-quantization
    dequantize(quantized_output, dequantized_output, quant_table);
    print_matrix_double(dequantized_output, "De-quantized Coefficients");

    // Step 4: Inverse DCT
    inv_DCT(dequantized_output, reconstructed_output);
    print_matrix(reconstructed_output, "Reconstructed Image Samples");

    return 0;
}