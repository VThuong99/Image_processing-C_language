#include "distance.h"
#include <math.h>

double calculate_distance(int vector1[], int vector2[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += pow(vector1[i] - vector2[i], 2);
    }
    return sqrt(sum);
}

double l1_distance(int vector1[], int vector2[], int size) {
    double distance = 0.0;
    for (int i = 0; i < size; i++) {
        distance += fabs(vector1[i] - vector2[i]);
    }
    return distance;
}
