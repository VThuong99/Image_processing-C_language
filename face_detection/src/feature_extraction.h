#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H

#define IMG_SIZE 64
#ifndef VECTOR_SIZE
#define VECTOR_SIZE IMG_SIZE
#endif

void extract_feature_vector(int img[IMG_SIZE][IMG_SIZE], int vector[VECTOR_SIZE]);

#endif
