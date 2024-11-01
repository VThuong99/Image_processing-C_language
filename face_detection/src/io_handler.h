#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "feature_extraction.h"

void read_pgm(const char *filename, int img[IMG_SIZE][IMG_SIZE]);
void save_feature_to_file(int vector[], int size, const char *filename);

#endif
