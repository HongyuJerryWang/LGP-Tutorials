#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int NUM_INPUTS = 4;
static int NUM_OUTPUTS = 3;

void gp(double r[21]) {

    // r[13] = r[19] == 0.0 ? 1000000.0 : r[5] / r[19];
    r[15] = r[12] - r[12];
    // r[14] = r[3] * r[1];
    // r[18] = r[19] - r[7];
    // r[9] = r[12] == 0.0 ? 1000000.0 : r[19] / r[12];
    // r[14] = fmax(fmin(r[1], r[8]), fmin(fmax(r[1], r[8]), r[18]));
    // r[4] = fmax(fmin(r[18], r[14]), fmin(fmax(r[18], r[14]), r[20]));
    r[0] = r[15] * r[12];
    // r[1] = r[6] * r[14];
    r[14] = r[3] * r[7];
    // r[4] = r[20] * r[17];
    // r[11] = fmax(fmin(r[20], r[1]), fmin(fmax(r[20], r[1]), r[8]));
    // r[13] = r[20] == 0.0 ? 1000000.0 : r[16] / r[20];
    // r[6] = r[3] * r[20];
    r[7] = r[0] - r[3];
    // r[13] = r[20] - r[9];
    // r[4] = r[12] - r[10];
    // r[18] = r[7] * r[8];
    r[18] = r[7] * r[8];
    r[8] = fmax(fmin(r[8], r[14]), fmin(fmax(r[8], r[14]), r[19]));
    // r[17] = fmax(fmin(r[8], r[20]), fmin(fmax(r[8], r[20]), r[19]));
    // r[14] = r[20] * r[7];
    // r[10] = r[20] + r[16];
    // r[17] = r[20] == 0.0 ? 1000000.0 : r[14] / r[20];
    // r[4] = r[20] + r[18];
    // r[4] = r[1] + r[2];
    // r[3] = r[10] + r[3];
    // r[10] = fmax(fmin(r[19], r[3]), fmin(fmax(r[19], r[3]), r[2]));
    r[16] = r[8] == 0.0 ? 1000000.0 : r[16] / r[8];
    // r[13] = r[6] + r[19];
    r[7] = r[19] + r[8];
    r[14] = r[20] * r[18];
    r[13] = r[14] + r[20];
    // r[6] = r[9] * r[5];
    // r[6] = r[18] * r[19];
    // r[3] = r[19] - r[20];
    // r[12] = r[0] + r[19];
    // r[4] = r[12] * r[10];
    r[10] = r[13] * r[8];
    r[5] = r[5] * r[5];
    r[5] = r[8] * r[5];
    // r[17] = r[8] - r[18];
    // r[14] = r[4] + r[20];
    r[3] = r[10] - r[20];
    // r[2] = r[9] + r[20];
    // r[9] = r[7] * r[0];
    r[6] = r[7] + r[5];
    r[14] = fmax(fmin(r[6], r[16]), fmin(fmax(r[6], r[16]), r[0]));
    // r[17] = r[13] + r[13];
    r[6] = fmax(fmin(r[5], r[20]), fmin(fmax(r[5], r[20]), r[20]));
    // r[15] = r[20] - r[4];
    r[12] = r[16] + r[6];
    // r[15] = r[3] == 0.0 ? 1000000.0 : r[20] / r[3];
    r[2] = r[12] == 0.0 ? 1000000.0 : r[5] / r[12];
    r[12] = r[14] * r[8];
    // r[10] = r[18] + r[7];
    // r[13] = r[20] * r[4];
    // r[13] = fmax(fmin(r[9], r[17]), fmin(fmax(r[9], r[17]), r[1]));
    r[5] = r[3] * r[0];
    // r[6] = r[14] + r[19];
    // r[18] = r[1] - r[12];
    // r[13] = r[16] == 0.0 ? 1000000.0 : r[20] / r[16];
    r[17] = r[5] - r[3];
    // r[6] = r[13] == 0.0 ? 1000000.0 : r[8] / r[13];
    // r[14] = r[1] + r[0];
    // r[16] = r[10] + r[11];
    // r[13] = r[5] == 0.0 ? 1000000.0 : r[7] / r[5];
    // r[4] = r[3] - r[20];
    // r[7] = r[20] + r[13];
    // r[3] = fmax(fmin(r[10], r[16]), fmin(fmax(r[10], r[16]), r[20]));
    // r[14] = fmax(fmin(r[18], r[20]), fmin(fmax(r[18], r[20]), r[19]));
    // r[8] = fmax(fmin(r[3], r[10]), fmin(fmax(r[3], r[10]), r[20]));
    // r[18] = r[14] == 0.0 ? 1000000.0 : r[15] / r[14];
    // r[1] = r[3] - r[2];
    // r[13] = r[5] * r[12];
    // r[9] = r[7] - r[20];
    // r[14] = fmax(fmin(r[20], r[2]), fmin(fmax(r[20], r[2]), r[1]));
    // r[5] = r[12] * r[17];
    // r[13] = r[6] + r[17];
    // r[18] = r[6] - r[18];
    // r[9] = r[11] * r[19];
    // r[11] = r[17] + r[19];
    // r[14] = r[19] - r[17];
    // r[1] = r[16] - r[12];
    // r[10] = r[19] * r[4];
    // r[5] = fmax(fmin(r[15], r[6]), fmin(fmax(r[15], r[6]), r[20]));
    // r[5] = r[0] + r[14];
    // r[4] = r[2] + r[14];
    // r[0] = fmax(fmin(r[19], r[13]), fmin(fmax(r[19], r[13]), r[4]));
    // r[14] = r[4] - r[8];
    // r[0] = r[4] * r[8];
    r[9] = r[12] + r[20];
    r[16] = r[17] == 0.0 ? 1000000.0 : r[19] / r[17];
    // r[3] = r[19] == 0.0 ? 1000000.0 : r[17] / r[19];
    // r[4] = r[13] == 0.0 ? 1000000.0 : r[20] / r[13];
    r[13] = r[9] + r[16];
    // r[10] = r[20] + r[18];
    // r[8] = fmax(fmin(r[10], r[18]), fmin(fmax(r[10], r[18]), r[19]));
    r[10] = r[13] == 0.0 ? 1000000.0 : r[20] / r[13];
    // r[15] = r[20] == 0.0 ? 1000000.0 : r[0] / r[20];
    // r[14] = r[7] + r[20];
    // r[5] = r[2] * r[14];
    // r[16] = r[18] + r[20];
    // r[5] = r[10] + r[13];
    // r[17] = fmax(fmin(r[17], r[9]), fmin(fmax(r[17], r[9]), r[20]));
    // r[13] = r[20] == 0.0 ? 1000000.0 : r[19] / r[20];
    // r[1] = r[19] - r[11];
    r[0] = r[10] - r[2];
    // r[11] = r[8] == 0.0 ? 1000000.0 : r[20] / r[8];
    // r[12] = r[4] == 0.0 ? 1000000.0 : r[20] / r[4];
    // r[11] = r[20] == 0.0 ? 1000000.0 : r[20] / r[20];
    // r[11] = r[2] + r[20];
    // r[16] = r[2] - r[18];
    r[11] = r[20] * r[10];
    // r[7] = fmax(fmin(r[16], r[12]), fmin(fmax(r[16], r[12]), r[2]));
    // r[18] = r[12] + r[17];
    // r[5] = r[20] - r[12];
    // r[3] = r[1] - r[19];
    // r[12] = r[11] + r[20];
    // r[7] = r[20] + r[20];
    // r[12] = r[2] - r[17];
    // r[8] = r[19] * r[19];
    // r[18] = r[7] == 0.0 ? 1000000.0 : r[4] / r[7];
    // r[16] = r[20] * r[1];
    // r[18] = r[20] - r[4];
    // r[13] = r[15] + r[11];
    // r[16] = r[1] - r[18];
    // r[16] = r[7] - r[13];
    // r[17] = r[5] - r[18];
    // r[5] = r[12] * r[6];
    // r[9] = r[20] + r[12];
    // r[6] = r[9] - r[13];
    // r[4] = r[11] - r[19];
    // r[13] = r[19] == 0.0 ? 1000000.0 : r[6] / r[19];
    // r[3] = r[20] + r[15];
    // r[7] = r[16] == 0.0 ? 1000000.0 : r[20] / r[16];
    // r[5] = r[19] * r[11];
    // r[8] = fmax(fmin(r[3], r[6]), fmin(fmax(r[3], r[6]), r[13]));
    // r[18] = r[8] - r[4];
    // r[4] = r[4] == 0.0 ? 1000000.0 : r[1] / r[4];
    // r[3] = r[3] == 0.0 ? 1000000.0 : r[10] / r[3];
    // r[14] = r[14] * r[18];
    // r[9] = fmax(fmin(r[5], r[1]), fmin(fmax(r[5], r[1]), r[20]));
    // r[17] = r[6] - r[9];
    // r[12] = fmax(fmin(r[4], r[5]), fmin(fmax(r[4], r[5]), r[4]));
    r[1] = fmax(fmin(r[2], r[11]), fmin(fmax(r[2], r[11]), r[19]));
    
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

    double r[21] = {
        0.0, // input
        0.0, // input
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
        r[2] = strtod(argv[3], NULL);
        r[3] = strtod(argv[4], NULL);

    } else {
      printf("Unexpected number of inputs...\n");

      exit(1);
    }

    gp(r);

    printf("%s\n", classify((double[]) { r[0], r[1], r[2] }, (char *[]) { "Iris-setosa", "Iris-versicolor", "Iris-virginica" }, 3));
    return 0;
}
