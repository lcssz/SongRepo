/**
 * @file repository.c
 * @brief Implementação das funções de processamento de arquivos de música.
 *
 * Este arquivo contém a lógica para ler, analisar e extrair palavras
 * e metadados de arquivos de texto de músicas.
 */

#include "include/repository.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Converte uma string para minúsculas.
 * @param s A string a ser convertida.
 */
void to_lowercase(char *s) {
  for (char *p = s; *p; p++) {
    *p = tolower(*p);
  }
}

/**
 * @brief Remove a pontuação de uma string.
 * @param s A string da qual a pontuação será removida.
 */
void remove_punctuation(char *s) {
  char *p = s, *q = s;
  while (*q) {
    if (isalnum(*q)) {
      *p++ = *q;
    }
    q++;
  }
  *p = '\0';
}

WordCount *find_or_create_word_count(WordCount **head, const char *word) {
  WordCount *current = *head;
  while (current != NULL) {
    if (strcmp(current->word, word) == 0) {
      return current;
    }
    current = current->next;
  }

  WordCount *new_count = (WordCount *)malloc(sizeof(WordCount));
  if (new_count == NULL) {
    fprintf(stderr, "Falha na alocação de memória para WordCount.\n");
    exit(EXIT_FAILURE);
  }

  new_count->word = strdup(word);
  new_count->count = 0;
  new_count->next = *head;
  *head = new_count;
  return new_count;
}

void free_word_count_list(WordCount *head) {
  while (head != NULL) {
    WordCount *temp = head;
    head = head->next;
    free(temp->word);
    free(temp);
  }
}

char *find_verse_snippet(FILE *file, const char *word, char *snippet_buffer,
                         size_t buffer_size) {
  char line[256];
  long current_pos = ftell(file);

  fseek(file, 0, SEEK_SET);
  fgets(line, sizeof(line), file);
  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file) != NULL) {
    char line_copy[256];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    char *token = strtok(line_copy, " \t\n\r");
    while (token != NULL) {
      char word_copy[256];
      strncpy(word_copy, token, sizeof(word_copy) - 1);
      word_copy[sizeof(word_copy) - 1] = '\0';
      remove_punctuation(word_copy);
      to_lowercase(word_copy);

      if (strcmp(word_copy, word) == 0) {
        line[strcspn(line, "\n")] = '\0';
        strncpy(snippet_buffer, line, buffer_size - 1);
        snippet_buffer[buffer_size - 1] = '\0';
        fseek(file, current_pos, SEEK_SET);
        return snippet_buffer;
      }
      token = strtok(NULL, " \t\n\r");
    }
  }

  fseek(file, current_pos, SEEK_SET);
  strncpy(snippet_buffer, "Verso não encontrado", buffer_size - 1);
  snippet_buffer[buffer_size - 1] = '\0';
  return snippet_buffer;
}

void process_music_file_for_word_count(const char *filepath, const char *title,
                                       const char *author) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  char line_buffer[256];
  char song_title[256] = "";
  char song_author[256] = "";
  WordCount *word_counts = NULL;

  if (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    strncpy(song_title, line_buffer, sizeof(song_title) - 1);
    song_title[sizeof(song_title) - 1] = '\0';
  }

  if (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    strncpy(song_author, line_buffer, sizeof(song_author) - 1);
    song_author[sizeof(song_author) - 1] = '\0';
  }

  while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    char *line_copy = strdup(line_buffer);
    if (line_copy == NULL) {
      fprintf(stderr, "Erro de alocação de memória.\n");
      exit(EXIT_FAILURE);
    }
    char *token = strtok(line_copy, " \t\n\r");
    while (token != NULL) {
      char word_copy[256];
      strncpy(word_copy, token, sizeof(word_copy) - 1);
      word_copy[sizeof(word_copy) - 1] = '\0';
      remove_punctuation(word_copy);
      to_lowercase(word_copy);

      if (strlen(word_copy) >= 3) {
        WordCount *wc = find_or_create_word_count(&word_counts, word_copy);
        wc->count++;
      }
      token = strtok(NULL, " \t\n\r");
    }
    free(line_copy);
  }

  WordCount *current = word_counts;
  while (current != NULL) {
    char verse_snippet[256];
    find_verse_snippet(file, current->word, verse_snippet,
                       sizeof(verse_snippet));

    Node *bst_node = create_node(current->word);
    if (bst_node != NULL) {
      bst_node->best_song_occurrence = create_song_occurrence(
          song_title, song_author, verse_snippet, current->count);
      bst_node->total_word_count = current->count;
      insert_node(bst_node);
    }

    Node *avl_node = create_node(current->word);
    if (avl_node != NULL) {
      avl_node->best_song_occurrence = create_song_occurrence(
          song_title, song_author, verse_snippet, current->count);
      avl_node->total_word_count = current->count;
      insert_node_avl(avl_node);
    }
    current = current->next;
  }

  free_word_count_list(word_counts);
  fclose(file);
}
