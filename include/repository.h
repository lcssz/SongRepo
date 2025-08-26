#ifndef REPOSITORY_H
#include "song.h"
#include "tree.h"
#include <stdio.h>

typedef struct {
  Tree tree;
  Tree avl_tree;
  Song *songs;
} Repository;

void insert_song(FILE *);

#endif // !REPOSITORY_H
