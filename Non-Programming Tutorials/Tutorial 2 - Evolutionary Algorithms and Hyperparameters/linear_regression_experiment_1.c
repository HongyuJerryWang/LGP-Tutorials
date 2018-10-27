#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int NUM_INPUTS = 2;
static int NUM_OUTPUTS = 1;

void gp(double r[14]) {

    // r[6] = r[6] - r[12];
    // r[4] = r[6] - r[2];
    // r[11] = r[4] - r[13];
    r[0] = r[13] + r[3];
    // r[3] = r[1] - r[3];
    // r[4] = r[11] - r[4];
    // r[10] = r[4] * r[3];
    // r[10] = r[10] * r[13];
    // r[2] = r[13] * r[10];
    // r[11] = r[11] * r[6];
    // r[8] = r[11] + r[5];
    // r[4] = r[2] - r[4];
    // r[5] = r[1] * r[13];
    // r[7] = r[0] - r[4];
    // r[10] = r[12] + r[0];
    // r[9] = r[12] - r[5];
    // r[3] = r[10] - r[9];
    // r[11] = r[5] - r[3];
    // r[2] = r[1] - r[12];
    // r[3] = r[9] + r[2];
    // r[10] = r[7] + r[0];
    // r[10] = r[3] + r[10];
    // r[2] = r[13] - r[10];
    // r[8] = r[12] + r[10];
    // r[10] = r[11] - r[8];
    // r[2] = r[10] - r[2];
    // r[9] = r[12] + r[2];
    
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
