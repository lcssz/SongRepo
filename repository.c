#include "include/repository.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_line_and_insert_words(const char *line) {
  char *line_copy = strdup(line);
  if (line_copy == NULL) {
    fprintf(stderr, "Erro de alocação de memória.");
    exit(EXIT_FAILURE);
  }

  char *token = strtok(line_copy, " ");

  while (token != NULL) {
    if (strlen(token) >= 3) {
      // Cria e insere o Node para árvore binária.
      Node *bst_node = create_node(token);
      if (bst_node != NULL)
        insert_node(bst_node);
      // Cria e insere o Node para árvore avl.
      Node *avl_node = create_node(token);
      if (avl_node != NULL)
        insert_node_avl(avl_node);
    }
    token = strtok(NULL, " ");
  }
  free(line_copy);
}

void process_music_file_for_word_count(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }
  char line_buffer[256];
  // Processa o nome música.
  if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    process_line_and_insert_words(line_buffer);
    fclose(file);
    return;
  }
  // Ignora o nome do cantor
  if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) {
    fclose(file);
    return;
  }
  // Processa a letra da música
  while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    process_line_and_insert_words(line_buffer);
  }
  fclose(file);
}
