#include "image.h"
#include <stdio.h>
#include <stdlib.h>

void read_pgm(const char *filename, int *width, int *height, unsigned char **image) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Cannot open file");
        exit(1);
    }

    // Doc header PGM (P5)
    fscanf(file, "P5\n%d %d\n255\n", width, height);

    // Cap phat bo nho cho anh
    *image = (unsigned char *)malloc((*width) * (*height));

    // Doc du lieu hinh anh
    fread(*image, sizeof(unsigned char), (*width) * (*height), file);
    fclose(file);
}

void write_pgm(const char *filename, int width, int height, unsigned char **image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Cannot open file");
        exit(1);
    }

    // Ghi header PGM
    fprintf(file, "P5\n%d %d\n255\n", width, height);

    // Ghi du lieu hinh anh
    fwrite(*image, sizeof(unsigned char), width * height, file);
    fclose(file);
}
