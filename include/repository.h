#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "structures.h"
#include <stdio.h>

/**
 * @struct WordCount
 * @brief Estrutura para contagem de palavras em uma
 * única música.
 */
typedef struct WordCount {
  char *word;             /**< A palavra. */
  unsigned int count;     /**< Contagem da palavra na música. */
  struct WordCount *next; /**< Ponteiro para o próximo WordCount. */
} WordCount;

/**
 * @struct Song
 * @brief Estrutura que representa uma música.
 */
typedef struct {
  char *title;         /**< Título da música. */
  char *author;        /**< Autor da música. */
  char **lyrics_lines; /**< Linhas da letra da música. */
  int number_of_lines; /**< Número de linhas na letra. */
} Song;

/**
 * @struct Repository
 * @brief Estrutura que representa o repositório de músicas.
 */
typedef struct {
  Node *bst_root; /**< Raiz da árvore de busca binária. */
  Node *avl_root; /**< Raiz da árvore AVL. */
  Song *songs;    /**< Array de músicas. */
  int num_songs;  /**< Número de músicas no repositório. */
} Repository;

/**
 * @brief Processa um arquivo de música, extraindo palavras e metadados.
 * @param filepath O caminho para o arquivo de música.
 * @param title O título da música (pode ser NULL).
 * @param author O autor da música (pode ser NULL).
 */
void process_music_file_for_word_count(const char *filepath, const char *title,
                                       const char *author);
/**
 * @brief Encontra ou cria uma entrada de contagem de palavras.
 * @param head Ponteiro para o início da lista de contagem de palavras.
 * @param word A palavra a ser encontrada ou criada.
 * @return Ponteiro para a entrada de contagem de palavras.
 */
WordCount *find_or_create_word_count(WordCount **head, const char *word);

/**
 * @brief Libera a lista de contagem de palavras.
 * @param head O início da lista.
 */
void free_word_count_list(WordCount *head);

/**
 * @brief Encontra um trecho de verso que contém uma palavra.
 * @param file Ponteiro para o arquivo.
 * @param word A palavra a ser encontrada.
 * @param snippet_buffer Buffer para armazenar o trecho.
 * @param buffer_size Tamanho do buffer.
 * @return Ponteiro para o buffer com o trecho do verso.
 */
char *find_verse_snippet(FILE *file, const char *word, char *snippet_buffer,
                         size_t buffer_size);
#endif // REPOSITORY_H
