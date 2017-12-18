#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "board.h"

void board_init(struct board* self, int width, int depth)
{
    // memory allocation and initialization
    self->width = width;
    self->depth = depth;
    self->removed = 0;

    self->cells = (unsigned char*)malloc(sizeof(unsigned char)*width*depth);
    memset(self->cells, 0, width*depth);

    self->fitness = (struct precalcs*)malloc(sizeof(struct precalcs));
    fitness_init(self->fitness, self);

    self->heights = (int*)malloc(sizeof(int)*width);
    wmemset(self->heights, 0, width);

    self->base_heights = (int*)malloc(sizeof(int)*width);
    wmemset(self->base_heights, 0, width);

    struct piece empty;
    piece_init(&empty, 0, 0);
    self->last = empty;
    self->x_last = 0;
    self->y_last = 0;
}

void board_destruct(struct board* self)
{
    // memory release
    free(self->cells);
    free(self->fitness);
    free(self->heights);
    free(self->base_heights);
}

unsigned char board_get_cell(struct board* self, int x, int y)
{
    // as the board is one-dimmensional this function helps in getting the value of the cells
    if (x < 0 || x >= self->width || y < 0) {
        return 1;
    }
    return self->cells[y * self->width + x];
}

void board_set_cell(struct board* self, int x, int y, unsigned char value)
{
    // the same but this one is to modify
    self->cells[y * self->width + x] = value;
}

void board_set_piece(struct board* self, struct piece current, int x, int y)
{
    // place an entire piece in the board by the position of the lower-left corner of its mask
    for (int i = 0; i < 4; i++) {
        board_set_cell(self, x + current.x[i], y + current.y[i], 1);
    }
    self->last = current;
    self->x_last = x;
    self->y_last = y;

    for (int i = 0; i < 4; i++) {
        if (current.u[i]) {
            self->heights[x + current.x[i]] = y + current.y[i] + 1;
        }
    }
}

void board_fixate_heights(struct board* self)
{
    // set the rollback array of heights to the current heights (for when the final position
    // of some piece is selected)
    memcpy(self->base_heights, self->heights, self->width*sizeof(int));
}

void board_rollback_heights(struct board* self)
{
    // rollback the array of heights, for when unsetting a piece
    memcpy(self->heights, self->base_heights, self->width*sizeof(int));
}

void board_unset_last_piece(struct board* self)
{
    // remove the last piece placed on the board and get the previous heights
    for (int i = 0; i < 4; i++) {
        board_set_cell(self, self->x_last + self->last.x[i], self->y_last + self->last.y[i], 0);
    }
    board_rollback_heights(self);
}

int board_drop_piece(struct board* self, struct piece current, int col)
{
    // higher level function tu drop a piece in some column (the piece is
    // idetified by its leftmost column)
    int max_h = -1, h;
    for (int i = 0; i < 4; i++) {
        if (current.l[i]) {
            h = self->heights[col + current.x[i]] - current.y[i];
            if (h > max_h) {
                max_h = h;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (current.u[i] && current.y[i] + max_h >= self->depth) {
            return -1;
        }
    }
    board_set_piece(self, current, col, max_h);
    return max_h;
}

void board_update_fitness(struct board* self, struct piece current, int col, int row)
{
    // update board's precalculated fitness
    self->fitness->holes += fitness_get_new_holes(self, current, col, row);
    self->fitness->row_transitions += fitness_get_new_row_transitions(self, current, col, row);
    self->fitness->column_transitions += fitness_get_new_column_transitions(self, current, col, row);
}

int board_check_row(struct board* self, int row)
{
    // returns 1 if a row is complete, 0 otherwise
    for (int i = 0; i < self->width; i++) {
        if (!board_get_cell(self, i, row)) {
            return 0;
        }
    }
    return 1;
}

void board_remove_row(struct board* self, int row)
{
    // move the upper part of the board down a row
    self->removed++;
    memcpy(
        &(self->cells[row * self->width]),
        &(self->cells[(row+1) * self->width]),
        self->width * (self->depth - row)
    );
    memset(&(self->cells[(self->depth - 1) * self->width]), 0, self->width);
    for (int i = 0; i < self->width; i++) {
        if (self->heights[i] >= row-1) {
            for (int j = self->heights[i]-1; j > -2; j--) {
                if (board_get_cell(self, i, j)) {
                    self->heights[i] = j + 1;
                    break;
                }
                if (j == -1) {
                    self->heights[i] = j + 1;
                }
            }
        }
    }
}

void board_recalculate_heights(struct board* self)
{
    // for when removing rows
    for (int i = 0; i < self->width; i++) {
        while (!board_get_cell(self, i, self->heights[i] - 1) && self->heights[i] > 0) {
            self->heights[i]--;
        }
    }
    board_fixate_heights(self);
}

void board_recalculate_fitness(struct board* self)
{
    // precalculate fitness from scratch
    self->fitness->holes = fitness_get_all_holes(self);
    self->fitness->row_transitions = fitness_get_all_row_transitions(self);
    self->fitness->column_transitions = fitness_get_all_column_transitions(self);
}

void board_fixate_piece(struct board* self, struct piece current, int col)
{
    // mechanic of dropping a piece and its implicances (remove rows if some are completed,
    // calculate and recalculate what is necesary)
    int row = board_drop_piece(self, current, col);
    board_update_fitness(self, current, col, row);
    board_fixate_heights(self);
    int mod = 0;
    for (int i = row + current.height - 1; i >= row ; i--) {
        if (board_check_row(self, i)) {
            board_remove_row(self, i);
            mod = 1;
        }
    }
    if (mod) {
        board_recalculate_heights(self);
        board_recalculate_fitness(self);
    }
    //board_print(self);
    //board_print_heights(self);
}

void board_print(struct board* self)
{
    // prints the current state of the board
    for (int i = 0; i < self->depth; i++) {
        for (int j = 0; j < self->width; j++) {
            printf("%u", board_get_cell(self, j, self->depth - 1 - i));
        }
        printf("\n");
    }
}

void board_print_heights(struct board* self)
{
    // prints the board heights
    for (int i = 0; i < self->width; i++) {
        printf("%d ", self->heights[i]);
    }
    printf("\n");
}