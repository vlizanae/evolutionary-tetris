#ifndef BOARD_H
#define BOARD_H

#include "fitness.h"
#include "piece.h"

struct board
{
    unsigned char *cells; // array corresponding to the flattened matrix where the pieces are placed
    struct precalcs *fitness; // precalculated fitness, so in each piece landing only local changes will be computed
    int *heights; // landing heights of each column
    int *base_heights; // same, used for rollbacks
    int width, depth; // board dimmensions
    struct piece last; // last piece placed on the board
    int x_last, y_last; // position of this piece
    int removed; // removed lines
};

void board_init(struct board* instance, int width, int depth);
void board_destruct(struct board* instance);
unsigned char board_get_cell(struct board* instance, int x, int y);
void board_set_cell(struct board* instance, int x, int y, unsigned char value);
void board_set_piece(struct board* instance, struct piece piece, int x_corner, int y_corner);
void board_fixate_heights(struct board* instance);
void board_rollback_heights(struct board* instance);
void board_unset_last_piece(struct board* instance);
int board_drop_piece(struct board* instance, struct piece piece, int column);
void board_update_fitness(struct board* self, struct piece current, int column, int row);
int board_check_row(struct board* instance, int row);
void board_remove_row(struct board* instance, int row);
void board_recalculate_heights(struct board* instance);
void board_recalculate_fitness(struct board* instance);
int board_fixate_piece(struct board* instance, struct piece current, int col);
void board_print(struct board* instance);
void board_print_heights(struct board* instance);

#endif