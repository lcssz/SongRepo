#include "include/structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tree *bin_tree = NULL;
Tree *avl_tree = NULL;
WordArray *sorted_word_array = NULL;
Tree *avl_frequency_tree = NULL;

Node *create_node(const char *word) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node == NULL) {
    fprintf(stderr, "Falha na alocação de memória para o Nó.\n");
    exit(EXIT_FAILURE);
  }
  new_node->word = strdup(word);
  if (new_node->word == NULL) {
    fprintf(stderr, "Falha na alocação de memória para a palavra.\n");
    exit(EXIT_FAILURE);
  }
  new_node->total_word_count = 1;
  new_node->height = 1;
  new_node->best_song_occurrence = NULL;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

SongOccurrence *create_song_occurrence(const char *title, const char *author,
                                       const char *verse_snippet,
                                       unsigned int word_count_in_song) {
  SongOccurrence *new_occurrence =
      (SongOccurrence *)malloc(sizeof(SongOccurrence));
  if (new_occurrence == NULL) {
    fprintf(stderr, "Falha na alocação de memória para SongOccurrence.\n");
    exit(EXIT_FAILURE);
  }
  new_occurrence->title = strdup(title);
  new_occurrence->author = strdup(author);
  new_occurrence->verse_snippet = strdup(verse_snippet);
  new_occurrence->word_count_in_song = word_count_in_song;
  return new_occurrence;
}

Node *right_rotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  return x;
}

Node *left_rotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  return y;
}

// Funções de rotação específicas para frequência
Node *right_rotate_frequency(Node *y) {
  if (y == NULL || y->left == NULL) {
    return y;
  }

  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  return x;
}

Node *left_rotate_frequency(Node *x) {
  if (x == NULL || x->right == NULL) {
    return x;
  }

  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  return y;
}

// Modificar a função insert_node_recursive em tree.c
void insert_node_recursive(Node *current, Node *new_node) {
  int comparison = strcmp(new_node->word, current->word);

  if (comparison < 0) {
    if (current->left == NULL) {
      current->left = new_node;
    } else {
      insert_node_recursive(current->left, new_node);
    }
  } else if (comparison > 0) {
    if (current->right == NULL) {
      current->right = new_node;
    } else {
      insert_node_recursive(current->right, new_node);
    }
  } else {
    // Palavra já existe - incrementar contagem total pelo número de ocorrências
    // na música
    if (new_node->best_song_occurrence != NULL) {
      current->total_word_count +=
          new_node->best_song_occurrence->word_count_in_song;
    } else {
      current->total_word_count++;
    }

    // Verificar se esta nova ocorrência tem mais aparições na música
    if (new_node->best_song_occurrence != NULL &&
        (current->best_song_occurrence == NULL ||
         new_node->best_song_occurrence->word_count_in_song >
             current->best_song_occurrence->word_count_in_song)) {
      // Substituir com a nova ocorrência (tem mais aparições)
      if (current->best_song_occurrence != NULL) {
        free(current->best_song_occurrence->title);
        free(current->best_song_occurrence->author);
        free(current->best_song_occurrence->verse_snippet);
        free(current->best_song_occurrence);
      }
      current->best_song_occurrence = new_node->best_song_occurrence;
      new_node->best_song_occurrence = NULL;
    } else if (new_node->best_song_occurrence != NULL) {
      // Liberar a nova ocorrência (tem menos aparições)
      free(new_node->best_song_occurrence->title);
      free(new_node->best_song_occurrence->author);
      free(new_node->best_song_occurrence->verse_snippet);
      free(new_node->best_song_occurrence);
    }
    free(new_node->word);
    free(new_node);
  }
}

// Modificar também insert_node_avl_recursive com a mesma lógica
Node *insert_node_avl_recursive(Node *current, Node *new_node) {
  if (current == NULL) {
    return new_node;
  }

  int comparison = strcmp(new_node->word, current->word);

  if (comparison < 0) {
    current->left = insert_node_avl_recursive(current->left, new_node);
  } else if (comparison > 0) {
    current->right = insert_node_avl_recursive(current->right, new_node);
  } else {
    // Palavra já existe - incrementar contagem total pelo número de ocorrências
    // na música
    if (new_node->best_song_occurrence != NULL) {
      current->total_word_count +=
          new_node->best_song_occurrence->word_count_in_song;
    } else {
      current->total_word_count++;
    }

    // Verificar se esta nova ocorrência tem mais aparições na música
    if (new_node->best_song_occurrence != NULL &&
        (current->best_song_occurrence == NULL ||
         new_node->best_song_occurrence->word_count_in_song >
             current->best_song_occurrence->word_count_in_song)) {
      // Substituir com a nova ocorrência (tem mais aparições)
      if (current->best_song_occurrence != NULL) {
        free(current->best_song_occurrence->title);
        free(current->best_song_occurrence->author);
        free(current->best_song_occurrence->verse_snippet);
        free(current->best_song_occurrence);
      }
      current->best_song_occurrence = new_node->best_song_occurrence;
      new_node->best_song_occurrence = NULL;
    } else if (new_node->best_song_occurrence != NULL) {
      // Liberar a nova ocorrência (tem menos aparições)
      free(new_node->best_song_occurrence->title);
      free(new_node->best_song_occurrence->author);
      free(new_node->best_song_occurrence->verse_snippet);
      free(new_node->best_song_occurrence);
    }
    free(new_node->word);
    free(new_node);
    return current;
  }

  current->height =
      1 + max(height_node(current->left), height_node(current->right));
  int balance = get_balance(current);

  // Caso Esquerda-Esquerda
  if (balance > 1 && strcmp(new_node->word, current->left->word) < 0) {
    return right_rotate(current);
  }
  // Caso Direita-Direita
  if (balance < -1 && strcmp(new_node->word, current->right->word) > 0) {
    return left_rotate(current);
  }
  // Caso Esquerda-Direita
  if (balance > 1 && strcmp(new_node->word, current->left->word) > 0) {
    current->left = left_rotate(current->left);
    return right_rotate(current);
  }
  // Caso Direita-Esquerda
  if (balance < -1 && strcmp(new_node->word, current->right->word) < 0) {
    current->right = right_rotate(current->right);
    return left_rotate(current);
  }

  return current;
}

Node *insert_node_avl_frequency_recursive(Node *current, Node *new_node) {
  if (current == NULL)
    return new_node;

  if (new_node->total_word_count < current->total_word_count) {
    current->left =
        insert_node_avl_frequency_recursive(current->left, new_node);
  } else if (new_node->total_word_count > current->total_word_count) {
    current->right =
        insert_node_avl_frequency_recursive(current->right, new_node);
  } else {
    // Mesma frequência, compara por palavra para manter a unicidade
    if (new_node->word == NULL || current->word == NULL) {
      free(new_node->word);
      free(new_node);
      return current;
    }

    int cmp = strcmp(new_node->word, current->word);
    if (cmp < 0) {
      current->left =
          insert_node_avl_frequency_recursive(current->left, new_node);
    } else if (cmp > 0) {
      current->right =
          insert_node_avl_frequency_recursive(current->right, new_node);
    } else {
      free(new_node->word);
      free(new_node);
      return current;
    }
  }

  current->height =
      1 + max(height_node(current->left), height_node(current->right));
  int balance = get_balance(current);

  // Realiza rotações com base na comparação de frequência
  // Caso Esquerda-Esquerda
  if (balance > 1 && current->left != NULL &&
      new_node->total_word_count < current->left->total_word_count) {
    return right_rotate_frequency(current);
  }

  // Caso Direita-Direita
  if (balance < -1 && current->right != NULL &&
      new_node->total_word_count > current->right->total_word_count) {
    return left_rotate_frequency(current);
  }

  // Caso Esquerda-Direita
  if (balance > 1 && current->left != NULL &&
      new_node->total_word_count > current->left->total_word_count) {
    current->left = left_rotate_frequency(current->left);
    return right_rotate_frequency(current);
  }

  // Caso Direita-Esquerda
  if (balance < -1 && current->right != NULL &&
      new_node->total_word_count < current->right->total_word_count) {
    current->right = right_rotate_frequency(current->right);
    return left_rotate_frequency(current);
  }

  return current;
}

void insert_node(Node *new_node) {
  if (bin_tree == NULL)
    initialize_tree(bin_tree);
  if (bin_tree->root == NULL)
    bin_tree->root = new_node;
  else
    insert_node_recursive(bin_tree->root, new_node);
}
void insert_node_avl(Node *new_node) {
  if (avl_tree == NULL)
    initialize_tree(avl_tree);
  avl_tree->root = insert_node_avl_recursive(avl_tree->root, new_node);
}

Node *search_bst(Node *root, const char *word) {
  if (root == NULL || strcmp(root->word, word) == 0)
    return root;
  if (strcmp(word, root->word) < 0)
    return search_bst(root->left, word);
  else
    return search_bst(root->right, word);
}
Node *search_avl(Node *root, const char *word) {
  if (root == NULL || strcmp(root->word, word) == 0)
    return root;
  if (strcmp(word, root->word) < 0)
    return search_avl(root->left, word);
  else
    return search_avl(root->right, word);
}

void free_tree(Node *node) {
  if (node == NULL)
    return;
  free_tree(node->left);
  free_tree(node->right);
  if (node->best_song_occurrence != NULL) {
    free(node->best_song_occurrence->title);
    free(node->best_song_occurrence->author);
    free(node->best_song_occurrence->verse_snippet);
    free(node->best_song_occurrence);
  }
  free(node->word);
  free(node);
}

WordArray *create_word_array() {
  WordArray *arr = (WordArray *)malloc(sizeof(WordArray));
  if (!arr) {
    fprintf(stderr, "falha no malloc\n");
    exit(1);
  }
  arr->size = 0;
  arr->capacity = 10;
  arr->nodes = (Node **)malloc(arr->capacity * sizeof(Node *));
  if (!arr->nodes) {
    fprintf(stderr, "falha no malloc\n");
    exit(1);
  }
  return arr;
}
void add_node_to_array(WordArray *arr, Node *node) {
  if (arr->size == arr->capacity) {
    arr->capacity *= 2;
    arr->nodes = (Node **)realloc(arr->nodes, arr->capacity * sizeof(Node *));
    if (!arr->nodes) {
      fprintf(stderr, "falha no realloc\n");
      exit(1);
    }
  }
  arr->nodes[arr->size++] = node;
}
int compare_nodes(const void *a, const void *b) {
  Node *node_a = *(Node **)a;
  Node *node_b = *(Node **)b;
  return strcmp(node_a->word, node_b->word);
}
void sort_word_array(WordArray *arr) {
  qsort(arr->nodes, arr->size, sizeof(Node *), compare_nodes);
}
Node *binary_search_array(WordArray *arr, const char *word) {
  int low = 0, high = arr->size - 1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    int cmp = strcmp(word, arr->nodes[mid]->word);
    if (cmp == 0)
      return arr->nodes[mid];
    else if (cmp < 0)
      high = mid - 1;
    else
      low = mid + 1;
  }
  return NULL;
}
void free_word_array(WordArray *arr) {
  if (arr == NULL)
    return;
  free(arr->nodes);
  free(arr);
}

void populate_array_from_tree(Node *node, WordArray *arr) {
  if (node == NULL)
    return;
  populate_array_from_tree(node->left, arr);
  add_node_to_array(arr, node);
  populate_array_from_tree(node->right, arr);
}

void free_frequency_tree(Node *node) {
  if (node == NULL)
    return;
  free_frequency_tree(node->left);
  free_frequency_tree(node->right);

  if (node->word != NULL) {
    free(node->word);
  }

  if (node->best_song_occurrence != NULL) {
    free(node->best_song_occurrence->title);
    free(node->best_song_occurrence->author);
    free(node->best_song_occurrence->verse_snippet);
    free(node->best_song_occurrence);
  }

  free(node);
}

void populate_frequency_avl_tree(Node *root) {
  if (root == NULL)
    return;

  populate_frequency_avl_tree(root->left);

  // Cria um novo nó para a árvore AVL de frequência, duplicando dados
  // relevantes
  Node *new_freq_node = create_node(root->word);
  new_freq_node->total_word_count = root->total_word_count;
  new_freq_node->best_song_occurrence = NULL;

  if (avl_frequency_tree == NULL) {
    avl_frequency_tree = (Tree *)malloc(sizeof(Tree));
    if (avl_frequency_tree == NULL) {
      fprintf(stderr,
              "Falha na alocação de memória para avl_frequency_tree.\n");
      exit(EXIT_FAILURE);
    }
    avl_frequency_tree->root = NULL;
  }

  avl_frequency_tree->root = insert_node_avl_frequency_recursive(
      avl_frequency_tree->root, new_freq_node);

  populate_frequency_avl_tree(root->right);
}

Node *search_avl_frequency(Node *root, unsigned int frequency) {
  if (root == NULL)
    return NULL;

  if (frequency == root->total_word_count)
    return root;
  else if (frequency < root->total_word_count)
    return search_avl_frequency(root->left, frequency);
  else
    return search_avl_frequency(root->right, frequency);
}

void collect_words_by_frequency(Node *root, unsigned int frequency,
                                WordArray *result) {
  if (root == NULL) {
    return;
  }

  if (root->total_word_count < frequency) {
    collect_words_by_frequency(root->right, frequency, result);
  } else {
    // Se a frequência for >=, procure na subárvore esquerda por mais resultados
    // válidos.
    collect_words_by_frequency(root->left, frequency, result);
    // Adicione o nó atual ao resultado.
    add_node_to_array(result, root);
    // Todos os nós na subárvore direita também são válidos, adicione todos.
    populate_array_from_tree(root->right, result);
  }
}

WordArray *search_all_by_frequency(Node *root, unsigned int frequency) {
  WordArray *results = create_word_array();
  collect_words_by_frequency(root, frequency, results);
  return results;
}
