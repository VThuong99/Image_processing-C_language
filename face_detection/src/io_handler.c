#include <stdio.h>
#include "io_handler.h"

void read_pgm(const char *filename, int img[IMG_SIZE][IMG_SIZE]) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }
    
    // Đọc header PGM
    char format[3];
    fscanf(file, "%s\n", format);
    int width, height, maxval;
    fscanf(file, "%d %d\n%d\n", &width, &height, &maxval);
    
    // Đọc dữ liệu pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            img[i][j] = fgetc(file);
        }
    }
    fclose(file);
}

void save_feature_to_file(int vector[], int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", vector[i]);
    }
    fclose(file);
}
