/**
 * @file repository.h
 * @brief Definições para o repositório de letras de músicas
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 */

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../structures/llist.h"
#include "../structures/tree.h"
#include <stdio.h>
#include <time.h>

/**
 * @brief Estrutura para armazenar informações de uma música
 */
typedef struct {
  const char *title;           /**< Título da música */
  const char *author;          /**< Autor da música */
  const char **lyrics_lines;   /**< Linhas da letra */
  int number_of_lines;         /**< Número de linhas da letra */
} Song;

/**
 * @brief Estrutura principal do repositório
 * 
 * Contém todas as estruturas de dados utilizadas para armazenar
 * e buscar palavras nas letras de músicas.
 */
typedef struct {
  Node *bst_root;              /**< Raiz da árvore binária de busca */
  Node *avl_root;              /**< Raiz da árvore AVL */
  Vector *word_vector;         /**< Vetor com busca binária */
  TreeComplete *bst_complete;  /**< Árvore binária completa */
  TreeComplete *avl_complete;  /**< Árvore AVL completa */
  Song *songs;                 /**< Array de músicas */
  int num_songs;               /**< Número de músicas */
} Repository;

/**
 * @brief Processa arquivo de música para contagem de palavras (versão básica)
 * @param filepath Caminho para o arquivo de música
 */
void process_music_file_for_word_count(const char *filepath);

/**
 * @brief Processa uma linha e insere palavras nas estruturas básicas
 * @param line Linha a ser processada
 */
void process_line_and_insert_words(const char *line);

/**
 * @brief Processa arquivo de música completo com todas as informações
 * @param filepath Caminho para o arquivo de música
 * @param repo Repositório onde armazenar os dados
 */
void process_music_file_complete(const char *filepath, Repository *repo);
void load_all_files_from_directory(const char *directory_path, Repository *repo);

/**
 * @brief Limpa uma palavra removendo pontuação e convertendo para minúsculo
 * @param word Palavra a ser limpa
 * @return Palavra limpa ou NULL se falhar
 */
char* clean_word(const char *word);

/**
 * @brief Converte uma string para minúsculo
 * @param str String a ser convertida
 * @return String em minúsculo ou NULL se falhar
 */
char* to_lowercase(const char *str);

/**
 * @brief Busca uma palavra em todas as estruturas de dados
 * @param repo Repositório onde buscar
 * @param word Palavra a ser buscada
 */
void search_word_in_all_structures(Repository *repo, const char *word);

/**
 * @brief Busca palavras por frequência mínima
 * @param repo Repositório onde buscar
 * @param min_frequency Frequência mínima desejada
 */
void search_by_frequency_bst(NodeComplete *root, int min_frequency, int *count);
void search_by_frequency_avl(NodeComplete *root, int min_frequency, int *count);
void search_by_frequency(Repository *repo, int min_frequency);

/**
 * @brief Imprime informações de uma palavra
 * @param info Informações da palavra a ser impressa
 */
void print_word_info(WordInfo *info);

/**
 * @brief Imprime resultados de busca em todas as estruturas
 * @param word Palavra buscada
 * @param vector_result Resultado no vetor
 * @param bst_result Resultado na árvore binária
 * @param avl_result Resultado na árvore AVL
 * @param vector_time Tempo de busca no vetor
 * @param bst_time Tempo de busca na árvore binária
 * @param avl_time Tempo de busca na árvore AVL
 */
void print_search_results(const char *word, WordInfo *vector_result, 
                         WordInfo *bst_result, WordInfo *avl_result,
                         double vector_time, double bst_time, double avl_time);

/**
 * @brief Limpa a tela do terminal (Windows)
 */
void clear_screen();

/**
 * @brief Exibe o menu principal do sistema
 */
void show_menu();

/**
 * @brief Menu para carregar arquivos de música
 * @param repo Repositório onde carregar os arquivos
 */
void load_files_menu(Repository *repo);

/**
 * @brief Menu para buscar palavras
 * @param repo Repositório onde buscar
 */
void search_word_menu(Repository *repo);

/**
 * @brief Menu para buscar por frequência
 * @param repo Repositório onde buscar
 */
void search_frequency_menu(Repository *repo);

/**
 * @brief Cria um novo repositório
 * @return Ponteiro para o repositório criado ou NULL se falhar
 */
Repository* create_repository();

/**
 * @brief Libera a memória de um repositório
 * @param repo Repositório a ser liberado
 */
void free_repository(Repository *repo);

#endif
