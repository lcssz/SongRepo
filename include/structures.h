/**
 * @file tree.h
 * @brief Arquivo de cabeçalho para as estruturas e operações de árvore.
 *
 * Define as estruturas de dados para nós de árvore, ocorrências de músicas
 * e arrays de palavras, além de protótipos de funções para manipulação de
 * árvores.
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * @struct SongOccurrence
 * @brief Armazena informações sobre a ocorrência de uma palavra em uma música.
 */
typedef struct SongOccurrence {
  char *title;         /**< Título da música. */
  char *author;        /**< Autor da música. */
  char *verse_snippet; /**< Trecho do verso onde a palavra ocorre. */
  unsigned int word_count_in_song; /**< Contagem da palavra na música. */
} SongOccurrence;

/**
 * @struct node
 * @brief Estrutura de um nó em uma árvore.
 */
typedef struct Node {
  char *word; /**< A palavra armazenada no nó. */
  unsigned int
      total_word_count; /**< Contagem total da palavra no repositório. */
  unsigned int height;  /**< Altura do nó (para árvores AVL). */
  SongOccurrence *best_song_occurrence; /**< Melhor ocorrência da palavra. */
  struct Node *left;                    /**< Ponteiro para o filho esquerdo. */
  struct Node *right;                   /**< Ponteiro para o filho direito. */
} Node;

/**
 * @struct Tree
 * @brief Estrutura que representa uma árvore.
 */
typedef struct {
  Node *root; /**< Ponteiro para o nó raiz da árvore. */
} Tree;

/**
 * @struct WordArray
 * @brief Estrutura para um array dinâmico de ponteiros de nós.
 */
typedef struct {
  Node **nodes; /**< Array de ponteiros para nós. */
  int size;     /**< Número de elementos no array. */
  int capacity; /**< Capacidade atual do array. */
} WordArray;

// Variáveis globais
extern Tree *bin_tree;
extern Tree *avl_tree;
extern WordArray *sorted_word_array;
extern Tree *avl_frequency_tree;

// Macros
#define max(a, b) ((a) > (b) ? (a) : (b))
#define initialize_tree(tree_name) (tree_name = (Tree *)malloc(sizeof(Tree)))
#define height_node(N) ((N) == NULL ? 0 : (N)->height)
#define get_balance(N)                                                         \
  ((N) == NULL ? 0 : (height_node((N)->left) - height_node((N)->right)))

/**
 * @brief Cria um novo nó para ser inserido na árvore.
 *
 * Aloca memória para um novo nó e inicializa seus campos.
 * A palavra é duplicada para evitar problemas de referência.
 *
 * @param word A palavra a ser armazenada no novo nó.
 * @return Um ponteiro para o nó recém-criado.
 */
Node *create_node(const char *word);

/**
 * @brief Cria uma nova estrutura de ocorrência de música.
 *
 * Aloca memória para uma nova ocorrência de música e preenche
 * com os detalhes da música onde uma palavra específica foi encontrada.
 *
 * @param title O título da música.
 * @param author O autor da música.
 * @param verse_snippet Um trecho do verso onde a palavra ocorre.
 * @param word_count_in_song O número de vezes que a palavra aparece na música.
 * @return Um ponteiro para a nova estrutura SongOccurrence.
 */
SongOccurrence *create_song_occurrence(const char *title, const char *author,
                                       const char *verse_snippet,
                                       unsigned int word_count_in_song);
/**
 * @brief Insere um novo nó em uma árvore de busca binária (BST).
 *
 * Se a árvore estiver vazia, o novo nó se torna a raiz. Caso contrário,
 * ele é inserido recursivamente na posição correta com base na ordem
 * alfabética.
 *
 * @param new_node O nó a ser inserido.
 */
void insert_node(Node *new_node);

/**
 * @brief Insere um novo nó em uma árvore AVL.
 *
 * Insere o nó e depois realiza as rotações necessárias para
 * manter a propriedade de balanceamento da árvore AVL.
 *
 * @param new_node O nó a ser inserido.
 */
void insert_node_avl(Node *new_node);

/**
 * @brief Executa uma rotação para a esquerda no nó fornecido.
 *
 * Utilizado para balancear a árvore AVL quando uma subárvore direita
 * se torna muito pesada.
 *
 * @param x O nó a ser rotacionado.
 * @return O novo nó raiz da subárvore rotacionada.
 */
Node *left_rotate(Node *x);

/**
 * @brief Executa uma rotação para a direita no nó fornecido.
 *
 * Utilizado para balancear a árvore AVL quando uma subárvore esquerda
 * se torna muito pesada.
 *
 * @param y O nó a ser rotacionado.
 * @return O novo nó raiz da subárvore rotacionada.
 */
Node *right_rotate(Node *y);

/**
 * @brief Busca por uma palavra em uma árvore de busca binária (BST).
 *
 * @param root A raiz da árvore onde a busca será realizada.
 * @param word A palavra a ser procurada.
 * @return Um ponteiro para o nó encontrado, ou NULL se a palavra não for
 * encontrada.
 */
Node *search_bst(Node *root, const char *word);

/**
 * @brief Busca por uma palavra em uma árvore AVL.
 *
 * @param root A raiz da árvore AVL onde a busca será realizada.
 * @param word A palavra a ser procurada.
 * @return Um ponteiro para o nó encontrado, ou NULL se a palavra não for
 * encontrada.
 */
Node *search_avl(Node *root, const char *word);

/**
 * @brief Libera toda a memória alocada por uma árvore.
 *
 * Percorre a árvore em pós-ordem para liberar todos os nós,
 * incluindo as ocorrências de músicas e as palavras.
 *
 * @param node A raiz da árvore a ser liberada.
 */
void free_tree(Node *node);

/**
 * @brief Cria e inicializa um array de palavras (WordArray).
 *
 * Aloca memória para a estrutura WordArray e seu array interno de nós.
 *
 * @return Um ponteiro para o novo WordArray.
 */
WordArray *create_word_array();

/**
 * @brief Adiciona um nó a um WordArray.
 *
 * Se necessário, redimensiona o array para acomodar mais nós.
 *
 * @param arr O WordArray ao qual o nó será adicionado.
 * @param node O nó a ser adicionado.
 */
void add_node_to_array(WordArray *arr, Node *node);

/**
 * @brief Ordena um WordArray em ordem alfabética.
 *
 * Utiliza a função qsort para ordenar o array de nós com base na palavra.
 *
 * @param arr O WordArray a ser ordenado.
 */
void sort_word_array(WordArray *arr);

/**
 * @brief Realiza uma busca binária por uma palavra em um WordArray.
 *
 * @param arr O WordArray (que deve estar ordenado).
 * @param word A palavra a ser procurada.
 * @return Um ponteiro para o nó encontrado, ou NULL se não for encontrado.
 */
Node *binary_search_array(WordArray *arr, const char *word);

/**
 * @brief Libera a memória alocada por um WordArray.
 *
 * Libera o array de nós e a própria estrutura WordArray.
 *
 * @param arr O WordArray a ser liberado.
 */
void free_word_array(WordArray *arr);

/**
 * @brief Preenche um WordArray com os nós de uma árvore.
 *
 * Percorre a árvore em-ordem e adiciona cada nó ao array.
 *
 * @param node A raiz da árvore.
 * @param arr O WordArray a ser preenchido.
 */
void populate_array_from_tree(Node *node, WordArray *arr);

/**
 * @brief Libera a memória de uma árvore AVL de frequência.
 *
 * Percorre a árvore em pós-ordem e libera todos os nós e seus conteúdos.
 *
 * @param node A raiz da árvore de frequência.
 */
void free_frequency_tree(Node *node);

/**
 * @brief Insere um nó em uma árvore AVL de frequência (recursivamente).
 *
 * A inserção é baseada na contagem total de palavras (frequência).
 *
 * @param current O nó atual na recursão.
 * @param new_node O novo nó a ser inserido.
 * @return A nova raiz da subárvore.
 */
Node *insert_node_avl_frequency_recursive(Node *current, Node *new_node);

/**
 * @brief Busca por uma frequência específica na árvore AVL de frequência.
 *
 * @param root A raiz da árvore de frequência.
 * @param frequency A frequência a ser buscada.
 * @return Um ponteiro para um nó com a frequência correspondente, ou NULL.
 */
Node *search_avl_frequency(Node *root, unsigned int frequency);

/**
 * @brief Preenche a árvore AVL de frequência a partir de outra árvore.
 *
 * Percorre a árvore principal e insere nós na árvore de frequência.
 *
 * @param node A raiz da árvore principal.
 */
void populate_frequency_avl_tree(Node *node);

/**
 * @brief Busca todas as palavras com uma frequência mínima.
 *
 * Percorre a árvore AVL de frequência para encontrar todas as palavras
 * que ocorrem pelo menos um certo número de vezes.
 *
 * @param root A raiz da árvore de frequência.
 * @param frequency A frequência mínima desejada.
 * @return Um WordArray contendo os nós que correspondem ao critério.
 */
WordArray *search_all_by_frequency(Node *root, unsigned int frequency);
#endif // TREE_H
