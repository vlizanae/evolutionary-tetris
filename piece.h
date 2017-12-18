#ifndef PIECE_H
#define PIECE_H

struct piece
{
    // each piece consist of a mask
    int x[4], y[4]; // position of each cell in the mask, relative to the lower left corner
    unsigned char u[4], l[4]; // boolean, whether the i-th cell is upper bound and lower bound of the piece
    int width, height; // dimmension of the mask
};

void piece_print_frontier(struct piece instance);
int piece_available_states(int type);
void piece_copy(struct piece* source, struct piece* destination);
void piece_init(struct piece* instance, int type, int rotation);

#endif