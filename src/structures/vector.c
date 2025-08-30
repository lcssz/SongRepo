/**
 * @file vector.c
 * @brief Implementação do vetor dinâmico com busca binária
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 * 
 * @details Este arquivo contém a implementação de um vetor dinâmico que
 *          armazena estruturas WordInfo e suporta busca binária após ordenação.
 *          O vetor pode ser expandido dinamicamente conforme necessário.
 */

#include "../../include/structures/llist.h"
#include "../../include/structures/word_info.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria um novo vetor dinâmico
 * @param initial_capacity Capacidade inicial do vetor
 * @return Ponteiro para o vetor criado ou NULL se falhar
 * @details Aloca e inicializa uma nova estrutura Vector com a capacidade
 *          inicial especificada. O vetor pode ser expandido dinamicamente
 *          conforme necessário.
 * 
 *          A função:
 *          - Aloca memória para a estrutura Vector
 *          - Aloca um array de WordInfo com a capacidade inicial
 *          - Inicializa size como 0 e capacity como initial_capacity
 * 
 * @note Se qualquer alocação falhar, a função libera a memória já alocada
 *       e retorna NULL para evitar vazamentos de memória.
 * 
 * @note O vetor começa vazio (size = 0) mas com capacidade para
 *       initial_capacity elementos.
 * 
 * @warning É responsabilidade do chamador verificar se o retorno é NULL
 *          e tratar adequadamente falhas de alocação.
 * 
 * @example
 * ```c
 * Vector *vec = create_vector(100);
 * if (vec == NULL) {
 *     fprintf(stderr, "Erro ao criar vetor.\n");
 *     return 1;
 * }
 * ```
 */
Vector* create_vector(int initial_capacity) {
    Vector *vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) return NULL;
    
    vec->words = (WordInfo*)malloc(initial_capacity * sizeof(WordInfo));
    if (vec->words == NULL) {
        free(vec);
        return NULL;
    }
    
    vec->size = 0;
    vec->capacity = initial_capacity;
    return vec;
}

/**
 * @brief Expande a capacidade do vetor
 * @param vec Vetor a ser expandido
 * @details Dobra a capacidade do vetor usando realloc() para permitir
 *          a inserção de mais elementos. A nova capacidade será o dobro
 *          da capacidade atual.
 * 
 *          A função:
 *          - Calcula a nova capacidade (atual * 2)
 *          - Usa realloc() para expandir o array de WordInfo
 *          - Atualiza a capacidade se a expansão for bem-sucedida
 * 
 * @note Se realloc() falhar, a capacidade permanece inalterada e
 *       o vetor mantém sua capacidade atual.
 * 
 * @note A função é chamada automaticamente por insert_word_vector()
 *       quando o vetor está cheio.
 * 
 * @warning A função assume que vec não é NULL. O chamador deve
 *          verificar a validade do ponteiro antes de chamar esta função.
 * 
 * @example
 * ```c
 * Vector *vec = create_vector(10);
 * // ... inserir elementos até encher ...
 * expand_vector(vec);  // Capacidade agora é 20
 * ```
 */
void expand_vector(Vector *vec) {
    int new_capacity = vec->capacity * 2;
    WordInfo *new_words = (WordInfo*)realloc(vec->words, new_capacity * sizeof(WordInfo));
    if (new_words != NULL) {
        vec->words = new_words;
        vec->capacity = new_capacity;
    }
}

/**
 * @brief Insere uma palavra no vetor com suas informações
 * @param vec Vetor onde inserir
 * @param word Palavra a ser inserida
 * @param title Título da música
 * @param author Autor da música
 * @param verse_snippet Trecho da estrofe
 * @param freq_in_song Frequência na música
 * @details Insere uma nova palavra no vetor ou atualiza uma palavra existente.
 *          Se a palavra já existe, atualiza a frequência total e, se a nova
 *          ocorrência tem frequência maior na música, atualiza as informações
 *          da música associada.
 * 
 *          A função:
 *          - Verifica se a palavra já existe no vetor
 *          - Se existir, atualiza frequência total e informações se necessário
 *          - Se não existir, expande o vetor se necessário e insere novo elemento
 *          - Usa strdup() para criar cópias das strings
 * 
 * @note A função assume que o vetor não está ordenado durante a inserção.
 *       A ordenação deve ser feita separadamente com sort_vector().
 * 
 * @note Se a nova ocorrência tem frequência maior na música, as informações
 *       da música (título, autor, trecho) são atualizadas para refletir
 *       a ocorrência mais relevante.
 * 
 * @note Complexidade: O(n) para busca linear + O(1) para inserção.
 * 
 * @warning A função assume que vec não é NULL. O chamador deve verificar
 *          a validade do ponteiro antes de chamar esta função.
 * 
 * @example
 * ```c
 * insert_word_vector(vec, "love", "Bohemian Rhapsody", "Queen", 
 *                   "Is this the real life?", 2);
 * ```
 */
void insert_word_vector(Vector *vec, const char *word, const char *title, 
                       const char *author, const char *verse_snippet, int freq_in_song) {
    if (vec == NULL) return;
    

    for (int i = 0; i < vec->size; i++) {
        if (strcmp(vec->words[i].word, word) == 0) {
            add_song_to_word(&vec->words[i], title, author, verse_snippet, freq_in_song);
            return;
        }
    }
    
    if (vec->size >= vec->capacity) {
        expand_vector(vec);
    }
    
    vec->words[vec->size].word = strdup(word);
    vec->words[vec->size].songs = (SongInfo*)malloc(10 * sizeof(SongInfo));
    vec->words[vec->size].num_songs = 0;
    vec->words[vec->size].capacity = 10;
    vec->words[vec->size].total_frequency = 0;
    
    if (vec->words[vec->size].songs == NULL) {
        free(vec->words[vec->size].word);
        return;
    }
    add_song_to_word(&vec->words[vec->size], title, author, verse_snippet, freq_in_song);
    vec->size++;
}

/**
 * @brief Busca uma palavra no vetor usando busca binária
 * @param vec Vetor onde buscar
 * @param word Palavra a ser buscada
 * @return Ponteiro para WordInfo se encontrada, NULL caso contrário
 * @details Realiza busca binária no vetor para encontrar uma palavra específica.
 *          O vetor deve estar ordenado alfabeticamente para que a busca funcione
 *          corretamente.
 * 
 *          A função:
 *          - Verifica se os parâmetros são válidos
 *          - Implementa busca binária iterativa
 *          - Compara strings usando strcmp()
 *          - Retorna ponteiro para o elemento encontrado ou NULL
 * 
 * @note O vetor DEVE estar ordenado alfabeticamente antes de chamar esta função.
 *       Use sort_vector() para ordenar o vetor se necessário.
 * 
 * @note Complexidade: O(log n) onde n é o número de elementos no vetor.
 * 
 * @note A função retorna um ponteiro para o elemento no vetor, não uma cópia.
 *       O ponteiro permanece válido enquanto o vetor existir.
 * 
 * @warning Se o vetor não estiver ordenado, a busca pode retornar resultados
 *          incorretos ou não encontrar elementos que existem no vetor.
 * 
 * @example
 * ```c
 * sort_vector(vec);  // Ordenar primeiro
 * WordInfo *result = search_word_vector(vec, "love");
 * if (result != NULL) {
 *     printf("Encontrado: %s\n", result->word);
 * }
 * ```
 */
WordInfo* search_word_vector(Vector *vec, const char *word) {
    if (vec == NULL || word == NULL) return NULL;
    
    int left = 0;
    int right = vec->size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int comparison = strcmp(vec->words[mid].word, word);
        
        if (comparison == 0) {
            return &vec->words[mid];
        } else if (comparison < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return NULL;
}

/**
 * @brief Função de comparação para ordenação de palavras
 * @param a Ponteiro para o primeiro WordInfo
 * @param b Ponteiro para o segundo WordInfo
 * @return Valor negativo se a < b, 0 se a == b, valor positivo se a > b
 * @details Função de comparação usada pelo qsort() para ordenar o vetor
 *          alfabeticamente por palavra. Compara os campos 'word' das
 *          estruturas WordInfo usando strcmp().
 * 
 *          A função:
 *          - Faz cast dos ponteiros void* para WordInfo*
 *          - Compara os campos 'word' usando strcmp()
 *          - Retorna o resultado da comparação
 * 
 * @note Esta função é projetada para ser usada com qsort() da biblioteca
 *       padrão C. O qsort() espera uma função de comparação com esta
 *       assinatura específica.
 * 
 * @note A comparação é feita alfabeticamente (lexicograficamente) usando
 *       strcmp(), que é sensível a maiúsculas/minúsculas.
 * 
 * @example
 * ```c
 * qsort(vec->words, vec->size, sizeof(WordInfo), compare_words);
 * ```
 */
int compare_words(const void *a, const void *b) {
    return strcmp(((WordInfo*)a)->word, ((WordInfo*)b)->word);
}

/**
 * @brief Ordena o vetor alfabeticamente para busca binária
 * @param vec Vetor a ser ordenado
 * @details Ordena o vetor alfabeticamente por palavra usando qsort() da
 *          biblioteca padrão C. A ordenação é necessária para que a busca
 *          binária funcione corretamente.
 * 
 *          A função:
 *          - Verifica se o ponteiro é válido
 *          - Usa qsort() com compare_words() como função de comparação
 *          - Ordena todo o array de WordInfo por campo 'word'
 * 
 * @note A ordenação é feita in-place, modificando o vetor original.
 * 
 * @note Complexidade: O(n log n) onde n é o número de elementos no vetor.
 * 
 * @note Após a ordenação, o vetor pode ser usado com search_word_vector()
 *       para busca binária eficiente.
 * 
 * @warning A função assume que vec não é NULL. O chamador deve verificar
 *          a validade do ponteiro antes de chamar esta função.
 * 
 * @example
 * ```c
 * // Inserir elementos...
 * sort_vector(vec);  // Ordenar para busca binária
 * WordInfo *result = search_word_vector(vec, "love");
 * ```
 */
void sort_vector(Vector *vec) {
    if (vec == NULL) return;
    qsort(vec->words, vec->size, sizeof(WordInfo), compare_words);
}

/**
 * @brief Libera a memória de um vetor
 * @param vec Vetor a ser liberado
 * @details Libera toda a memória alocada para um vetor, incluindo todos
 *          os elementos WordInfo e suas strings associadas.
 * 
 *          A função:
 *          - Verifica se o ponteiro é válido (não NULL)
 *          - Libera todas as strings de cada WordInfo (word, title, author, verse_snippet)
 *          - Libera o array de WordInfo
 *          - Libera a estrutura Vector
 * 
 * @note A função é segura para chamadas com ponteiro NULL (não faz nada).
 * 
 * @note Esta função deve ser chamada antes do programa terminar para
 *       evitar vazamentos de memória.
 * 
 * @warning Após chamar esta função, o ponteiro vec se torna inválido
 *          e não deve ser usado novamente.
 * 
 * @example
 * ```c
 * Vector *vec = create_vector(100);
 * // ... usar o vetor ...
 * free_vector(vec);
 * // vec agora é inválido
 * ```
 */
void free_vector(Vector *vec) {
    if (vec == NULL) return;
    
    for (int i = 0; i < vec->size; i++) {
        if (vec->words[i].word) free(vec->words[i].word);
        
        for (int j = 0; j < vec->words[i].num_songs; j++) {
            if (vec->words[i].songs[j].title) free(vec->words[i].songs[j].title);
            if (vec->words[i].songs[j].author) free(vec->words[i].songs[j].author);
            if (vec->words[i].songs[j].verse_snippet) free(vec->words[i].songs[j].verse_snippet);
        }
        
        if (vec->words[i].songs) free(vec->words[i].songs);
    }
    
    free(vec->words);
    free(vec);
}
