#include "feature_extraction.h"

int kernel_v[5][5] = {{0,0,0,0,0},{1,1,1,1,1},{0,0,0,0,0},{-1,-1,-1,-1,-1},{0,0,0,0,0}};
int kernel_h[5][5] = {{0,1,0,-1,0},{0,1,0,-1,0},{0,1,0,-1,0},{0,1,0,-1,0},{0,1,0,-1,0}};
int kernel_p[5][5] = {{0,0,0,1,0},{0,1,1,0,-1},{0,1,0,-1,0},{1,0,-1,-1,0},{0,-1,0,0,0}};
int kernel_m[5][5] = {{0,-1,0,0,0},{1,0,-1,-1,0},{0,1,0,-1,0},{0,1,1,0,-1},{0,0,0,1,0}};

int max(int a, int b, int c, int d){
    int max_val = a;
    if (b > max_val) max_val = b;
    if (c > max_val) max_val = c;
    if (d > max_val) max_val = d;
    return max_val;
}

void apply_kernel(int input[IMG_SIZE][IMG_SIZE], int output[IMG_SIZE][IMG_SIZE], int kernel[5][5]) {
    for (int i = 1; i < IMG_SIZE-1; i++) {
        for (int j = 1; j < IMG_SIZE-1; j++) {
            int value = 0;
            for (int ki = 0; ki < 5; ki++) {
                for (int kj = 0; kj < 5; kj++) {
                    value += input[i-1+ki][j-1+kj] * kernel[ki][kj];
                }
            }
            output[i][j] = value;
        }
    }
}

void extract_feature_vector(int img[IMG_SIZE][IMG_SIZE], int vector[VECTOR_SIZE]) {
    int edge_v[IMG_SIZE][IMG_SIZE];
    int edge_h[IMG_SIZE][IMG_SIZE];
    int edge_p[IMG_SIZE][IMG_SIZE];
    int edge_m[IMG_SIZE][IMG_SIZE];

    apply_kernel(img, edge_v, kernel_v);
    apply_kernel(img, edge_h, kernel_h);
    apply_kernel(img, edge_p, kernel_p);
    apply_kernel(img, edge_m, kernel_m);

    for (int j = 0; j < IMG_SIZE; j++) {
        int sum = 0;
        for (int i = 0; i < IMG_SIZE; i++) {
            int max_value = max(edge_v[i][j], edge_h[i][j], edge_p[i][j], edge_m[i][j]);
            sum += max_value;
        }
        vector[j] = sum;
    }
}
