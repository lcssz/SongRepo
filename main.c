/**
 * @file main.c
 * @brief Ponto de entrada principal para o repositório de músicas.
 *
 * Este arquivo contém o menu principal e a lógica para interagir com o usuário,
 * permitindo carregar arquivos de música, buscar palavras e buscar por
 * frequência.
 */

#include "include/repository.h"
#include "include/structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Exibe as informações de um nó da árvore.
 * @param node O nó a ser exibido.
 */
void display_word_info(Node *node) {
  if (node == NULL) {
    printf("Palavra não encontrada.\n");
    return;
  }
  printf("Palavra: %s\n", node->word);
  printf("Total de ocorrências no repositório: %u\n", node->total_word_count);
  if (node->best_song_occurrence != NULL) {
    printf("  Melhor música: %s\n", node->best_song_occurrence->title);
    printf("  Autor: %s\n", node->best_song_occurrence->author);
    printf("  Trecho do verso: %s\n",
           node->best_song_occurrence->verse_snippet);
    printf("  Ocorrências na música: %u\n",
           node->best_song_occurrence->word_count_in_song);
  }
}

/**
 * @brief Constrói uma árvore AVL de frequência a partir da árvore AVL
 * principal.
 * @param root A raiz da árvore AVL principal.
 */
void build_frequency_avl_tree_from_main_avl(Node *root) {
  if (avl_frequency_tree != NULL) {
    free_frequency_tree(avl_frequency_tree->root);
    free(avl_frequency_tree);
    avl_frequency_tree = NULL;
  }

  avl_frequency_tree = (Tree *)malloc(sizeof(Tree));
  if (avl_frequency_tree == NULL) {
    fprintf(stderr, "Falha na alocação de memória para avl_frequency_tree.\n");
    exit(EXIT_FAILURE);
  }
  avl_frequency_tree->root = NULL;

  populate_frequency_avl_tree(root);
}

/**
 * @brief Função principal que executa o menu do programa.
 * @return 0 se o programa for executado com sucesso, 1 caso contrário.
 */
int main() {
  initialize_tree(bin_tree);
  initialize_tree(avl_tree);

  int choice;
  char filepath[256];
  char search_word[256];
  unsigned int search_frequency;
  clock_t start_time, end_time;
  double cpu_time_used;
  Node *found_node;
  bool has_file = false;

  do {
    printf("\n--- Menu do Repositório de Músicas ---\n");
    printf("1. Carregar arquivo de música\n");
    printf("2. Buscar palavra\n");
    printf("3. Buscar por frequência\n");
    printf("0. Sair\n");
    printf("Digite sua escolha: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      printf("Digite o caminho para o arquivo de música: ");
      scanf("%s", filepath);
      start_time = clock();
      process_music_file_for_word_count(filepath, NULL, NULL);
      end_time = clock();
      cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
      printf("Arquivo carregado. Tempo decorrido: %f segundos\n",
             cpu_time_used);

      if (sorted_word_array != NULL) {
        free_word_array(sorted_word_array);
      }
      sorted_word_array = create_word_array();
      populate_array_from_tree(bin_tree->root, sorted_word_array);
      sort_word_array(sorted_word_array);
      build_frequency_avl_tree_from_main_avl(avl_tree->root);
      has_file = true;
      break;
    case 2:
      if (!has_file) {
        printf("Erro: Nenhum arquivo de música foi carregado ainda. Por favor, "
               "use a opção 1 primeiro.\n");
        break;
      }
      printf("Digite a palavra para buscar: ");
      scanf("%s", search_word);

      // Busca na BST
      start_time = clock();
      found_node = search_bst(bin_tree->root, search_word);
      end_time = clock();
      cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
      printf("\n--- Resultado da Busca na BST ---\n");
      display_word_info(found_node);
      printf("Tempo decorrido: %f segundos\n", cpu_time_used);

      // Busca na AVL
      start_time = clock();
      found_node = search_avl(avl_tree->root, search_word);
      end_time = clock();
      cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
      printf("\n--- Resultado da Busca na AVL ---\n");
      display_word_info(found_node);
      printf("Tempo decorrido: %f segundos\n", cpu_time_used);

      // Busca no Array
      start_time = clock();
      found_node = binary_search_array(sorted_word_array, search_word);
      end_time = clock();
      cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
      printf("\n--- Resultado da Busca Binária (Array) ---\n");
      display_word_info(found_node);
      printf("Tempo decorrido: %f segundos\n", cpu_time_used);
      break;
    case 3:
      if (!has_file) {
        printf("Erro: Nenhum arquivo de música foi carregado ainda. Por favor, "
               "use a opção 1 primeiro.\n");
        break;
      }
      printf("Digite a frequência mínima para buscar: ");
      scanf("%u", &search_frequency);

      start_time = clock();
      WordArray *frequency_results =
          search_all_by_frequency(avl_frequency_tree->root, search_frequency);
      end_time = clock();
      cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

      printf("\n--- Resultado da Busca por Frequência Mínima ---\n");
      if (frequency_results->size == 0) {
        printf("Nenhuma palavra encontrada com frequência >= %u\n",
               search_frequency);
      } else {
        printf("Encontrada(s) %d palavra(s) com frequência >= %u:\n\n",
               frequency_results->size, search_frequency);
        for (size_t i = 0; i < frequency_results->size; i++) {
          printf("Palavra %zu:\n", i + 1);
          display_word_info(frequency_results->nodes[i]);
          printf("\n");
        }
      }
      printf("Tempo decorrido: %f segundos\n", cpu_time_used);

      free(frequency_results->nodes);
      free(frequency_results);
      break;
    case 0:
      printf("Saindo do programa.\n");
      break;
    default:
      printf("Escolha inválida. Por favor, tente novamente.\n");
    }
  } while (choice != 0);

  if (has_file) {
    free_tree(bin_tree->root);
    free(bin_tree);
    free_tree(avl_tree->root);
    free(avl_tree);
    free_word_array(sorted_word_array);

    if (avl_frequency_tree != NULL) {
      free_frequency_tree(avl_frequency_tree->root);
      free(avl_frequency_tree);
    }
  }

  return 0;
}
