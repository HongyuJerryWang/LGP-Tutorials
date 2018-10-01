#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int NUM_INPUTS = 4;

void gp(double r[16]) {

    // r[9] = r[6] - r[2];
    // r[4] = r[11] == 0.0 ? 1000000.0 : r[12] / r[11];
    // r[5] = r[7] - r[7];
    r[4] = r[3] == 0.0 ? 1000000.0 : r[15] / r[3];
    // r[12] = r[14] == 0.0 ? 1000000.0 : r[15] / r[14];
    r[7] = r[4] - r[13];
    // r[13] = r[15] - r[2];
    // r[12] = r[12] + r[12];
    // r[4] = r[15] * r[10];
    // r[6] = r[14] - r[9];
    // r[9] = r[2] * r[10];
    // r[6] = r[15] + r[10];
    // r[11] = r[4] == 0.0 ? 1000000.0 : r[6] / r[4];
    // r[11] = fmax(fmin(r[15], r[15]), fmin(fmax(r[15], r[15]), r[7]));
    // r[4] = r[10] == 0.0 ? 1000000.0 : r[1] / r[10];
    // r[6] = r[5] - r[10];
    // r[12] = r[12] == 0.0 ? 1000000.0 : r[2] / r[12];
    // r[10] = r[12] * r[15];
    // r[11] = r[2] - r[15];
    // r[1] = r[4] + r[3];
    // r[5] = r[4] == 0.0 ? 1000000.0 : r[4] / r[4];
    // r[6] = r[5] * r[12];
    // r[10] = r[11] + r[15];
    // r[9] = r[13] == 0.0 ? 1000000.0 : r[11] / r[13];
    // r[6] = r[14] + r[10];
    // r[12] = r[1] * r[10];
    r[0] = r[0] * r[7];
    r[7] = r[0] * r[7];
    // r[5] = r[6] * r[10];
    // r[2] = r[11] == 0.0 ? 1000000.0 : r[15] / r[11];
    r[7] = r[7] == 0.0 ? 1000000.0 : r[14] / r[7];
    // r[11] = r[15] - r[11];
    // r[8] = r[14] - r[13];
    // r[12] = r[8] - r[15];
    r[11] = r[7] * r[14];
    // r[5] = fmax(fmin(r[10], r[14]), fmin(fmax(r[10], r[14]), r[3]));
    r[0] = fmax(fmin(r[0], r[11]), fmin(fmax(r[0], r[11]), r[15]));
    // r[7] = r[0] == 0.0 ? 1000000.0 : r[15] / r[0];
    // r[10] = r[13] * r[11];
    // r[9] = r[14] + r[10];
    // r[10] = r[3] - r[15];
    // r[8] = r[15] + r[10];
    // r[2] = r[12] + r[11];
    // r[12] = r[1] + r[14];
    
}

int main(int argc, char *argv[]) {

    if (argc != NUM_INPUTS + 1) {
        printf("Please specify %d input value(s)...\n", NUM_INPUTS);

        exit(1);
    }

    double r[16] = {
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
        0.0, // constant
        1.0, // constant
    };

    if (NUM_INPUTS == 1) {
        double input = strtod(argv[1], NULL);

        r[0] = input;
    } else if (NUM_INPUTS > 1) {

        for (int i = 0; i < NUM_INPUTS; i++) {
            r[i] = strtod(argv[i + 1], NULL);
        }

    } else {
      printf("Unexpected number of inputs...\n");

      exit(1);
    }

    gp(r);

    printf("%f\n", r[0]);

    return 0;
}