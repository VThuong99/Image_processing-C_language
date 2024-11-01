#include "sobel.h"
#include <math.h>

// Kernel Sobel
int Gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

int Gy[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};

void sobel_filter(int width, int height, unsigned char input[height][width], unsigned char output[height][width]) {
    int gx, gy, g;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            gx = 0;
            gy = 0;

            // Nhan chap Kernel voi anh
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    gx += Gx[i + 1][j + 1] * input[y + i][x + j];
                    gy += Gy[i + 1][j + 1] * input[y + i][x + j];
                }
            }

            // Tinh do lon
            g = (int) sqrt(gx * gx + gy * gy);
            if (g > 255) g = 255;
            if (g < 0) g = 0;

            output[y][x] = (unsigned char) g;
        }
    }
}
