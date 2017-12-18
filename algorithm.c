#include <stdlib.h>
#include <limits.h>

#include "algorithm.h"

double get_random()
{
    // utility to get a random double between 0 and 1
    return (double) rand() / RAND_MAX;
}

int less_than(const void * a, const void * b) {
    // function used to sort doubles with qsort
    return ( *(double*)a > *(double*)b ) ? 1 : 0;
}

void algorithm_get_parents_and_mutants(int* p1, int* p2, int* m)
{
    // this function fills the arrays p1 and p2 with the indexes selected to be parents
    // (at random according to the probability) and m with the indexes selected to mutate.
    double random;
    int index_m = 0, index_p = 0;
    for (int i = 0; i < POPSIZE; i++) {
        random = get_random();
        if (random < MUTATIONPROB) {
            m[index_m++] = i;
        }
        else if (random < MUTATIONPROB + CROSSOVERPROB) {
            if (index_p % 2) {
                p2[index_p/2] = i;
            }
            else {
                p1[index_p/2] = i;
            }
            index_p++;
        }
    }
    p1[index_p/2] = -1;
    p2[index_p/2] = -1;
    m[index_m] = -1;
}

void generation_init(struct generation* self)
{
    // initialization of the struct generation
    self->population = (struct precalcs*)malloc(POPSIZE*sizeof(struct precalcs));
    self->best = (struct precalcs*)malloc(sizeof(struct precalcs));
    for (int i = 0; i < POPSIZE; i++) {
        weights_init(&self->population[i]);
    }
}

void generation_destruct(struct generation* self)
{
    // memory release
    free(self->population);
    free(self->best);
}

void algorithm_mutation(struct precalcs* weights)
{
    // mutation consists in increasing or decreasing each weight in a random number up to
    // 10 with a certain probability.
    // valid flag is used for not having to recalculate the score in unmodified individuals
    if (get_random() < CHANGEPROB) {
        weights->holes += (rand() % 21) - 10;
    }
    if (get_random() < CHANGEPROB) {
        weights->row_transitions += (rand() % 21) - 10;
    }
    if (get_random() < CHANGEPROB) {
        weights->column_transitions += (rand() % 21) - 10;
    }
    if (get_random() < CHANGEPROB) {
        weights->heights += (rand() % 21) - 10;
    }
    weights->valid = 0;
}

void algorithm_crossover(struct precalcs* weights1, struct precalcs* weights2)
{
    // crossover consists in swapping each weight of a couple of individuals with a
    // certain probability.
    // valid flag is used for not having to recalculate the score in unmodified individuals
    int aux;
    if (get_random() < CHANGEPROB) {
        aux = weights1->holes;
        weights1->holes = weights2->holes;
        weights2->holes = aux;
    }
    if (get_random() < CHANGEPROB) {
        aux = weights1->row_transitions;
        weights1->row_transitions = weights2->row_transitions;
        weights2->row_transitions = aux;
    }
    if (get_random() < CHANGEPROB) {
        aux = weights1->column_transitions;
        weights1->column_transitions = weights2->column_transitions;
        weights2->column_transitions = aux;
    }
    if (get_random() < CHANGEPROB) {
        aux = weights1->heights;
        weights1->heights = weights2->heights;
        weights2->heights = aux;
    }
    weights1->valid = 0;
    weights2->valid = 0;
}

void algorithm_get_scores(struct generation* self, struct data* info)
{
    // calculate the score of each individual
    for (int i = 0; i < POPSIZE; i++) {
        if (!self->population[i].valid) {
            weights_run_loop(&self->population[i], info, 0);
        }
    }
}

void algorithm_set_slices(struct generation* self)
{
    // using the roulette's method here.
    // this function set the slices with the accumulative probability for selection
    int best_index = 0, best_score = INT_MIN;
    self->total = 0;
    self->worst = 0;
    for (int i = 0; i < POPSIZE; i++) {
        self->total += self->population[i].score;
        if (self->population[i].score < self->worst) {
            self->worst = self->population[i].score;
        }
        if (self->population[i].score > best_score) {
            best_index = i;
        }
    }
    self->total += -POPSIZE * self->worst;
    weights_copy(self->best, &self->population[best_index]);
    for (int i = 0; i < POPSIZE; i++) {
        self->slices[i] = (self->population[i].score - self->worst) / self->total;
        self->slices[i] += (i > 0 ? self->slices[i-1] : 0);
        //printf("%d, %f\n", i, self->slices[i]);
    }
    //printf("\n");
}

void algorithm_selection(struct generation* self)
{
    // a set of random doubles is generated and the new individuals are selected
    // depending where do they land on the roulette
    double rands[POPSIZE];
    struct precalcs aux[POPSIZE];
    for (int i = 0; i < POPSIZE; i++) {
        rands[i] = get_random();
        weights_copy(&aux[i], &self->population[i]);
    }
    qsort(rands, POPSIZE, sizeof(double), less_than);
    int current_index = 0;
    for (int i = 0; i < POPSIZE; i++) {
        while (rands[i] > self->slices[current_index] && current_index < POPSIZE) {
            current_index++;
        }
        weights_copy(&self->population[i], &aux[current_index]);
        //printf("%d, %f\n", current_index, rands[i]);
    }
}

void algorithm_crossover_and_mutate(struct generation* self)
{
    // after the selection parents and mutants are selected at random and modifications take place
    int p1[POPSIZE];
    int p2[POPSIZE];
    int m[POPSIZE];
    algorithm_get_parents_and_mutants(p1, p2, m);
    for (int i = 0; p1[i] != -1; i++) {
        algorithm_crossover(&self->population[p1[i]], &self->population[p2[i]]);
    }
    for (int i = 0; m[i] != -1; i++) {
        algorithm_mutation(&self->population[m[i]]);
    }
}

void algorithm_elitism(struct generation* self)
{
    // the worst scored not-modified individual is replaced with the best individual
    // of the previous generation
    int worst_score = INT_MAX;
    int worst_index = -1;
    for (int i = 0; i < POPSIZE; i++) {
        if (self->population[i].valid && self->population[i].score < worst_score) {
            worst_score = self->population[i].score;
            worst_index = i;
        }
    }
    weights_copy(&self->population[worst_index], self->best);
}

void algorithm_run_evolution(struct generation* self, struct data* info)
{
    // the full genetic algorithm
    for (int i = 0; i < MAXGENS; i++) {
        algorithm_get_scores(self, info);
        algorithm_set_slices(self);
        algorithm_selection(self);
        algorithm_elitism(self);
    }
    algorithm_get_scores(self, info);
    algorithm_set_slices(self);
}