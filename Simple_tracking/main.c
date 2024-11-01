#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"


void calculate_center(int width, int height, unsigned char *image, unsigned char *background, int *center_x, int *center_y) {
    int sum_x = 0, sum_y = 0, count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            if (image[index] != background[index]) { // Pixel khong phai nen la object
                sum_x += x;
                sum_y += y;
                count++;
            }
        }
    }
    if (count > 0) {
        *center_x = sum_x / count;
        *center_y = sum_y / count;
    } else {
        *center_x = -1;
        *center_y = -1;
    }
}

int main() {
    int width, height;
    unsigned char *background, *image1, *image2;

    read_pgm("data/bg.pgm", &width, &height, &background);
    read_pgm("data/fig1.pgm", &width, &height, &image1);
    read_pgm("data/fig2.pgm", &width, &height, &image2);

    int center_x1, center_y1, center_x2, center_y2;
    calculate_center(width, height, image1, background, &center_x1, &center_y1);
    calculate_center(width, height, image2, background, &center_x2, &center_y2);

    int delta_x = center_x2 - center_x1;
    int delta_y = center_y2 - center_y1;
    double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
        
    printf("Object move %.2f pixel.\n", distance);
    printf("Direction: ");
        
    if (delta_y < 0) printf("Up ");
    if (delta_y > 0) printf("Down ");
    if (delta_x > 0) printf("Right ");
    if (delta_x < 0) printf("Left ");
        
    printf("\n");

    return 0;
}
