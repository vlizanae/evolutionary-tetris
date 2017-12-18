#ifndef ALGORITHM_H
#define ALGORITHM_H

#define POPSIZE 20
#define MAXGENS 1000
#define CHANGEPROB 0.4
#define MUTATIONPROB 0.1
#define CROSSOVERPROB 0.4

#include "fitness.h"
#include "reader.h"
struct data;

struct generation
{
    struct precalcs *population; // individuals of each generation
    struct precalcs *best; // spot used for elitism
    int indexes[POPSIZE]; // array of indexes used for selection
    double slices[POPSIZE]; // array of probabilities also used for selection
    double worst; // worst score of the generation
    double total; // total score, both of this values are used to build the roulette
};

double get_random();
int less_than(const void * a, const void * b);
void algorithm_get_parents_and_mutants(int* parents1, int* parents2, int* mutants);

void generation_init(struct generation* instance);
void generation_destruct(struct generation* instance);
void algorithm_mutation(struct precalcs* weights);
void algorithm_crossover(struct precalcs* weights1, struct precalcs* weights2);
void algorithm_get_scores(struct generation* instance, struct data* pieces);
void algorithm_set_slices(struct generation* instance);
void algorithm_selection(struct generation* instance);
void algorithm_elitism(struct generation* instance);
void algorithm_run_evolution(struct generation* instance, struct data* pieces);

#endif
