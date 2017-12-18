#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "fitness.h"

void fitness_init(struct precalcs* self, struct board* instance)
{
    // initial fitness values
    self->holes = 0;
    self->row_transitions = 2 * instance->depth;
    self->column_transitions = instance->width;
    self->heights = 0;
}

int fitness_get_all_holes(struct board* instance)
{
    // computes all the holes on the board
    // a hole is an empty cell who has a non empty cell above it somewhere in the same column
    int count = 0;
    for (int i = 0; i < instance->width; i++) {
        for (int j = 0; j < instance->heights[i]; j++) {
            if (!board_get_cell(instance, i, j)) {
                count++;
            }
        }
    }
    return count;
}

int fitness_get_all_row_transitions(struct board* instance)
{
    // computes all row transitions
    // actually row transitions remain the same after removing a row
    // row transitions are the number of times there's an empty cell next to a non empty one
    // in each row, or the number of vertical frontiers on each row
    return instance->fitness->row_transitions;
}

int fitness_get_all_column_transitions(struct board* instance)
{
    // computes all column transitions on the board
    // column transitions are the same as row transitions but columnwise
    int count = 0;
    for (int i = 0; i < instance->width; i++) {
        for (int j = instance->heights[i]; j > -1; j--) {
            if (board_get_cell(instance, i, j) != board_get_cell(instance, i, j-1)) {
                count++;
            }
        }
    }
    return count;
}

int fitness_get_new_holes(struct board* instance, struct piece current, int x, int y)
{
    // computes the variation of holes for a certain piece placement by counting the
    // empty spaces directly below the piece
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (current.l[i]) {
            for (int j = y + current.y[i] - 1; j > -1; j--) {
                if (board_get_cell(instance, x + current.x[i], j)) {
                    break;
                }
                else {
                    count ++;
                }
            }
        }
    }
    return count;
}

int fitness_get_new_row_transitions(struct board* instance, struct piece current, int x, int y)
{
    // computes the variation of row transitions for a certain piece placement by counting
    // local row transitions after and before placing the piece
    int count = 0, curr_x, curr_y;
    for (int i = 0; i < 4; i++) {
        curr_x = x + current.x[i];
        curr_y = y + current.y[i];
        if (!board_get_cell(instance, curr_x-1, curr_y)) {
            count++;
        }
        if (!board_get_cell(instance, curr_x+1, curr_y)) {
            count++;
        }
    }
    for (int i = 0; i < 4; i++) {
        board_set_cell(instance, x + current.x[i], y + current.y[i], 0);
    }
    for (int i = 0; i < 4; i++) {
        curr_x = x + current.x[i];
        curr_y = y + current.y[i];
        if (board_get_cell(instance, curr_x-1, curr_y)) {
            count--;
        }
        if (board_get_cell(instance, curr_x+1, curr_y)) {
            count--;
        }
    }
    for (int i = 0; i < 4; i++) {
        board_set_cell(instance, x + current.x[i], y + current.y[i], 1);
    }
    return count;
}

int fitness_get_new_column_transitions(struct board* instance, struct piece current, int x, int y)
{
    // computes the variation of column transitions for a certain piece placement by counting
    // how many cells directly under the piece are empty
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (current.l[i] && !board_get_cell(instance, x + current.x[i], y + current.y[i] - 1)) {
            count += 2;
        }
    }
    return count;
}

int fitness_get_new_heights(struct board* instance, struct piece current, int x, int y)
{
    // gets the piece landing height on the board
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (current.u[i]) {
            count += y + current.y[i];
        }
    }
    return count / current.width;
}

int fitness_get_score(struct precalcs* fitness, struct precalcs* weights, int remaining)
{
    // gets score of the current board state
    int count = 0;
    count += fitness->holes * weights->holes;
    count += fitness->row_transitions * weights->row_transitions;
    count += fitness->column_transitions * weights->column_transitions;
    count -= PENALIZATION * remaining;
    return count;
}

void weights_init(struct precalcs* weights)
{
    // random initialization of the weghts
    // they are all initialized in negative values because the characteristics are all negative
    weights->holes = -(rand() % 101);
    weights->row_transitions = -(rand() % 101);
    weights->column_transitions = -(rand() % 101);
    weights->heights = -(rand() % 101);

    weights->score = 0;
    weights->valid = 0;
}

void weights_copy(struct precalcs* dst, struct precalcs* src)
{
    // utility to copy the struct
    dst->holes = src->holes;
    dst->row_transitions = src->row_transitions;
    dst->column_transitions = src->column_transitions;
    dst->heights = src->heights;

    dst->score = src->score;
    dst->valid = src->valid;
}

int weights_evaluate_state(struct precalcs* weights, struct board* instance, struct piece current, int col)
{
    // returns the variation of score produced by a single piece placement in a certain position
    int row = board_drop_piece(instance, current, col);
    if (row == -1) {
        return INT_MIN;
    }
    int eval = 0;
    eval += weights->holes * fitness_get_new_holes(instance, current, col, row);
    eval += weights->row_transitions * fitness_get_new_row_transitions(instance, current, col, row);
    eval += weights->column_transitions * fitness_get_new_column_transitions(instance, current, col, row);
    eval += weights->heights * fitness_get_new_heights(instance, current, col, row);
    board_unset_last_piece(instance);
    return eval;
}

int weights_select_better_state(struct precalcs* weights, struct board* instance, struct piece* current, int type)
{
    // this function tries every possible position of the piece in the board and selects the best
    // it returns the column of that position and sets the piece to the corrent rotation
    int best_eval = INT_MIN, best_col = -1, best_rot = 0, eval;
    struct piece aux;
    for (int j = 0; j < piece_available_states(type); j++) {
        piece_init(&aux, type, j);
        for (int k = 0; k <= instance->width - aux.width; k++) {
            eval = weights_evaluate_state(weights, instance, aux, k);
            if (eval > best_eval) {
                best_eval = eval;
                best_col = k;
                best_rot = j;
            }
        }
    }
    piece_init(current, type, best_rot);
    return best_col;
}

void weights_run_loop(struct precalcs* weights, struct data* info, int print, int verbose)
{
    // runs a whole game with a certain weight configuration
    // it also has the option of printing the final information like the board, the weights, etc.
    struct board instance;
    board_init(&instance, info->width, info->height);
    struct piece current;
    int current_col, i, rem;
    for (i = 0; i < info->n_pieces; i++) {
        current_col = weights_select_better_state(weights, &instance, &current, info->pieces[i]);
        if (current_col == -1) {
            break;
        }
        rem = board_fixate_piece(&instance, current, current_col);
        if (verbose) {
            printf("Piece %d:\n", i+1);
            board_print(&instance);
            printf("removed lines: %d\n\n", rem);
        }
    }
    weights->score = fitness_get_score(instance.fitness, weights, info->n_pieces - i);
    weights->valid = 1;
    if (print) {
        printf("Final board:\n");
        board_print(&instance);
        printf("Score: %d\n", weights->score);
        printf("Removed lines: %d\n", instance.removed);
        printf("Weights:\n holes: %d\n row transitions: %d\n column transitions: %d\n landing height: %d\n\n",
            weights->holes, weights->row_transitions, weights->column_transitions, weights->heights);
    }
    board_destruct(&instance);
}