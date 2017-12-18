#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "reader.h"

void data_read(struct data* self, char* name)
{
    // to read the instance, the list of possible pieces is ignored by now
    char path[128];
    strcpy(path, "instances/");
    strcat(path, name);
    FILE *fd = fopen(path, "r");
    fscanf(fd, "%d %d\n", &self->width, &self->height);
    fscanf(fd, "%d", &self->n_pieces);
    self->pieces = (int *)malloc(sizeof(int)*self->n_pieces);
    fscanf(fd, "%s\n", path);
    for (int i = 0; i < self->n_pieces; i++) {
        fscanf(fd, "%d", &self->pieces[i]);
    }
    fclose(fd);
}

void data_destruct(struct data* self)
{
    // freedom of memory
    free(self->pieces);
}

void dump_generation(struct generation* gen)
{
    // not in use, to save a generation
    // intended for when more generations are needed, so the algorithm doesn't have to run from scratch
    char path[128], buffer[128];
    strcpy(path, "dumps/");
    sprintf(buffer, "%ld.csv", time(0));
    strcat(path, buffer);
    FILE *fd = fopen(path, "w");
    for (int i = 0; i < POPSIZE; i++) {
        fprintf(fd, "%d,%d,%d,%d\n",
            gen->population[i].holes,
            gen->population[i].row_transitions,
            gen->population[i].column_transitions,
            gen->population[i].heights
        );
    }
    fclose(fd);
}

void load_generation(struct generation* gen, char* name)
{
    // not in use, to load a saved generation
    char path[128];
    strcpy(path, "dumps/");
    strcat(path, name);
    FILE *fd = fopen(path, "r");
    for (int i = 0; i < POPSIZE; i++) {
        fscanf(fd, "%d,%d,%d,%d\n",
            &gen->population[i].holes,
            &gen->population[i].row_transitions,
            &gen->population[i].column_transitions,
            &gen->population[i].heights            
        );
    }
    fclose(fd);
}