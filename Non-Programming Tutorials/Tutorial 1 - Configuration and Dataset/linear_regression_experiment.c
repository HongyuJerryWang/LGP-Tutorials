#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int NUM_INPUTS = 2;

void gp(double r[14]) {

    r[6] = r[11] + r[1];
    // r[8] = pow(r[6], r[2]);
    // r[2] = r[11] * r[8];
    // r[2] = r[2] * r[8];
    // r[5] = pow(r[7], r[2]);
    r[8] = pow(r[12], r[6]);
    r[6] = r[4] * r[13];
    r[8] = r[8] * r[10];
    // r[3] = r[2] * r[12];
    // r[11] = r[2] + r[3];
    // r[3] = r[6] * r[9];
    r[3] = r[12] * r[8];
    // r[8] = r[2] - r[6];
    // r[2] = r[12] + r[4];
    // r[2] = r[2] * r[6];
    // r[5] = pow(r[10], r[3]);
    // r[2] = r[11] + r[3];
    // r[4] = pow(r[2], r[13]);
    // r[9] = r[7] * r[7];
    r[9] = r[6] + r[3];
    r[4] = pow(r[9], r[12]);
    r[9] = r[1] / r[4];
    r[0] = pow(r[0], r[9]);
    // r[2] = r[12] - r[3];
    
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
