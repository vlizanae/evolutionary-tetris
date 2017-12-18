#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algorithm.h"
#include "piece.h"
#include "board.h"
#include "fitness.h"
#include "reader.h"

int main(int argc, char **argv)
{
    int verbose = 0;
    struct timespec initial_time, final_time;
    if (argc < 2) {
        printf("Missing name of the instance\n");
        return 1;
    }
    if (argc == 3 && argv[2][0] == 'v') {
        verbose = 1;
    }
    // setting undeterministic behaviour
    srand(time(0));

    // input
    struct data pieces;
    data_read(&pieces, argv[1]);

    // inicialization
    struct generation individuals;
    generation_init(&individuals);

    // main loop
    clock_gettime(CLOCK_MONOTONIC, &initial_time);
    algorithm_run_evolution(&individuals, &pieces);
    clock_gettime(CLOCK_MONOTONIC, &final_time);

    // solution extraction
    struct precalcs best;
    weights_copy(&best, individuals.best);
    weights_run_loop(&best, &pieces, 1, verbose);

    // time reporting
    double total_time = final_time.tv_sec - initial_time.tv_sec;
    total_time += (final_time.tv_nsec - initial_time.tv_nsec) / 1000000000.0;
    printf("Time taken: %f [s]\n", total_time);

    // set the memory free
    generation_destruct(&individuals);
    data_destruct(&pieces);

    return 0;
}