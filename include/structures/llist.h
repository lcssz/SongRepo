/**
 * @file llist.h
 * @brief Definições para lista encadeada e vetor dinâmico
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "word_info.h"

/**
 * @brief Estrutura de nó para lista encadeada simples
 */
typedef struct ll {
  char *word;        /**< Palavra armazenada no nó */
  struct ll *next;   /**< Ponteiro para o próximo nó */
} LinkedList;

/**
 * @brief Estrutura para vetor dinâmico com busca binária
 * 
 * Implementa um vetor dinâmico que pode ser expandido conforme necessário
 * e suporta busca binária após ordenação.
 */
typedef struct {
  WordInfo *words;   /**< Array de WordInfo */
  int size;          /**< Número atual de elementos */
  int capacity;      /**< Capacidade total do vetor */
} Vector;

/**
 * @brief Cria um novo nó de lista encadeada
 * @param word Palavra a ser armazenada
 * @return Ponteiro para o novo nó ou NULL se falhar
 */
LinkedList *create_node_ll(const char *word);

/**
 * @brief Insere um nó na lista encadeada
 * @param head Ponteiro para o cabeçalho da lista
 * @param tail Ponteiro para o final da lista
 * @param new_node Nó a ser inserido
 */
void insert_node_ll(LinkedList **head, LinkedList **tail, LinkedList *new_node);

/**
 * @brief Libera a memória de uma lista encadeada
 * @param list Lista a ser liberada
 */
void free_list(LinkedList *list);

/**
 * @brief Cria um novo vetor dinâmico
 * @param initial_capacity Capacidade inicial do vetor
 * @return Ponteiro para o vetor criado ou NULL se falhar
 */
Vector* create_vector(int initial_capacity);

/**
 * @brief Insere uma palavra no vetor com suas informações
 * @param vec Vetor onde inserir
 * @param word Palavra a ser inserida
 * @param title Título da música
 * @param author Autor da música
 * @param verse_snippet Trecho da estrofe
 * @param freq_in_song Frequência na música
 */
void insert_word_vector(Vector *vec, const char *word, const char *title, 
                       const char *author, const char *verse_snippet, int freq_in_song);

/**
 * @brief Busca uma palavra no vetor usando busca binária
 * @param vec Vetor onde buscar
 * @param word Palavra a ser buscada
 * @return Ponteiro para WordInfo se encontrada, NULL caso contrário
 */
WordInfo* search_word_vector(Vector *vec, const char *word);

/**
 * @brief Ordena o vetor alfabeticamente para busca binária
 * @param vec Vetor a ser ordenado
 */
void sort_vector(Vector *vec);

/**
 * @brief Libera a memória de um vetor
 * @param vec Vetor a ser liberado
 */
void free_vector(Vector *vec);

#endif
