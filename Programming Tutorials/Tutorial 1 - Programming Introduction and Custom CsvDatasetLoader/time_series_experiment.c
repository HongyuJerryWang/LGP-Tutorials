#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int NUM_INPUTS = 4;

void gp(double r[16]) {

    // r[11] = r[9] * r[15];
    // r[5] = r[4] + r[6];
    r[7] = r[1] * r[15];
    // r[6] = r[7] * r[13];
    // r[11] = r[0] - r[5];
    // r[10] = r[12] + r[1];
    // r[8] = r[4] + r[10];
    r[12] = r[0] - r[15];
    // r[0] = r[14] == 0.0 ? 1000000.0 : r[12] / r[14];
    r[8] = r[12] * r[15];
    // r[13] = r[12] == 0.0 ? 1000000.0 : r[15] / r[12];
    // r[13] = r[2] + r[8];
    // r[0] = r[11] == 0.0 ? 1000000.0 : r[15] / r[11];
    // r[9] = r[14] - r[8];
    // r[9] = r[1] - r[13];
    r[13] = r[7] - r[8];
    // r[5] = r[12] - r[11];
    // r[4] = r[10] == 0.0 ? 1000000.0 : r[6] / r[10];
    // r[7] = r[6] == 0.0 ? 1000000.0 : r[13] / r[6];
    // r[6] = r[2] == 0.0 ? 1000000.0 : r[6] / r[2];
    // r[5] = r[15] == 0.0 ? 1000000.0 : r[8] / r[15];
    // r[4] = r[9] + r[14];
    // r[8] = r[10] == 0.0 ? 1000000.0 : r[5] / r[10];
    // r[5] = r[14] + r[3];
    // r[7] = r[15] + r[10];
    // r[10] = r[11] - r[9];
    r[0] = r[2] * r[13];
    // r[12] = r[12] - r[7];
    // r[4] = r[13] * r[7];
    // r[11] = r[4] + r[9];
    // r[12] = r[3] * r[8];
    // r[4] = r[4] == 0.0 ? 1000000.0 : r[12] / r[4];
    // r[4] = r[5] * r[12];
    // r[5] = r[6] == 0.0 ? 1000000.0 : r[2] / r[6];
    // r[8] = r[13] == 0.0 ? 1000000.0 : r[14] / r[13];
    // r[4] = r[15] - r[8];
    // r[4] = r[12] - r[1];
    // r[8] = r[0] * r[14];
    // r[9] = r[8] - r[0];
    // r[12] = r[6] + r[9];
    // r[4] = r[1] == 0.0 ? 1000000.0 : r[15] / r[1];
    // r[10] = r[2] - r[4];
    // r[13] = r[12] == 0.0 ? 1000000.0 : r[8] / r[12];
    // r[4] = r[10] + r[14];
    // r[9] = r[3] + r[15];
    
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