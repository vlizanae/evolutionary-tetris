#ifndef READER_H
#define READER_H

#include "algorithm.h"
struct generation;

struct data
{
    int width, height; // dimmensions of the board
    int n_pieces; // number of pieces
    int *pieces; // pieces identifiers
};

void data_read(struct data* instance, char* file_name);
void data_destruct(struct data* instance);
void dump_generation(struct generation* generation);
void load_generation(struct generation* generation, char* name);

#endif