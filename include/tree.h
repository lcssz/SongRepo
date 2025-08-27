#include <stdbool.h>
#ifndef TREE_H

typedef struct {
  const char *title;
  const char *author;
  const char *verse_snippet;
  unsigned int word_count_in_song;
} SongOccurrence;

typedef struct node {
  const char *word;
  unsigned char total_word_count;
  unsigned char height;
  SongOccurrence *best_song_occurrence;
  struct node *left;
  struct node *right;
} Node;

typedef struct {
  Node *root;
} Tree;

#define max(a, b) ((a) > (b) ? (a) : (b))
#define initialize_tree(tree_name) (tree_name = (Tree *)malloc(sizeof(Tree)))
#define height_node(N) ((N) == NULL ? 0 : (N)->height)
#define get_balance(N)                                                         \
  ((N) == NULL ? 0 : (height_node((N)->left) - height_node((N)->right)))

Node *create_node(const char *);
void insert_node(Node *);
void insert_node_avl(Node *);
Node *left_rotate(Node *);
Node *right_rotate(Node *);

extern Tree *bin_tree;
extern Tree *avl_tree;

#endif // !TREE_H
