#include "include/tree.h"
#include <stdlib.h>
#include <string.h>

Node *create_node(const char *word) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  new_node->word = word;
  new_node->total_word_count = 0;
  new_node->height = 1;
  return new_node;
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
    current->total_word_count++;
    free(new_node);
  }
}

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
    // Palavra já existe, atualiza a contagem e libera o nó
    current->total_word_count++;
    free(new_node);
    return current;
  }
  // Atualiza a altura do nó
  current->height =
      1 + max(height_node(current->left), height_node(current->right));
  // Pega o fator de balanceamento do nó
  int balance = get_balance(current);

  // Se o nó estiver desbalanceado, faça as rotações
  // Rotação à esquerda (Left-Left Case)
  if (balance > 1 && comparison < 0) {
    return right_rotate(current);
  }
  // Rotação à direita (Right-Right Case)
  if (balance < -1 && comparison > 0) {
    return left_rotate(current);
  }
  // Rotação Esquerda-Direita (Left-Right Case)
  if (balance > 1 && comparison > 0) {
    current->left = left_rotate(current->left);
    return right_rotate(current);
  }
  // Rotação Direita-Esquerda (Right-Left Case)
  if (balance < -1 && comparison < 0) {
    current->right = right_rotate(current->right);
    return left_rotate(current);
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
