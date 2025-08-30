/**
 * @file word_info.h
 * @brief Definição da estrutura WordInfo para armazenar informações de palavras
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 */

#ifndef WORD_INFO_H
#define WORD_INFO_H

/**
 * @brief Estrutura para armazenar informações de uma música específica
 */
typedef struct {
  char *title;             /**< Título da música */
  char *author;            /**< Autor da música */
  char *verse_snippet;     /**< Trecho da estrofe contendo a palavra */
  int frequency_in_song;   /**< Frequência da palavra nesta música específica */
} SongInfo;

/**
 * @brief Estrutura para armazenar informações completas de uma palavra
 * 
 * Esta estrutura contém todas as informações necessárias sobre uma palavra
 * encontrada em letras de músicas, incluindo múltiplas músicas onde aparece.
 */
typedef struct {
  char *word;              /**< A palavra em si */
  SongInfo *songs;         /**< Array de músicas onde a palavra aparece */
  int num_songs;           /**< Número de músicas onde a palavra aparece */
  int capacity;            /**< Capacidade do array de músicas */
  int total_frequency;     /**< Frequência total da palavra no repositório */
} WordInfo;

/**
 * @brief Funções para gerenciar WordInfo
 */
SongInfo* create_song_info(const char *title, const char *author, const char *verse_snippet, int frequency);
void free_song_info(SongInfo *song);
WordInfo* create_word_info(const char *word);
void add_song_to_word(WordInfo *word_info, const char *title, const char *author, const char *verse_snippet, int frequency);
void free_word_info(WordInfo *word_info);

#endif
