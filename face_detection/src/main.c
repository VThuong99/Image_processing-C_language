#include <stdio.h>
#include "feature_extraction.h"
#include "io_handler.h"
#include "distance.h"

#define H_THRESHOLD 8000000000 // Threshold
#define L_THRESHOLD 7000000000 

int main() {
    int img[IMG_SIZE][IMG_SIZE];
    int feature_vector[VECTOR_SIZE];

    // Datasets
    const char *image_files[] = {
        "../dataset/1.pgm",
        "../dataset/2.pgm",
        "../dataset/3.pgm",
        "../dataset/4.pgm"
    };

    FILE *output_file = fopen("../dataset/dataset_vectors.txt", "w");
    if (output_file == NULL) {
        perror("Can't open these files.");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        read_pgm(image_files[i], img);

        extract_feature_vector(img, feature_vector);

        fprintf(output_file, "Vector for %s: ", image_files[i]);
        for (int j = 0; j < VECTOR_SIZE; j++) {
            fprintf(output_file, "%d ", feature_vector[j]);
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);
    printf("Vector saved completed -> dataset/dataset_vectors.txt\n");

    return 0;
    // int input_img[IMG_SIZE][IMG_SIZE];
    // int input_vector[VECTOR_SIZE];
    // int dataset_vector[VECTOR_SIZE];

    // read_pgm("../dataset/4.pgm", input_img);
    
    // extract_feature_vector(input_img, input_vector);
    
    // FILE *file = fopen("../dataset/dataset_vectors.txt", "r");
    // if (file == NULL) {
    //     perror("Can't open dataset_vectors.txt");
    //     return 1;
    // }

    // char line[256];
    // int found = 0;

    // int id = 0;
    // for (int i=0; i<64; i++) printf("%d ", input_vector[i]);
    // while (fgets(line, sizeof(line), file)) {
    //     sscanf(line, "Vector for %*s:"); 

    //     for (int i = 0; i < VECTOR_SIZE; i++) {
    //         fscanf(file, "%d", &dataset_vector[i]);
    //     }
    //     for (int i=0; i<64; i++) printf("%d ", dataset_vector[i]);

    //     double distance = l1_distance(input_vector, dataset_vector, 64);
    //     printf("Distance: %f\n", distance);

    //     id ++;
    //     if (distance < H_THRESHOLD && distance > L_THRESHOLD) {
    //         found = 1;
    //         printf("Faced %d\n", id);
    //         break;
    //     }
    // }

    // if (!found) {
    //     printf("Non faced.\n");
    // }

    // fclose(file);
    // return 0;
}
