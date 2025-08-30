/**
 * @file tree.h
 * @brief Definições para árvores binárias de busca e AVL
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 */

#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <time.h>
#include "word_info.h"

/**
 * @brief Estrutura para armazenar ocorrência de música
 */
typedef struct {
  const char *title;           /**< Título da música */
  const char *author;          /**< Autor da música */
  const char *verse_snippet;   /**< Trecho da estrofe */
  unsigned int word_count_in_song; /**< Contagem de palavras na música */
} SongOccurrence;

/**
 * @brief Estrutura de nó para árvore binária básica
 */
typedef struct node {
  const char *word;                    /**< Palavra armazenada */
  unsigned char total_word_count;      /**< Contagem total da palavra */
  unsigned char height;                /**< Altura do nó na árvore */
  SongOccurrence *best_song_occurrence; /**< Melhor ocorrência da palavra */
  struct node *left;                   /**< Filho esquerdo */
  struct node *right;                  /**< Filho direito */
} Node;

/**
 * @brief Estrutura para árvore binária básica
 */
typedef struct {
  Node *root;  /**< Raiz da árvore */
} Tree;

/**
 * @brief Estrutura de nó para árvore completa com WordInfo
 */
typedef struct node_complete {
  WordInfo *info;              /**< Informações completas da palavra */
  unsigned char height;        /**< Altura do nó na árvore */
  struct node_complete *left;  /**< Filho esquerdo */
  struct node_complete *right; /**< Filho direito */
} NodeComplete;

/**
 * @brief Estrutura para árvore completa
 */
typedef struct {
  NodeComplete *root;  /**< Raiz da árvore */
} TreeComplete;

/**
 * @brief Macro para calcular o máximo entre dois valores
 */
#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Macro para inicializar uma árvore
 */
#define initialize_tree(tree_name) (tree_name = (Tree *)malloc(sizeof(Tree)))

/**
 * @brief Macro para obter a altura de um nó
 */
#define height_node(N) ((N) == NULL ? 0 : (N)->height)

/**
 * @brief Macro para calcular o fator de balanceamento
 */
#define get_balance(N)                                                         \
  ((N) == NULL ? 0 : (height_node((N)->left) - height_node((N)->right)))

/**
 * @brief Cria um novo nó para árvore básica
 * @param word Palavra a ser armazenada
 * @return Ponteiro para o novo nó ou NULL se falhar
 */
Node *create_node(const char *word);

/**
 * @brief Insere um nó na árvore binária de busca
 * @param new_node Nó a ser inserido
 */
void insert_node(Node *new_node);

/**
 * @brief Insere um nó na árvore AVL
 * @param new_node Nó a ser inserido
 */
void insert_node_avl(Node *new_node);

/**
 * @brief Realiza rotação à esquerda
 * @param x Nó raiz da subárvore
 * @return Nova raiz após rotação
 */
Node *left_rotate(Node *x);

/**
 * @brief Realiza rotação à direita
 * @param y Nó raiz da subárvore
 * @return Nova raiz após rotação
 */
Node *right_rotate(Node *y);

/**
 * @brief Busca uma palavra na árvore binária de busca
 * @param root Raiz da árvore
 * @param word Palavra a ser buscada
 * @return Ponteiro para o nó se encontrado, NULL caso contrário
 */
Node* search_bst(Node *root, const char *word);

/**
 * @brief Busca uma palavra na árvore AVL
 * @param root Raiz da árvore
 * @param word Palavra a ser buscada
 * @return Ponteiro para o nó se encontrado, NULL caso contrário
 */
Node* search_avl(Node *root, const char *word);

/**
 * @brief Cria um novo nó para árvore completa
 * @param info Informações da palavra
 * @return Ponteiro para o novo nó ou NULL se falhar
 */
NodeComplete* create_node_complete(WordInfo *info);

/**
 * @brief Insere um nó na árvore binária completa
 * @param tree Árvore onde inserir
 * @param info Informações da palavra
 */
void insert_node_complete_bst(TreeComplete *tree, WordInfo *info);

/**
 * @brief Insere um nó na árvore AVL completa (recursivo)
 * @param root Raiz da subárvore
 * @param info Informações da palavra
 * @return Nova raiz da subárvore
 */
NodeComplete* insert_node_complete_avl_recursive(NodeComplete *root, WordInfo *info);

/**
 * @brief Insere um nó na árvore AVL completa
 * @param tree Árvore onde inserir
 * @param info Informações da palavra
 */
void insert_node_complete_avl(TreeComplete *tree, WordInfo *info);

/**
 * @brief Busca uma palavra na árvore completa
 * @param root Raiz da árvore
 * @param word Palavra a ser buscada
 * @return Ponteiro para o nó se encontrado, NULL caso contrário
 */
NodeComplete* search_tree_complete(NodeComplete *root, const char *word);

/**
 * @brief Realiza rotação à esquerda em árvore completa
 * @param x Nó raiz da subárvore
 * @return Nova raiz após rotação
 */
NodeComplete* left_rotate_complete(NodeComplete *x);

/**
 * @brief Realiza rotação à direita em árvore completa
 * @param y Nó raiz da subárvore
 * @return Nova raiz após rotação
 */
NodeComplete* right_rotate_complete(NodeComplete *y);

/**
 * @brief Mede o tempo de inserção de uma função
 * @param insert_func Função de inserção
 * @param data Dados para inserção
 * @return Tempo em segundos
 */
double measure_insertion_time(void (*insert_func)(void*), void *data);

/**
 * @brief Mede o tempo de busca de uma função
 * @param search_func Função de busca
 * @param root Raiz da árvore
 * @param word Palavra a ser buscada
 * @return Tempo em segundos
 */
double measure_search_time(Node* (*search_func)(Node*, const char*), Node *root, const char *word);

/**
 * @brief Árvore binária de busca global
 */
extern Tree *bin_tree;

/**
 * @brief Árvore AVL global
 */
extern Tree *avl_tree;

#endif
