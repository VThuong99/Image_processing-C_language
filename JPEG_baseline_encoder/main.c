#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 8

typedef struct {
    int rle;
    int size;
    int amplitude;
}RLECode;


/*
 * Reorder elements in zig-zag
 * new[i] = old[scan_order[i]]
 */
int scan_order[] = {
 0,  1,  8, 16,  9,  2,  3, 10,
17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34,
27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36,
29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46,
53, 60, 61, 54, 47, 55, 62, 63};

// Define DC Huffman codes
const char* dc_codes[] = {
    "00",         // 0
    "010",       // 1
    "011",       // 2
    "100",       // 3
    "101",       // 4
    "110",       // 5
    "1110",      // 6
    "11110",     // 7
    "111110",    // 8
    "1111110",   // 9
    "11111110",  // 10
    "111111110"  // 11
};

// Define AC Huffman codes
const char* ac_codes[256][2] = {
    // Run/Size, Code Length, Code Word
    {"0/0", "1010"},
    {"0/1", "00"},
    {"0/2", "01"},
    {"0/3", "100"},
    {"0/4", "1011"},
    {"0/5", "11010"},
    {"0/6", "1111000"},
    {"0/7", "11111000"},
    {"0/8", "1111110110"},
    {"0/9", "1111111110000010"},
    {"0/A", "1111111110000011"},
    {"1/1", "1100"},
    {"1/2", "11011"},
    {"1/3", "1111001"},
    {"1/4", "111110110"},
    {"1/5", "11111110110"},
    {"1/6", "1111111110000100"},
    {"1/7", "1111111110000101"},
    {"1/8", "1111111110000110"},
    {"1/9", "1111111110000111"},
    {"2/1", "11100"},
    {"2/2", "11111001"},
    {"2/3", "1111110111"},
    {"2/4", "111111110100"},
    {"2/5", "1111111110001001"},
    {"2/6", "1111111110001010"},
    {"2/7", "1111111110001011"},
    {"2/8", "1111111110001100"},
    {"2/9", "1111111110001101"},
    {"3/1", "111010"},
    {"3/2", "111110111"},
    {"3/3", "111111110101"},
    {"3/4", "1111111110001111"},
    {"3/5", "1111111110010000"},
    {"3/6", "1111111110010001"},
    {"3/7", "1111111110010010"},
    {"3/8", "1111111110010011"},
};

// Function to get AC code based on run and size
const char* get_ac_code(int run, int size) {
    if (run > 3) return NULL;  // For example
    if (size > 15) return NULL; // For example
    char key[5];
    sprintf(key, "%d/%d", run, size);
    for (int i = 0; i < sizeof(ac_codes) / sizeof(ac_codes[0]); i++) {
        if (strcmp(ac_codes[i][0], key) == 0) {
            return ac_codes[i][1];
        }
    }
    return NULL;
}

/*
 * Reorders the dct coefficients in zig-zag order using the scan_order matrix
 */
void zigzag_scan(int matrix[N][N], int *output) {
    int zigzag_index[N][N] = {
        {0, 1, 5, 6, 14, 15, 27, 28},
        {2, 4, 7, 13, 16, 26, 29, 42},
        {3, 8, 12, 17, 25, 30, 41, 43},
        {9, 11, 18, 24, 31, 40, 44, 53},
        {10, 19, 23, 32, 39, 45, 52, 54},
        {20, 22, 33, 38, 46, 51, 55, 60},
        {21, 34, 37, 47, 50, 56, 59, 61},
        {35, 36, 48, 49, 57, 58, 62, 63}
    };
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output[zigzag_index[i][j]] = matrix[i][j];
        }
    }
}
int calculate_size(int value) {
    if (value == 0) return 0;
    return (int)(log2(abs(value)) + 1);
}
char* int_to_baseline_binary(int value) {
    char* binary = (char*)malloc(20 * sizeof(char));
    memset(binary, 0, 20); 
    int abs_value = abs(value); 

    // Find the SIZE
    int size = 0;
    if (abs_value == 0) {
        return "0"; 
    } else if (abs_value == 1) {
        size = 1;
    } else if (abs_value <= 3) {
        size = 2;
    } else if (abs_value <= 7) {
        size = 3;
    } else if (abs_value <= 15) {
        size = 4;
    } else if (abs_value <= 31) {
        size = 5;
    } else if (abs_value <= 63) {
        size = 6;
    } else if (abs_value <= 127) {
        size = 7;
    } else if (abs_value <= 255) {
        size = 8;
    } else if (abs_value <= 511) {
        size = 9;
    } else if (abs_value <= 1023) {
        size = 10;
    }

    // Econde SIZE to binary
    for (int i = size - 1; i >= 0; i--) {
        binary[size - 1 - i] = (abs_value & (1 << i)) ? '1' : '0';
    }

    if (value < 0) {
        for (int i = 0; i < size; i++) {
            binary[i] = (binary[i] == '1') ? '0' : '1';
        }
    }

    return binary; 
}
int encode_rle(int *vector, int size, RLECode *output) {
    int runLength = 0;
    int outputIndex = 0;

    // DC encode
    int dc_value = vector[0];
    output[outputIndex].rle = 0;
    output[outputIndex].size = calculate_size(dc_value);
    output[outputIndex].amplitude = dc_value;
    outputIndex++;

    // AC encode
    for (int i = 1; i < size; i++) {  
        if (vector[i] == 0) {
            runLength++;
        } else {
            // Save (RunLength, Size)(Amplitude)
            output[outputIndex].rle = runLength;
            output[outputIndex].size = calculate_size(vector[i]);
            output[outputIndex].amplitude = vector[i];
            outputIndex++;

            runLength = 0; 
        }
    }

    // Endpoint (0,0)
    output[outputIndex].rle = 0;
    output[outputIndex].size = 0;
    output[outputIndex].amplitude = 0;

    return outputIndex + 1; 
}

// Function to convert RLE output to bit string
char* huffman_encode(RLECode* rle_output, int count) {
    char* result = malloc(1024); 
    result[0] = '\0'; 

    // Encode DC coefficient
    int dc_diff = rle_output[0].amplitude; 
    int dc_category = abs(dc_diff); 
    int dc_code_index = 0;

    if (dc_category == 0) dc_code_index = 0;
    else if (dc_category < 3) dc_code_index = 1;
    else if (dc_category < 7) dc_code_index = 2;
    else if (dc_category < 15) dc_code_index = 3;
    else if (dc_category < 31) dc_code_index = 4;
    else if (dc_category < 63) dc_code_index = 5;
    else if (dc_category < 127) dc_code_index = 6;
    else if (dc_category < 255) dc_code_index = 7;
    else if (dc_category < 511) dc_code_index = 8;
    else if (dc_category < 1023) dc_code_index = 9;
    else if (dc_category < 2047) dc_code_index = 10;
    else if (dc_category < 4095) dc_code_index = 11;

    strcat(result, dc_codes[dc_code_index]); // Add DC code to result
    strcat(result, int_to_baseline_binary(dc_category));

    // Encode AC coefficients
    for (int i = 1; i < count; i++) {
        int run = rle_output[i].rle;
        int size = rle_output[i].size;
        int amplitude = rle_output[i].amplitude;

        if (run == 0 && size == 0) {
            strcat(result, "1010");
            break; // End of block
        }

        // Get AC code
        const char* ac_code = get_ac_code(run, size);
        if (ac_code) {
            strcat(result, ac_code); // Add AC code to result

            // Encode amplitude
            // Amplitude converted to a binary string
            char* amplitude_bits; 
            amplitude_bits = int_to_baseline_binary(amplitude);

            strcat(result, amplitude_bits); 
        }
    }

    return result;
}

int decode_dc_category(const char* encoded_bits, int* bit_pos) {
    for (int i = 0; i < 12; i++) {
        int length = strlen(dc_codes[i]);
        if (strncmp(&encoded_bits[*bit_pos], dc_codes[i], length) == 0) {
            *bit_pos += length;
            return i;
        }
    }
    return -1; 
}

int decode_amplitude(const char* encoded_bits, int size, int* bit_pos) {
    if (size == 0) return 0; 

    int amplitude = 0;
    for (int i = 0; i < size; i++) {
        amplitude = (amplitude << 1) | (encoded_bits[*bit_pos + i] - '0');
    }
    *bit_pos += size;

    if (amplitude < (1 << (size - 1))) {
        amplitude -= (1 << size) - 1;
    }
    return amplitude;
}

const char* decode_ac_code(const char* encoded_bits, int* run, int* size, int* bit_pos) {
    for (int i = 0; i < sizeof(ac_codes) / sizeof(ac_codes[0]); i++) {
        int length = strlen(ac_codes[i][1]);
        if (strncmp(&encoded_bits[*bit_pos], ac_codes[i][1], length) == 0) {
            *bit_pos += length;
            sscanf(ac_codes[i][0], "%d/%d", run, size);
            return ac_codes[i][1];
        }
    }
    return NULL;  
}

void huffman_decode(const char* encoded_bits, int* output) {
    int bit_pos = 0;
    
    int dc_category = decode_dc_category(encoded_bits, &bit_pos);
    int dc_amplitude = decode_amplitude(encoded_bits, dc_category, &bit_pos);
    output[0] = dc_amplitude;

    int idx = 1;
    while (idx < 64) {
        int run, size;
        if (strncmp(&encoded_bits[bit_pos], "1010", 4) == 0) { 
            bit_pos += 4;
            break;
        }

        decode_ac_code(encoded_bits, &run, &size, &bit_pos);
        int amplitude = decode_amplitude(encoded_bits, size, &bit_pos);

        while (run-- > 0) {
            output[idx++] = 0;
        }
        output[idx++] = amplitude;
    }

    while (idx < 64) {
        output[idx++] = 0;
    }
}
void inverse_zigzag(int* input, int matrix[8][8]) {
    int zigzag_index[8][8] = {
        {0, 1, 5, 6, 14, 15, 27, 28},
        {2, 4, 7, 13, 16, 26, 29, 42},
        {3, 8, 12, 17, 25, 30, 41, 43},
        {9, 11, 18, 24, 31, 40, 44, 53},
        {10, 19, 23, 32, 39, 45, 52, 54},
        {20, 22, 33, 38, 46, 51, 55, 60},
        {21, 34, 37, 47, 50, 56, 59, 61},
        {35, 36, 48, 49, 57, 58, 62, 63}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix[i][j] = input[zigzag_index[i][j]];
        }
    }
}
void print_vector(int* vector, int size) {
    printf("Vector: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}
void print_array(int arr[][8], int rows) {
    printf("Array:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}
void print_rle(RLECode *output, int count) {
    printf("Intermediate sequence: \n");
    printf("(%d)(%d) ",output[0].size, output[0].amplitude);
    for (int i = 1; i < count; i++) {
        if (output[i].size == 0 && output[i].rle == 0) {
            printf("(0,0)\n");  
        } else {
            printf("(%d,%d)(%d) ", output[i].rle, output[i].size, output[i].amplitude);
        }
    }
}
void print_matrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // 8x8 matrix after DCT and quantized
    int matrix[N][N] = {
        {3, 0, -1, 0, 0, 0, 0, 0},
        {-2, -1, 0, 0, 0, 0, 0, 0},
        {-1, -1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    printf("Quantized matrix: \n");
    print_matrix(matrix);
    int freq[64];
    RLECode* output = malloc(64 * sizeof(RLECode)); // Allocate memory for RLE output
    zigzag_scan(matrix, freq);
    int count = encode_rle(freq, 64, output);
    print_rle(output, count);

    // Perform Huffman encoding
    char* encoded_bits = huffman_encode(output, count);

    printf("Encoded bits: %s\n", encoded_bits);
     // Decode Huffman and RLE to get back the frequency array
    int decoded_freq[64];
    huffman_decode(encoded_bits, decoded_freq);

    // Reconstruct the matrix from the zig-zag decoded frequency array
    int decoded_matrix[N][N];
    inverse_zigzag(decoded_freq, decoded_matrix);

    // Print the decoded matrix
    printf("Decoded Matrix:\n");
    print_matrix(decoded_matrix);


    free(encoded_bits); // Free allocated memory for encoded bits
    free(output); // Free allocated memory for RLE output
}