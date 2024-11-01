#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "sobel.h"

int main() {
    unsigned char *input_image, *output_image;
    int width, height;
    read_pgm("lena.pgm", &width, &height, &input_image);

    output_image = (unsigned char *)malloc(width * height);

    sobel_filter(width, height, (unsigned char(*)[width])input_image, (unsigned char(*)[width])output_image);
    
    write_pgm("output_image.pgm", width, height, &output_image);

}
