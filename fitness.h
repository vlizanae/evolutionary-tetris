#ifndef FITNESS_H
#define FITNESS_H

#define PENALIZATION 1000

#include "piece.h"
#include "board.h"
#include "reader.h"
struct board;
struct data;

struct precalcs
{
    // this struct is used both to store fitness and weights
    // some of the fields are only used on weights
    int holes;
    int row_transitions;
    int column_transitions;
    int heights;

    int score;
    int valid;
};

void fitness_init(struct precalcs* fitness, struct board* instance);
int fitness_get_all_holes(struct board* instance);
int fitness_get_all_row_transitions(struct board* instance);
int fitness_get_all_column_transitions(struct board* instance);

int fitness_get_new_holes(struct board* instance, struct piece current, int x, int y);
int fitness_get_new_row_transitions(struct board* instance, struct piece current, int x, int y);
int fitness_get_new_column_transitions(struct board* instance, struct piece current, int x, int y);
int fitness_get_new_heights(struct board* instance, struct piece current, int x, int y);

int fitness_get_score(struct precalcs* fitness, struct precalcs* weights, int remaining);

void weights_init(struct precalcs* weights);
void weights_copy(struct precalcs* destination, struct precalcs* source);
int weights_evaluate_state(struct precalcs* weights, struct board* instance, struct piece current, int col);
int weights_select_better_state(struct precalcs* weights, struct board* instance, struct piece* current, int type);
void weights_run_loop(struct precalcs* weights, struct data* pieces, int print, int verbose);

#endif