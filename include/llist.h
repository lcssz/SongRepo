#ifndef LINKED_LIST_H

typedef struct ll {
  char *word;
  struct ll *next;
} LinkedList;

LinkedList *create_node_ll(const char *);
void insert_node_ll(LinkedList **, LinkedList **, LinkedList *);
void free_list(LinkedList *);

#endif
