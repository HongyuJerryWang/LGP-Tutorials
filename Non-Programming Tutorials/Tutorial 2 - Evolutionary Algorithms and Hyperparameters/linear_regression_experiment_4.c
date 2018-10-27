#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int NUM_INPUTS = 2;
static int NUM_OUTPUTS = 1;

void gp(double r[19]) {

    // r[12] = r[11] * r[3];
    // r[10] = r[3] * r[18];
    // r[0] = r[17] * r[3];
    // r[0] = r[1] - r[18];
    // r[3] = r[9] == 0.0 ? 1000000.0 : r[9] / r[9];
    // r[13] = r[8] == 0.0 ? 1000000.0 : r[9] / r[8];
    // r[5] = r[6] == 0.0 ? 1000000.0 : r[1] / r[6];
    // r[0] = r[10] + r[14];
    r[15] = r[15] - r[18];
    r[3] = r[15] == 0.0 ? 1000000.0 : r[18] / r[15];
    // r[13] = r[8] * r[12];
    // r[0] = r[8] * r[17];
    // r[5] = r[15] * r[1];
    r[13] = r[17] + r[3];
    r[13] = r[13] == 0.0 ? 1000000.0 : r[1] / r[13];
    // r[6] = r[5] * r[18];
    // r[0] = r[8] == 0.0 ? 1000000.0 : r[10] / r[8];
    // r[0] = r[7] == 0.0 ? 1000000.0 : r[7] / r[7];
    // r[0] = r[12] - r[18];
    // r[0] = r[11] + r[17];
    // r[12] = r[8] * r[10];
    r[4] = r[1] * r[13];
    // r[9] = r[6] + r[11];
    // r[0] = r[2] - r[14];
    // r[6] = r[0] + r[10];
    // r[10] = r[9] - r[17];
    r[9] = r[11] + r[17];
    // r[6] = r[9] + r[4];
    // r[7] = r[11] - r[6];
    // r[5] = r[14] == 0.0 ? 1000000.0 : r[4] / r[14];
    // r[12] = r[5] * r[18];
    r[12] = r[8] + r[17];
    r[6] = r[4] == 0.0 ? 1000000.0 : r[18] / r[4];
    r[10] = r[17] == 0.0 ? 1000000.0 : r[6] / r[17];
    r[12] = r[12] + r[11];
    r[16] = r[15] == 0.0 ? 1000000.0 : r[9] / r[15];
    // r[0] = r[18] == 0.0 ? 1000000.0 : r[8] / r[18];
    // r[2] = r[13] - r[17];
    r[13] = r[17] + r[16];
    // r[9] = r[18] + r[10];
    r[1] = r[18] - r[10];
    r[4] = r[14] + r[18];
    r[9] = r[4] + r[17];
    // r[14] = r[17] - r[9];
    // r[5] = r[14] + r[18];
    r[3] = r[9] == 0.0 ? 1000000.0 : r[17] / r[9];
    // r[6] = r[18] - r[12];
    // r[9] = r[9] - r[0];
    r[8] = r[4] == 0.0 ? 1000000.0 : r[12] / r[4];
    // r[7] = r[18] * r[14];
    // r[5] = r[0] - r[18];
    // r[6] = r[9] == 0.0 ? 1000000.0 : r[18] / r[9];
    // r[5] = r[17] * r[5];
    // r[7] = r[6] - r[10];
    // r[0] = r[11] == 0.0 ? 1000000.0 : r[12] / r[11];
    r[5] = r[13] + r[8];
    // r[13] = r[0] == 0.0 ? 1000000.0 : r[5] / r[0];
    // r[7] = r[17] + r[2];
    // r[16] = r[4] * r[5];
    // r[10] = r[17] * r[13];
    // r[11] = r[18] + r[14];
    // r[16] = r[3] + r[17];
    // r[10] = r[4] - r[17];
    // r[11] = r[10] == 0.0 ? 1000000.0 : r[1] / r[10];
    // r[0] = r[11] == 0.0 ? 1000000.0 : r[17] / r[11];
    // r[14] = r[17] + r[13];
    r[7] = r[18] - r[1];
    // r[0] = r[8] - r[17];
    // r[6] = r[2] - r[11];
    // r[2] = r[17] + r[13];
    // r[15] = r[18] + r[2];
    // r[10] = r[8] - r[1];
    // r[16] = r[18] * r[8];
    // r[6] = r[17] * r[9];
    // r[8] = r[17] * r[15];
    // r[12] = r[16] * r[7];
    r[11] = r[3] == 0.0 ? 1000000.0 : r[5] / r[3];
    // r[6] = r[5] * r[7];
    r[4] = r[11] * r[18];
    r[3] = r[18] * r[4];
    // r[2] = r[18] + r[7];
    // r[13] = r[18] - r[16];
    // r[12] = r[9] - r[2];
    // r[13] = r[17] + r[12];
    // r[14] = r[12] * r[4];
    // r[15] = r[3] - r[8];
    // r[6] = r[17] * r[7];
    // r[12] = r[6] * r[4];
    // r[13] = r[4] == 0.0 ? 1000000.0 : r[18] / r[4];
    // r[10] = r[4] == 0.0 ? 1000000.0 : r[17] / r[4];
    r[0] = r[7] == 0.0 ? 1000000.0 : r[3] / r[7];
    // r[7] = r[13] == 0.0 ? 1000000.0 : r[18] / r[13];
    // r[15] = r[11] + r[16];
    // r[15] = r[5] * r[3];
    // r[12] = r[10] * r[17];
    // r[5] = r[18] * r[8];
    // r[15] = r[18] - r[15];
    // r[11] = r[5] - r[17];
    
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
