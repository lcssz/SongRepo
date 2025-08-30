/**
 * @file repository.c
 * @brief Implementação do repositório de letras de músicas
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 * 
 * @details Este arquivo contém a implementação das funções do repositório,
 * incluindo processamento de arquivos, busca de palavras e interface do usuário.
 */

#include "../../include/core/repository.h"
#include "../../include/structures/word_info.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>


/**
 * @brief Converte uma string para minúsculo
 * @param str String a ser convertida
 * @return String em minúsculo ou NULL se falhar
 * @details Cria uma cópia da string original e converte todos os caracteres
 *          para minúsculo usando a função tolower() da biblioteca ctype.h.
 *          Retorna NULL se a string de entrada for NULL ou se houver falha
 *          na alocação de memória.
 */
char* to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    
    char *result = strdup(str);
    if (result == NULL) return NULL;
    
    for (int i = 0; result[i]; i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

/**
 * @brief Limpa uma palavra removendo pontuação e convertendo para minúsculo
 * @param word Palavra a ser limpa
 * @return Palavra limpa ou NULL se falhar
 * @details Remove caracteres de pontuação do início e fim da palavra,
 *          converte todos os caracteres para minúsculo e retorna uma nova string.
 *          A função usa isalpha() para identificar caracteres alfabéticos e
 *          memmove() para remover pontuação do início da string.
 * 
 * @note Esta função é essencial para o processamento de palavras do repositório,
 *       garantindo consistência na comparação e armazenamento.
 * 
 * @example
 * ```c
 * char *cleaned = clean_word("Hello, World!");
 * // Resultado: "hello world"
 * ```
 */
char* clean_word(const char *word) {
    if (word == NULL) return NULL;
    
    char *cleaned = strdup(word);
    if (cleaned == NULL) return NULL;
    

    while (*cleaned && !isalpha(*cleaned)) {
        memmove(cleaned, cleaned + 1, strlen(cleaned));
    }
    

    int len = strlen(cleaned);
    while (len > 0 && !isalpha(cleaned[len - 1])) {
        cleaned[--len] = '\0';
    }
    

    for (int i = 0; cleaned[i]; i++) {
        cleaned[i] = tolower(cleaned[i]);
    }
    
    return cleaned;
}

/**
 * @brief Processa uma linha e insere palavras nas estruturas básicas
 * @param line Linha a ser processada
 * @details Divide a linha em tokens usando strtok() e insere cada palavra
 *          válida (com 3 ou mais caracteres) nas árvores binária e AVL básicas.
 *          Esta função é usada na versão simplificada do processamento.
 * 
 * @warning Esta função termina o programa (exit) se houver falha na alocação
 *          de memória, pois é uma versão simplificada para demonstração.
 * 
 * @note As palavras são inseridas diretamente sem limpeza adicional,
 *       assumindo que já foram processadas adequadamente.
 */
void process_line_and_insert_words(const char *line) {
  char *line_copy = strdup(line);
  if (line_copy == NULL) {
    fprintf(stderr, "Erro de alocação de memória.");
    exit(EXIT_FAILURE);
  }

  char *token = strtok(line_copy, " ");

  while (token != NULL) {
    if (strlen(token) >= 3) {

      Node *bst_node = create_node(token);
      if (bst_node != NULL)
        insert_node(bst_node);

      Node *avl_node = create_node(token);
      if (avl_node != NULL)
        insert_node_avl(avl_node);
    }
    token = strtok(NULL, " ");
  }
  free(line_copy);
}

/**
 * @brief Processa arquivo de música para contagem de palavras (versão básica)
 * @param filepath Caminho para o arquivo de música
 * @details Lê um arquivo .txt contendo uma letra de música e processa cada linha,
 *          inserindo as palavras nas estruturas básicas (árvores binária e AVL).
 *          Esta é uma versão simplificada que não armazena informações completas
 *          como título, autor e trechos da música.
 * 
 * @note Esta função assume que o arquivo tem o formato:
 *       - Primeira linha: título da música
 *       - Segunda linha: autor
 *       - Linhas subsequentes: letra da música
 * 
 * @warning A função não valida extensivamente o formato do arquivo e pode
 *          falhar silenciosamente se o arquivo não existir ou estiver mal formatado.
 */
void process_music_file_for_word_count(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return;
  }
  char line_buffer[256];

  if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    process_line_and_insert_words(line_buffer);
    fclose(file);
    return;
  }

  if (fgets(line_buffer, sizeof(line_buffer), file) == NULL) {
    fclose(file);
    return;
  }

  while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    process_line_and_insert_words(line_buffer);
  }
  fclose(file);
}


/**
 * @brief Processa arquivo de música completo com todas as informações
 * @param filepath Caminho para o arquivo de música
 * @param repo Repositório onde armazenar os dados
 * @details Esta é a função principal de processamento que lê um arquivo .txt
 *          contendo uma letra de música e processa cada palavra, inserindo-as
 *          nas três estruturas de dados do repositório (vetor, árvore binária
 *          e árvore AVL) com informações completas.
 * 
 *          A função:
 *          - Lê o título e autor da música
 *          - Processa cada linha da letra
 *          - Limpa cada palavra (remove pontuação, converte para minúsculo)
 *          - Filtra palavras com menos de 4 caracteres
 *          - Cria estruturas WordInfo com informações completas
 *          - Insere nas três estruturas de dados
 * 
 * @note Formato esperado do arquivo:
 *       - Linha 1: Título da música
 *       - Linha 2: Autor da música
 *       - Linhas 3+: Letra da música
 * 
 * @note Complexidade: O(n * log m) onde n é o número de palavras e m é o
 *       tamanho das estruturas de dados.
 * 
 * @warning A função assume que o arquivo está bem formatado e pode falhar
 *          silenciosamente se o arquivo não existir ou estiver mal formatado.
 */


void process_music_file_complete(const char *filepath, Repository *repo) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    
    char line_buffer[1024];
    char title[256] = "";
    char author[256] = "";
    char current_verse[1024] = "";
    
    typedef struct {
        char *word;
        int count;
        char *verse_snippet;
    } WordCount;
    
    WordCount *word_counts = NULL;
    int word_count_size = 0;
    int word_count_capacity = 0;
    

    if (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        strcpy(title, line_buffer);
    }
    

    if (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        strcpy(author, line_buffer);
    }
    
    fgets(line_buffer, sizeof(line_buffer), file);
    
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';
        strcpy(current_verse, line_buffer);
        
        char *line_copy = strdup(line_buffer);
        if (line_copy == NULL) continue;
        
        char *token = strtok(line_copy, " \t\n");
        while (token != NULL) {
            char *cleaned_word = clean_word(token);
            if (cleaned_word != NULL && strlen(cleaned_word) >= 4) {
                
                int found = 0;
                for (int i = 0; i < word_count_size; i++) {
                    if (strcmp(word_counts[i].word, cleaned_word) == 0) {
                        word_counts[i].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (word_count_size >= word_count_capacity) {
                        word_count_capacity = word_count_capacity == 0 ? 10 : word_count_capacity * 2;
                        WordCount *new_counts = realloc(word_counts, word_count_capacity * sizeof(WordCount));
                        if (new_counts == NULL) {
                            free(cleaned_word);
                            free(line_copy);
                            continue;
                        }
                        word_counts = new_counts;
                    }
                    
                    word_counts[word_count_size].word = strdup(cleaned_word);
                    word_counts[word_count_size].count = 1;
                    word_counts[word_count_size].verse_snippet = strdup(current_verse);
                    word_count_size++;
                }
            }
            free(cleaned_word);
            token = strtok(NULL, " \t\n");
        }
        free(line_copy);
    }
    
    for (int i = 0; i < word_count_size; i++) {
        WordInfo *info = create_word_info(word_counts[i].word);
        if (info != NULL) {
            add_song_to_word(info, title, author, word_counts[i].verse_snippet, word_counts[i].count);
            
            insert_word_vector(repo->word_vector, word_counts[i].word, title, author, word_counts[i].verse_snippet, word_counts[i].count);
            insert_node_complete_bst(repo->bst_complete, info);
            insert_node_complete_avl(repo->avl_complete, info);
        }
        
        free(word_counts[i].word);
        free(word_counts[i].verse_snippet);
    }
    
    free(word_counts);
    fclose(file);
}


/**
 * @brief Busca uma palavra em todas as estruturas de dados
 * @param repo Repositório onde buscar
 * @param word Palavra a ser buscada
 * @details Realiza a busca da palavra especificada em todas as três estruturas
 *          de dados (vetor, árvore binária e árvore AVL) e mede o tempo de
 *          execução de cada busca. Os resultados são exibidos com os tempos
 *          de performance para comparação.
 * 
 *          A função:
 *          - Limpa a palavra de entrada (remove pontuação, converte para minúsculo)
 *          - Executa busca no vetor usando busca binária
 *          - Executa busca na árvore binária de busca
 *          - Executa busca na árvore AVL
 *          - Mede o tempo de cada operação usando clock()
 *          - Exibe os resultados formatados
 * 
 * @note Complexidade das buscas:
 *       - Vetor (busca binária): O(log n)
 *       - Árvore Binária: O(log n) no melhor caso, O(n) no pior caso
 *       - Árvore AVL: O(log n) garantido
 * 
 * @note A função assume que o vetor está ordenado para busca binária.
 *       Se não estiver ordenado, a busca no vetor pode retornar resultados
 *       incorretos.
 * 
 * @warning A função não valida se o repositório foi inicializado corretamente
 *          ou se as estruturas de dados estão em um estado válido.
 */
void search_word_in_all_structures(Repository *repo, const char *word) {
    if (repo == NULL || word == NULL) return;
    
    char *cleaned_word = clean_word(word);
    if (cleaned_word == NULL) return;
    
    clock_t start, end;
    double vector_time, bst_time, avl_time;
    
    printf("\n=== Resultados da busca para '%s' ===\n", cleaned_word);
    
    printf("\n1. Vetor (Busca Binária):\n");
    start = clock();
    WordInfo *vector_result = search_word_vector(repo->word_vector, cleaned_word);
    end = clock();
    vector_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos\n", vector_time);
    
    if (vector_result != NULL) {
        printf("Palavra encontrada!\n");
        printf("Frequência total: %d\n", vector_result->total_frequency);
        printf("Número de músicas: %d\n", vector_result->num_songs);
        printf("---\n");
        
        printf("Músicas onde aparece:\n");
        for (int i = 0; i < vector_result->num_songs; i++) {
            printf("%d. Música: %s\n", i + 1, vector_result->songs[i].title);
            printf("   Autor: %s\n", vector_result->songs[i].author);
            printf("   Trecho: %s\n", vector_result->songs[i].verse_snippet);
            printf("   Frequência nesta música: %d\n", vector_result->songs[i].frequency_in_song);
            printf("   ---\n");
        }
    } else {
        printf("Palavra não encontrada.\n");
    }
    
    printf("\n2. Árvore Binária de Busca:\n");
    start = clock();
    NodeComplete *bst_result = search_tree_complete(repo->bst_complete->root, cleaned_word);
    end = clock();
    bst_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos\n", bst_time);
    
    if (bst_result != NULL && bst_result->info != NULL) {
        printf("Palavra encontrada!\n");
        printf("Frequência total: %d\n", bst_result->info->total_frequency);
        printf("Número de músicas: %d\n", bst_result->info->num_songs);
        printf("---\n");
        
        printf("Músicas onde aparece:\n");
        for (int i = 0; i < bst_result->info->num_songs; i++) {
            printf("%d. Música: %s\n", i + 1, bst_result->info->songs[i].title);
            printf("   Autor: %s\n", bst_result->info->songs[i].author);
            printf("   Trecho: %s\n", bst_result->info->songs[i].verse_snippet);
            printf("   Frequência nesta música: %d\n", bst_result->info->songs[i].frequency_in_song);
            printf("   ---\n");
        }
    } else {
        printf("Palavra não encontrada.\n");
    }
    
    printf("\n3. Árvore AVL:\n");
    start = clock();
    NodeComplete *avl_result = search_tree_complete(repo->avl_complete->root, cleaned_word);
    end = clock();
    avl_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos\n", avl_time);
    
    if (avl_result != NULL && avl_result->info != NULL) {
        printf("Palavra encontrada!\n");
        printf("Frequência total: %d\n", avl_result->info->total_frequency);
        printf("Número de músicas: %d\n", avl_result->info->num_songs);
        printf("---\n");
        
        printf("Músicas onde aparece:\n");
        for (int i = 0; i < avl_result->info->num_songs; i++) {
            printf("%d. Música: %s\n", i + 1, avl_result->info->songs[i].title);
            printf("   Autor: %s\n", avl_result->info->songs[i].author);
            printf("   Trecho: %s\n", avl_result->info->songs[i].verse_snippet);
            printf("   Frequência nesta música: %d\n", avl_result->info->songs[i].frequency_in_song);
            printf("   ---\n");
        }
    } else {
        printf("Palavra não encontrada.\n");
    }
    
    free(cleaned_word);
}

/**
 * @brief Busca palavras por frequência mínima
 * @param repo Repositório onde buscar
 * @param min_frequency Frequência mínima desejada
 * @details Lista todas as palavras que aparecem com frequência igual ou superior
 *          à especificada. Útil para identificar as palavras mais recorrentes
 *          no repositório de letras.
 * 
 *          A função:
 *          - Percorre o vetor de palavras
 *          - Filtra palavras com frequência total >= min_frequency
 *          - Exibe informações completas de cada palavra encontrada
 * 
 * @note Esta função usa apenas o vetor para busca, pois é mais eficiente
 *       para percorrer todos os elementos. As árvores seriam mais úteis
 *       para busca por palavra específica.
 * 
 * @note Complexidade: O(n) onde n é o número de palavras no vetor.
 * 
 * @example
 * ```c
 * search_by_frequency(repo, 5);  // Mostra palavras que aparecem 5+ vezes
 * ```
 * 
 * @warning A função assume que o repositório foi inicializado corretamente
 *          e que o vetor contém dados válidos.
 */
void search_by_frequency_bst(NodeComplete *root, int min_frequency, int *count) {
    if (root == NULL) return;
    
    search_by_frequency_bst(root->left, min_frequency, count);
    
    if (root->info->total_frequency >= min_frequency) {
        print_word_info(root->info);
        (*count)++;
    }
    
    search_by_frequency_bst(root->right, min_frequency, count);
}

void search_by_frequency_avl(NodeComplete *root, int min_frequency, int *count) {
    if (root == NULL) return;
    
    search_by_frequency_avl(root->left, min_frequency, count);
    
    if (root->info->total_frequency >= min_frequency) {
        print_word_info(root->info);
        (*count)++;
    }
    
    search_by_frequency_avl(root->right, min_frequency, count);
}

void load_all_files_from_directory(const char *directory_path, Repository *repo) {
    if (repo == NULL || directory_path == NULL) return;
    
    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        printf("Erro ao abrir diretório: %s\n", directory_path);
        return;
    }
    
    struct dirent *entry;
    int files_loaded = 0;
    int total_files = 0;
    
    printf("Carregando arquivos do diretório: %s\n", directory_path);
    
    while ((entry = readdir(dir)) != NULL) {
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory_path, entry->d_name);
        
        if (strstr(entry->d_name, ".txt") != NULL) {
            printf("Processando: %s\n", entry->d_name);
            process_music_file_complete(filepath, repo);
            files_loaded++;
        }
        total_files++;
    }
    
    closedir(dir);
    
    printf("Carregamento concluído!\n");
    printf("Arquivos processados: %d/%d\n", files_loaded, total_files);
}

void search_by_frequency(Repository *repo, int min_frequency) {
    if (repo == NULL) return;
    
    printf("\n=== Palavras com frequência >= %d ===\n", min_frequency);
    
    clock_t start, end;
    double vector_time, bst_time, avl_time;
    int vector_count = 0, bst_count = 0, avl_count = 0;
    
    printf("\n1. Busca no Vetor:\n");
    start = clock();
    for (int i = 0; i < repo->word_vector->size; i++) {
        if (repo->word_vector->words[i].total_frequency >= min_frequency) {
            print_word_info(&repo->word_vector->words[i]);
            vector_count++;
        }
    }
    end = clock();
    vector_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos | Palavras encontradas: %d\n", vector_time, vector_count);
    
    printf("\n2. Busca na Árvore Binária:\n");
    start = clock();
    search_by_frequency_bst(repo->bst_complete->root, min_frequency, &bst_count);
    end = clock();
    bst_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos | Palavras encontradas: %d\n", bst_time, bst_count);
    
    printf("\n3. Busca na Árvore AVL:\n");
    start = clock();
    search_by_frequency_avl(repo->avl_complete->root, min_frequency, &avl_count);
    end = clock();
    avl_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo: %.6f segundos | Palavras encontradas: %d\n", avl_time, avl_count);
    
    printf("\n=== Resumo da Performance ===\n");
    printf("Vetor: %.6f segundos (%d palavras)\n", vector_time, vector_count);
    printf("Árvore Binária: %.6f segundos (%d palavras)\n", bst_time, bst_count);
    printf("Árvore AVL: %.6f segundos (%d palavras)\n", avl_time, avl_count);
}


/**
 * @brief Imprime informações de uma palavra
 * @param info Informações da palavra a ser impressa
 * @details Exibe todas as informações armazenadas em uma estrutura WordInfo,
 *          incluindo a palavra, título da música, autor, trecho da estrofe,
 *          frequência na música e frequência total no repositório.
 * 
 *          A função:
 *          - Verifica se o ponteiro é válido
 *          - Exibe as informações em formato tabular
 *          - Adiciona separadores para melhor legibilidade
 * 
 * @note Se info for NULL, exibe uma mensagem de "Palavra não encontrada".
 * 
 * @example
 * ```
 * Palavra: love
 * Música: Bohemian Rhapsody
 * Autor: Queen
 * Trecho: Is this the real life?
 * Frequência na música: 2
 * Frequência total: 5
 * ---
 * ```
 */
void print_word_info(WordInfo *info) {
    if (info == NULL) {
        printf("Palavra não encontrada.\n");
        return;
    }
    
    printf("Palavra: %s\n", info->word);
    printf("Frequência total: %d\n", info->total_frequency);
    printf("Número de músicas: %d\n", info->num_songs);
    printf("---\n");
    
    for (int i = 0; i < info->num_songs; i++) {
        printf("Música %d: %s\n", i + 1, info->songs[i].title);
        printf("Autor: %s\n", info->songs[i].author);
        printf("Trecho: %s\n", info->songs[i].verse_snippet);
        printf("Frequência nesta música: %d\n", info->songs[i].frequency_in_song);
        printf("---\n");
    }
}

/**
 * @brief Imprime resultados de busca em todas as estruturas
 * @param word Palavra buscada
 * @param vector_result Resultado no vetor
 * @param bst_result Resultado na árvore binária
 * @param avl_result Resultado na árvore AVL
 * @param vector_time Tempo de busca no vetor
 * @param bst_time Tempo de busca na árvore binária
 * @param avl_time Tempo de busca na árvore AVL
 * @details Exibe os resultados de busca de uma palavra em todas as três
 *          estruturas de dados, incluindo os tempos de execução para
 *          comparação de performance.
 * 
 *          A função:
 *          - Exibe um cabeçalho com a palavra buscada
 *          - Para cada estrutura, mostra o tempo de busca
 *          - Exibe as informações da palavra se encontrada
 *          - Mostra mensagem se a palavra não for encontrada
 * 
 * @note Os tempos são exibidos em segundos com precisão de 6 casas decimais.
 * 
 * @note Esta função é chamada por search_word_in_all_structures() para
 *       exibir os resultados de forma organizada e comparativa.
 * 
 * @example
 * ```
 * === Resultados da busca para 'love' ===
 * 
 * 1. Vetor (Busca Binária):
 * Tempo: 0.000123 segundos
 * Palavra: love
 * Música: Bohemian Rhapsody
 * ...
 * 
 * 2. Árvore Binária de Busca:
 * Tempo: 0.000145 segundos
 * ...
 * 
 * 3. Árvore AVL:
 * Tempo: 0.000134 segundos
 * ...
 * ```
 */
void print_search_results(const char *word, WordInfo *vector_result, 
                         WordInfo *bst_result, WordInfo *avl_result,
                         double vector_time, double bst_time, double avl_time) {
    printf("\n=== Resultados da busca para '%s' ===\n", word);
    
    printf("\n1. Vetor (Busca Binária):\n");
    printf("Tempo: %.6f segundos\n", vector_time);
    if (vector_result) {
        print_word_info(vector_result);
    } else {
        printf("Palavra não encontrada.\n");
    }
    
    printf("\n2. Árvore Binária de Busca:\n");
    printf("Tempo: %.6f segundos\n", bst_time);
    if (bst_result) {
        print_word_info(bst_result);
    } else {
        printf("Palavra não encontrada.\n");
    }
    
    printf("\n3. Árvore AVL:\n");
    printf("Tempo: %.6f segundos\n", avl_time);
    if (avl_result) {
        print_word_info(avl_result);
    } else {
        printf("Palavra não encontrada.\n");
    }
}


/**
 * @brief Exibe o menu principal do sistema
 * @details Apresenta as opções disponíveis no sistema de repositório de
 *          letras de músicas, incluindo carregamento de arquivos, busca
 *          de palavras e busca por frequência.
 * 
 *          O menu inclui:
 *          - Opção 1: Carregar arquivos .txt
 *          - Opção 2: Buscar palavra
 *          - Opção 3: Buscar por frequência
 *          - Opção 4: Sair
 * 
 * @note Esta função é chamada repetidamente no loop principal do programa
 *       para manter a interface do usuário ativa.
 * 
 * @example
 * ```
 * === REPOSITÓRIO DE LETRAS DE MÚSICAS ===
 * 1. Carregar arquivos .txt
 * 2. Buscar palavra
 * 3. Buscar por frequência
 * 4. Sair
 * Escolha uma opção:
 * ```
 */
void clear_screen() {
    system("cls");
}

void show_menu() {
    printf("\n=== REPOSITÓRIO DE LETRAS DE MÚSICAS ===\n");
    printf("1. Carregar arquivos .txt\n");
    printf("2. Buscar palavra\n");
    printf("3. Buscar por frequência\n");
    printf("4. Limpar tela\n");
    printf("5. Sair\n");
    printf("Escolha uma opção: ");
}

/**
 * @brief Menu para carregar arquivos de música
 * @param repo Repositório onde carregar os arquivos
 * @details Interface do usuário para carregar arquivos .txt contendo letras
 *          de músicas no repositório. A função solicita o nome do arquivo,
 *          processa o arquivo e exibe o tempo de carregamento.
 * 
 *          A função:
 *          - Solicita o nome do arquivo ao usuário
 *          - Mede o tempo de processamento usando clock()
 *          - Chama process_music_file_complete() para processar o arquivo
 *          - Exibe o tempo total de carregamento
 *          - Ordena o vetor para busca binária
 * 
 * @note O arquivo deve estar no formato correto:
 *       - Linha 1: Título da música
 *       - Linha 2: Autor da música
 *       - Linhas 3+: Letra da música
 * 
 * @note Após o carregamento, o vetor é automaticamente ordenado para
 *       permitir busca binária eficiente.
 * 
 * @warning A função usa scanf() sem validação de tamanho, o que pode
 *          causar buffer overflow se o nome do arquivo for muito longo.
 * 
 * @example
 * ```
 * Digite o nome do arquivo .txt: exemplo_musica.txt
 * Arquivo carregado em 0.123456 segundos
 * Vetor ordenado para busca binária.
 * ```
 */
void load_files_menu(Repository *repo) {
    int choice;
    printf("\n=== Carregamento de Arquivos ===\n");
    printf("1. Carregar arquivo individual\n");
    printf("2. Carregar todos os arquivos de uma pasta\n");
    printf("Escolha uma opção: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        char filepath[256];
        printf("Digite o caminho do arquivo: ");
        scanf("%s", filepath);
        
        clock_t start = clock();
        process_music_file_complete(filepath, repo);
        clock_t end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Arquivo carregado em %.6f segundos\n", time_taken);
        
        sort_vector(repo->word_vector);
        printf("Vetor ordenado para busca binária.\n");
    } else if (choice == 2) {
        char directory_path[256];
        printf("Digite o caminho da pasta: ");
        scanf("%s", directory_path);
        
        clock_t start = clock();
        load_all_files_from_directory(directory_path, repo);
        clock_t end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Tempo total de processamento: %.6f segundos\n", time_taken);
        
        sort_vector(repo->word_vector);
        printf("Vetor ordenado para busca binária.\n");
    } else {
        printf("Opção inválida!\n");
    }
}

/**
 * @brief Menu para buscar palavras
 * @param repo Repositório onde buscar
 * @details Interface do usuário para buscar uma palavra específica em todas
 *          as estruturas de dados do repositório. A função solicita a palavra
 *          ao usuário e executa a busca comparativa.
 * 
 *          A função:
 *          - Solicita a palavra a ser buscada
 *          - Chama search_word_in_all_structures() para executar a busca
 *          - Exibe os resultados comparativos das três estruturas
 * 
 * @note A palavra será automaticamente limpa (remove pontuação, converte
 *       para minúsculo) antes da busca.
 * 
 * @warning A função usa scanf() sem validação de tamanho, o que pode
 *          causar buffer overflow se a palavra for muito longa.
 * 
 * @example
 * ```
 * Digite a palavra a ser buscada: love
 * === Resultados da busca para 'love' ===
 * 1. Vetor (Busca Binária):
 * Tempo: 0.000123 segundos
 * ...
 * ```
 */
void search_word_menu(Repository *repo) {
    char word[100];
    printf("Digite a palavra a ser buscada: ");
    scanf("%s", word);
    
    search_word_in_all_structures(repo, word);
}

/**
 * @brief Menu para buscar por frequência
 * @param repo Repositório onde buscar
 * @details Interface do usuário para buscar palavras que aparecem com uma
 *          frequência mínima especificada no repositório. A função solicita
 *          a frequência mínima e lista todas as palavras que atendem ao critério.
 * 
 *          A função:
 *          - Solicita a frequência mínima ao usuário
 *          - Chama search_by_frequency() para executar a busca
 *          - Exibe todas as palavras que aparecem com frequência >= especificada
 * 
 * @note Esta funcionalidade é útil para identificar as palavras mais
 *       recorrentes no repositório de letras.
 * 
 * @warning A função usa scanf() sem validação de entrada, o que pode
 *          causar problemas se o usuário inserir valores inválidos.
 * 
 * @example
 * ```
 * Digite a frequência mínima: 3
 * === Palavras com frequência >= 3 ===
 * Busca no vetor:
 * Palavra: love
 * Música: Bohemian Rhapsody
 * ...
 * ```
 */
void search_frequency_menu(Repository *repo) {
    int frequency;
    printf("Digite a frequência mínima: ");
    scanf("%d", &frequency);
    
    search_by_frequency(repo, frequency);
}


/**
 * @brief Cria um novo repositório
 * @return Ponteiro para o repositório criado ou NULL se falhar
 * @details Aloca e inicializa uma nova estrutura Repository com todas as
 *          estruturas de dados necessárias para o sistema de repositório
 *          de letras de músicas.
 * 
 *          A função:
 *          - Aloca memória para a estrutura Repository
 *          - Cria um vetor dinâmico com capacidade inicial de 100 elementos
 *          - Aloca estruturas para árvores binárias completas (BST e AVL)
 *          - Inicializa ponteiros de raiz como NULL
 *          - Inicializa contadores de músicas
 * 
 * @note Se qualquer alocação falhar, a função retorna NULL e o programa
 *       deve tratar adequadamente esse caso.
 * 
 * @note O vetor é criado com capacidade inicial de 100 elementos, mas pode
 *       ser expandido dinamicamente conforme necessário.
 * 
 * @warning É responsabilidade do chamador verificar se o retorno é NULL
 *          e tratar adequadamente falhas de alocação.
 * 
 * @example
 * ```c
 * Repository *repo = create_repository();
 * if (repo == NULL) {
 *     fprintf(stderr, "Erro ao criar repositório.\n");
 *     return 1;
 * }
 * ```
 */
Repository* create_repository() {
    Repository *repo = (Repository*)malloc(sizeof(Repository));
    if (repo == NULL) return NULL;
    
    repo->word_vector = create_vector(100);
    repo->bst_complete = (TreeComplete*)malloc(sizeof(TreeComplete));
    repo->avl_complete = (TreeComplete*)malloc(sizeof(TreeComplete));
    
    if (repo->bst_complete) repo->bst_complete->root = NULL;
    if (repo->avl_complete) repo->avl_complete->root = NULL;
    
    repo->songs = NULL;
    repo->num_songs = 0;
    
    return repo;
}

/**
 * @brief Libera a memória de um repositório
 * @param repo Repositório a ser liberado
 * @details Libera toda a memória alocada para um repositório, incluindo
 *          todas as estruturas de dados internas e seus conteúdos.
 * 
 *          A função:
 *          - Verifica se o ponteiro é válido (não NULL)
 *          - Libera o vetor dinâmico e todos os seus elementos
 *          - Libera as estruturas das árvores binárias
 *          - Libera a estrutura principal do repositório
 * 
 * @note A função é segura para chamadas com ponteiro NULL (não faz nada).
 * 
 * @note Esta função deve ser chamada antes do programa terminar para
 *       evitar vazamentos de memória.
 * 
 * @warning Após chamar esta função, o ponteiro repo se torna inválido
 *          e não deve ser usado novamente.
 * 
 * @example
 * ```c
 * Repository *repo = create_repository();
 * // ... usar o repositório ...
 * free_repository(repo);
 * // repo agora é inválido
 * ```
 */
void free_repository(Repository *repo) {
    if (repo == NULL) return;
    
    free_vector(repo->word_vector);

    free(repo->bst_complete);
    free(repo->avl_complete);
    free(repo);
}
