#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int NUM_INPUTS = 2;
static int NUM_OUTPUTS = 1;

void gp(double r[19]) {

    // r[7] = r[16] - r[14];
    // r[5] = r[10] * r[17];
    // r[4] = r[12] - r[2];
    // r[12] = r[17] + r[3];
    // r[15] = r[6] * r[6];
    r[3] = r[10] * r[0];
    r[15] = r[8] - r[17];
    r[4] = r[2] * r[15];
    // r[14] = r[16] * r[7];
    r[2] = r[16] == 0.0 ? 1000000.0 : r[18] / r[16];
    // r[6] = r[2] == 0.0 ? 1000000.0 : r[11] / r[2];
    // r[14] = r[9] * r[17];
    // r[8] = r[14] - r[18];
    // r[13] = r[16] == 0.0 ? 1000000.0 : r[6] / r[16];
    r[0] = r[3] * r[1];
    // r[10] = r[13] * r[17];
    // r[6] = r[11] * r[17];
    r[6] = r[0] == 0.0 ? 1000000.0 : r[18] / r[0];
    r[0] = r[2] == 0.0 ? 1000000.0 : r[6] / r[2];
    // r[14] = r[18] - r[6];
    r[12] = r[4] == 0.0 ? 1000000.0 : r[2] / r[4];
    r[10] = r[12] - r[18];
    // r[12] = r[4] == 0.0 ? 1000000.0 : r[17] / r[4];
    // r[7] = r[16] + r[3];
    r[14] = r[10] - r[0];
    // r[3] = r[0] - r[10];
    // r[16] = r[17] * r[3];
    r[4] = r[18] * r[14];
    r[12] = r[4] * r[0];
    r[9] = r[2] - r[12];
    r[11] = r[9] == 0.0 ? 1000000.0 : r[18] / r[9];
    // r[16] = r[3] + r[11];
    // r[14] = r[18] - r[9];
    // r[5] = r[14] - r[18];
    // r[0] = r[4] - r[16];
    // r[15] = r[16] == 0.0 ? 1000000.0 : r[15] / r[16];
    // r[0] = r[14] + r[10];
    // r[4] = r[2] - r[2];
    // r[8] = r[18] + r[4];
    r[0] = r[18] + r[11];
    // r[8] = r[15] == 0.0 ? 1000000.0 : r[17] / r[15];
    // r[4] = r[8] + r[18];
    // r[6] = r[11] - r[11];
    // r[12] = r[17] == 0.0 ? 1000000.0 : r[9] / r[17];
    // r[14] = r[17] + r[0];
    // r[2] = r[17] - r[4];
    // r[11] = r[6] - r[4];
    // r[16] = r[15] + r[18];
    // r[2] = r[0] + r[8];
    // r[2] = r[17] * r[13];
    // r[4] = r[17] == 0.0 ? 1000000.0 : r[2] / r[17];
    // r[16] = r[6] == 0.0 ? 1000000.0 : r[1] / r[6];
    // r[6] = r[3] - r[13];
    // r[15] = r[15] * r[7];
    // r[3] = r[9] == 0.0 ? 1000000.0 : r[6] / r[9];
    // r[8] = r[9] * r[14];
    // r[8] = r[14] * r[2];
    // r[15] = r[17] * r[8];
    // r[2] = r[18] == 0.0 ? 1000000.0 : r[3] / r[18];
    // r[15] = r[0] * r[4];
    // r[2] = r[17] == 0.0 ? 1000000.0 : r[9] / r[17];
    // r[14] = r[18] == 0.0 ? 1000000.0 : r[0] / r[18];
    // r[13] = r[3] * r[18];
    // r[16] = r[13] + r[17];
    // r[10] = r[18] - r[14];
    // r[10] = r[6] + r[0];
    // r[9] = r[13] * r[7];
    // r[8] = r[2] + r[15];
    // r[16] = r[6] - r[12];
    // r[5] = r[3] - r[9];
    // r[5] = r[3] + r[17];
    // r[16] = r[12] == 0.0 ? 1000000.0 : r[17] / r[12];
    
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

    double r[19] = {
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
