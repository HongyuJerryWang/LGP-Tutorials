#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int NUM_INPUTS = 2;
static int NUM_OUTPUTS = 1;

void gp(double r[14]) {

    r[5] = r[0] == 0.0 ? 1000000.0 : r[8] / r[0];
    // r[11] = pow(r[2], r[6]);
    r[7] = r[5] == 0.0 ? 1000000.0 : r[3] / r[5];
    r[9] = r[12] * r[7];
    // r[4] = pow(r[1], r[11]);
    r[2] = r[1] + r[9];
    // r[4] = r[1] * r[13];
    // r[9] = r[8] * r[13];
    // r[3] = r[9] == 0.0 ? 1000000.0 : r[12] / r[9];
    // r[6] = r[12] + r[0];
    // r[8] = r[6] * r[2];
    // r[6] = r[7] + r[7];
    // r[3] = r[7] * r[1];
    // r[9] = r[10] == 0.0 ? 1000000.0 : r[12] / r[10];
    // r[4] = pow(r[3], r[9]);
    // r[11] = r[13] - r[2];
    r[0] = pow(r[0], r[2]);
    // r[9] = pow(r[12], r[11]);
    // r[8] = r[9] + r[3];
    // r[7] = pow(r[6], r[3]);
    // r[4] = r[7] * r[8];
    // r[8] = r[11] * r[0];
    // r[5] = r[8] - r[11];
    // r[10] = r[1] + r[4];
    // r[2] = r[10] - r[10];
    // r[10] = pow(r[5], r[2]);
    // r[11] = r[0] - r[10];
    // r[2] = r[11] + r[12];
    // r[2] = r[12] == 0.0 ? 1000000.0 : r[0] / r[12];
    
}


char *classify(double values[], char *categories[], int numCategories) {
    int classification = 0;
    double value = values[0];
    for (int i = 1; i < numCategories; i++)
        if (values[i] > value) {
            classification = i;
            value = values[i];
        }
    return categories[classification];
}

int main(int argc, char *argv[]) {

    if (argc != NUM_INPUTS + 1) {
        printf("Please specify %d input value(s)...\n", NUM_INPUTS);

        exit(1);
    }

    double r[14] = {
        0.0, // input
        0.0, // input
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        1.0, // calculation
        0.0, // constant
        1.0, // constant
    };

    if (NUM_INPUTS >= 1) {

        r[0] = strtod(argv[1], NULL);
        r[1] = strtod(argv[2], NULL);

    } else {
      printf("Unexpected number of inputs...\n");

      exit(1);
    }

    gp(r);

    printf("%f\n", r[0]);
    return 0;
}
