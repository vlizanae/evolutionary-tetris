#include <stdio.h>
#include <string.h>

#include "piece.h"

void piece_print_frontier(struct piece self)
{
    // piece printing utility to check if the hardcoding went well
    char repr[16];
    memset(&repr, '-', 16);
    for (int i = 0; i < 4; i++) {
        if (self.u[i] && self.l[i])
            repr[4 * self.y[i] + self.x[i]] = 'B';
        else if (self.u[i])
            repr[4 * self.y[i] + self.x[i]] = 'U';
        else if (self.l[i])
            repr[4 * self.y[i] + self.x[i]] = 'L';
        else
            repr[4 * self.y[i] + self.x[i]] = 'N';
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%c", repr[4*(3-i) + j]);
        }
        printf("\n");
    }
}

int piece_available_states(int type)
{
    // to avoid innecesary iterations when evaluating possible states
    switch (type) {
        case 1:
            return 2;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 1;
        case 5:
            return 4;
        case 6:
            return 4;
        case 7:
            return 2;
        default:
            return 0;
    }
}

void piece_copy(struct piece* src, struct piece* dst)
{
    // struct copy utility
    for (int i = 0; i < 4; i++) {
        dst->x[i] = src->x[i];
        dst->y[i] = src->y[i];
        dst->u[i] = src->u[i];
        dst->l[i] = src->l[i];
    }
    dst->width = src->width;
    dst->height = src->height;
}

void piece_init(struct piece* self, int type, int rot)
{
    // This is where real hardcoding comes to life
    switch (type) {
        case 1:
            if (rot % 2 == 0) {
                self->x[0] = 0; self->y[0] = 1;
                self->x[1] = 1; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 0;
                self->x[3] = 2; self->y[3] = 0;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 0;
                self->u[2] = 0; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 3;
                self->height = 2;
            }
            else {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 1;
                self->x[3] = 1; self->y[3] = 2;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 0;
                self->u[2] = 0; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 0;
                
                self->width = 2;                
                self->height = 3;
            }
            break;
        case 2:
            if (rot % 2 == 0) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 0;
                self->x[2] = 1; self->y[2] = 1;
                self->x[3] = 2; self->y[3] = 1;
                
                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;
                
                self->width = 3;
                self->height = 2;
            }
            else {
                self->x[0] = 1; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 1;
                self->x[2] = 0; self->y[2] = 1;
                self->x[3] = 0; self->y[3] = 2;
                
                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 0;
                self->u[2] = 0; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 0;
                
                self->width = 2;
                self->height = 3;
            }
            break;
        case 3:
            if (rot % 4 == 0) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 0;
                self->x[2] = 2; self->y[2] = 0;
                self->x[3] = 1; self->y[3] = 1;
                
                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 0;
                
                self->width = 3;
                self->height = 2;
            }
            else if (rot % 4 == 1) {
                self->x[0] = 1; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 2;
                self->x[3] = 0; self->y[3] = 1;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 0;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 2;
                self->height = 3;
            }
            else if (rot % 4 == 2) {
                self->x[0] = 1; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 1;
                self->x[3] = 2; self->y[3] = 1;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;
                
                self->width = 3;
                self->height = 2;
            }
            else {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 0; self->y[2] = 2;
                self->x[3] = 1; self->y[3] = 1;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 0;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 2;
                self->height = 3;
            }
            break;
        case 4:
            self->x[0] = 0; self->y[0] = 0;
            self->x[1] = 0; self->y[1] = 1;
            self->x[2] = 1; self->y[2] = 0;
            self->x[3] = 1; self->y[3] = 1;

            self->u[0] = 0; self->l[0] = 1;
            self->u[1] = 1; self->l[1] = 0;
            self->u[2] = 0; self->l[2] = 1;
            self->u[3] = 1; self->l[3] = 0;
            
            self->width = 2;
            self->height = 2;
            break;
        case 5:
            if (rot % 4 == 0) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 0;
                self->x[2] = 1; self->y[2] = 1;
                self->x[3] = 1; self->y[3] = 2;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 1;
                self->u[2] = 0; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 0;

                self->width = 2;
                self->height = 3;
            }
            else if (rot % 4 == 1) {
                self->x[0] = 0; self->y[0] = 1;
                self->x[1] = 1; self->y[1] = 1;
                self->x[2] = 2; self->y[2] = 1;
                self->x[3] = 2; self->y[3] = 0;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 0; self->l[3] = 1;

                self->width = 3;
                self->height = 2;
            }
            else if (rot % 4 == 2) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 0; self->y[2] = 2;
                self->x[3] = 1; self->y[3] = 2;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 0;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 2;
                self->height = 3;
            }
            else {
                self->x[0] = 0; self->y[0] = 1;
                self->x[1] = 0; self->y[1] = 0;
                self->x[2] = 1; self->y[2] = 0;
                self->x[3] = 2; self->y[3] = 0;

                self->u[0] = 1; self->l[0] = 0;
                self->u[1] = 0; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 3;
                self->height = 2;
            }
            break;
        case 6:
            if (rot % 4 == 0) {
                self->x[0] = 1; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 0;
                self->x[2] = 0; self->y[2] = 1;
                self->x[3] = 0; self->y[3] = 2;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 1;
                self->u[2] = 0; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 0;

                self->width = 2;
                self->height = 3;
            }
            else if (rot % 4 == 1) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 0;
                self->x[2] = 2; self->y[2] = 0;
                self->x[3] = 2; self->y[3] = 1;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 1;
                self->u[2] = 0; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 0;
                
                self->width = 3;
                self->height = 2;
            }
            else if (rot % 4 == 2) {
                self->x[0] = 1; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 2;
                self->x[3] = 0; self->y[3] = 2;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 0;
                self->u[2] = 1; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 2;
                self->height = 3;
            }
            else {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 1; self->y[2] = 1;
                self->x[3] = 2; self->y[3] = 1;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 0;
                self->u[2] = 1; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 3;
                self->height = 2;
            }
            break;
        case 7:
            if (rot % 2 == 0) {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 0; self->y[1] = 1;
                self->x[2] = 0; self->y[2] = 2;
                self->x[3] = 0; self->y[3] = 3;

                self->u[0] = 0; self->l[0] = 1;
                self->u[1] = 0; self->l[1] = 0;
                self->u[2] = 0; self->l[2] = 0;
                self->u[3] = 1; self->l[3] = 0;

                self->width = 1;
                self->height = 4;
            }
            else {
                self->x[0] = 0; self->y[0] = 0;
                self->x[1] = 1; self->y[1] = 0;
                self->x[2] = 2; self->y[2] = 0;
                self->x[3] = 3; self->y[3] = 0;

                self->u[0] = 1; self->l[0] = 1;
                self->u[1] = 1; self->l[1] = 1;
                self->u[2] = 1; self->l[2] = 1;
                self->u[3] = 1; self->l[3] = 1;

                self->width = 4;
                self->height = 1;
            }
            break;
        default:
            self->x[0] = -1; self->y[0] = -1;
            self->x[1] = -1; self->y[1] = -1;
            self->x[2] = -1; self->y[2] = -1;
            self->x[3] = -1; self->y[3] = -1;

            self->u[0] = 0; self->l[0] = 0;
            self->u[1] = 0; self->l[1] = 0;
            self->u[2] = 0; self->l[2] = 0;
            self->u[3] = 0; self->l[3] = 0;

            self->width = 0;
            self->height = 0;
            break;
    }
}