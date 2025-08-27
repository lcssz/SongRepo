#ifndef REPOSITORY_H
#include "llist.h"
#include "tree.h"
#include <stdio.h>

typedef struct {
  const char *title;
  const char *author;
  const char **lyrics_lines;
  int number_of_lines;
} Song;

typedef struct {
  Node *bst_root;
  Node *avl_root;
  Song *songs;
  int num_songs;
} Repository;

void process_music_file_for_word_count(const char *);
void process_line_and_insert_words(const char *);

#endif // !REPOSITORY_H
