#ifndef IMAGE_H
#define IMAGE_H

void read_pgm(const char *filename, int *width, int *height, unsigned char **image);
void write_pgm(const char *filename, int width, int height, unsigned char **image);

#endif
