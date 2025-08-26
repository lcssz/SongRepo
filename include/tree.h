#include <stdbool.h>
#ifndef TREE_H

typedef struct {
  const char *word;
  unsigned char count;
} Node;

typedef struct {
  Node *left;
  Node *right;
} Tree;

#define max(a, b) ((a) > (b) ? (a) : (b))

Node *create_node(const char *);
void insert_node(Node *, bool);
void get_balance(Node *);
int height_node(Node *);
Node *left_rotate(Node *);
Node *right_rotate(Node *);

#endif // !TREE_H
