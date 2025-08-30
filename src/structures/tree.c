/**
 * @file tree.c
 * @brief Implementação das árvores binárias de busca e AVL
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 * 
 * @details Este arquivo contém a implementação de árvores binárias de busca
 *          (não balanceadas) e árvores AVL (balanceadas), incluindo todas
 *          as operações de inserção, busca e rotações necessárias para
 *          manter o balanceamento das árvores AVL.
 */

#include "../../include/structures/tree.h"
#include "../../include/structures/word_info.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Árvore binária de busca global
 * @details Árvore binária de busca não balanceada usada para demonstração
 *          e comparação de performance com a árvore AVL.
 */
Tree *bin_tree = NULL;

/**
 * @brief Árvore AVL global
 * @details Árvore AVL balanceada que mantém automaticamente o balanceamento
 *          através de rotações durante inserções e remoções.
 */
Tree *avl_tree = NULL;

/**
 * @brief Cria um novo nó para árvore básica
 * @param word Palavra a ser armazenada
 * @return Ponteiro para o novo nó ou NULL se falhar
 * @details Aloca e inicializa um novo nó para árvore binária básica.
 *          O nó armazena uma palavra e mantém contadores de frequência
 *          e altura para operações de árvore.
 * 
 *          A função:
 *          - Aloca memória para a estrutura Node
 *          - Armazena a palavra fornecida
 *          - Inicializa total_word_count como 0
 *          - Define altura inicial como 1
 *          - Inicializa ponteiros left e right como NULL
 * 
 * @note A palavra é armazenada como ponteiro, não como cópia.
 *       O chamador deve garantir que a string permaneça válida.
 * 
 * @note O nó é criado com altura 1, que será atualizada durante
 *       inserções na árvore AVL.
 * 
 * @warning É responsabilidade do chamador verificar se o retorno é NULL
 *          e tratar adequadamente falhas de alocação.
 * 
 * @example
 * ```c
 * Node *node = create_node("love");
 * if (node == NULL) {
 *     fprintf(stderr, "Erro ao criar nó.\n");
 *     return 1;
 * }
 * ```
 */
Node *create_node(const char *word) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  new_node->word = word;
  new_node->total_word_count = 0;
  new_node->height = 1;
  return new_node;
}

/**
 * @brief Realiza rotação à direita
 * @param y Nó raiz da subárvore
 * @return Nova raiz após rotação
 * @details Realiza uma rotação à direita em um nó da árvore AVL para
 *          restaurar o balanceamento. A rotação é usada quando um nó
 *          fica desbalanceado para a esquerda.
 * 
 *          A rotação:
 *          - x = filho esquerdo de y
 *          - T2 = filho direito de x
 *          - x se torna a nova raiz
 *          - y se torna filho direito de x
 *          - T2 se torna filho esquerdo de y
 *          - Atualiza alturas dos nós afetados
 * 
 * @note Esta função é usada para balancear árvores AVL quando o fator
 *       de balanceamento de um nó é maior que 1 (desbalanceado à esquerda).
 * 
 * @note A função atualiza automaticamente as alturas dos nós afetados
 *       pela rotação.
 * 
 * @example
 * ```
 * Antes:     y         Depois:    x
 *           / \                  / \
 *          x   C               A   y
 *         / \                      / \
 *        A   B                    B   C
 * ```
 */
Node *right_rotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  return x;
}

/**
 * @brief Realiza rotação à esquerda
 * @param x Nó raiz da subárvore
 * @return Nova raiz após rotação
 * @details Realiza uma rotação à esquerda em um nó da árvore AVL para
 *          restaurar o balanceamento. A rotação é usada quando um nó
 *          fica desbalanceado para a direita.
 * 
 *          A rotação:
 *          - y = filho direito de x
 *          - T2 = filho esquerdo de y
 *          - y se torna a nova raiz
 *          - x se torna filho esquerdo de y
 *          - T2 se torna filho direito de x
 *          - Atualiza alturas dos nós afetados
 * 
 * @note Esta função é usada para balancear árvores AVL quando o fator
 *       de balanceamento de um nó é menor que -1 (desbalanceado à direita).
 * 
 * @note A função atualiza automaticamente as alturas dos nós afetados
 *       pela rotação.
 * 
 * @example
 * ```
 * Antes:     x         Depois:    y
 *           / \                  / \
 *          A   y               x   C
 *             / \              / \
 *            B   C            A   B
 * ```
 */
Node *left_rotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height_node(x->left), height_node(x->right)) + 1;
  y->height = max(height_node(y->left), height_node(y->right)) + 1;
  return y;
}

void insert_node_recursive(Node *current, Node *new_node) {
  int comparison = strcmp(new_node->word, current->word);

  if (comparison < 0) {
    if (current->left == NULL) {
      current->left = new_node;
    } else {
      insert_node_recursive(current->left, new_node);
    }
  } else if (comparison > 0) {
    if (current->right == NULL) {
      current->right = new_node;
    } else {
      insert_node_recursive(current->right, new_node);
    }
  } else {
    current->total_word_count++;
    free(new_node);
  }
}

Node *insert_node_avl_recursive(Node *current, Node *new_node) {
  if (current == NULL) {
    return new_node;
  }

  int comparison = strcmp(new_node->word, current->word);

  if (comparison < 0) {
    current->left = insert_node_avl_recursive(current->left, new_node);
  } else if (comparison > 0) {
    current->right = insert_node_avl_recursive(current->right, new_node);
  } else {

    current->total_word_count++;
    free(new_node);
    return current;
  }

  current->height =
      1 + max(height_node(current->left), height_node(current->right));

  int balance = get_balance(current);



  if (balance > 1 && comparison < 0) {
    return right_rotate(current);
  }

  if (balance < -1 && comparison > 0) {
    return left_rotate(current);
  }

  if (balance > 1 && comparison > 0) {
    current->left = left_rotate(current->left);
    return right_rotate(current);
  }

  if (balance < -1 && comparison < 0) {
    current->right = right_rotate(current->right);
    return left_rotate(current);
  }
  return current;
}

void insert_node(Node *new_node) {
  if (bin_tree == NULL)
    initialize_tree(bin_tree);

  if (bin_tree->root == NULL)
    bin_tree->root = new_node;
  else
    insert_node_recursive(bin_tree->root, new_node);
}

void insert_node_avl(Node *new_node) {
  if (avl_tree == NULL)
    initialize_tree(avl_tree);

  avl_tree->root = insert_node_avl_recursive(avl_tree->root, new_node);
}


Node* search_bst(Node *root, const char *word) {
    if (root == NULL || word == NULL) return NULL;
    
    int comparison = strcmp(word, root->word);
    if (comparison == 0) {
        return root;
    } else if (comparison < 0) {
        return search_bst(root->left, word);
    } else {
        return search_bst(root->right, word);
    }
}

Node* search_avl(Node *root, const char *word) {

    return search_bst(root, word);
}


NodeComplete* create_node_complete(WordInfo *info) {
    NodeComplete *new_node = (NodeComplete*)malloc(sizeof(NodeComplete));
    if (new_node == NULL) return NULL;
    
    new_node->info = info;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void insert_node_complete_bst(TreeComplete *tree, WordInfo *info) {
    if (tree == NULL || info == NULL) return;
    
    NodeComplete *new_node = create_node_complete(info);
    if (new_node == NULL) return;
    
    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }
    
    NodeComplete *current = tree->root;
    NodeComplete *parent = NULL;
    
    while (current != NULL) {
        parent = current;
        int comparison = strcmp(info->word, current->info->word);
        
        if (comparison < 0) {
            current = current->left;
        } else if (comparison > 0) {
            current = current->right;
        } else {
                    for (int i = 0; i < info->num_songs; i++) {
                add_song_to_word(current->info, 
                                info->songs[i].title,
                                info->songs[i].author,
                                info->songs[i].verse_snippet,
                                info->songs[i].frequency_in_song);
            }
            free(new_node);
            return;
        }
    }
    
    int comparison = strcmp(info->word, parent->info->word);
    if (comparison < 0) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

NodeComplete* left_rotate_complete(NodeComplete *x) {
    NodeComplete *y = x->right;
    NodeComplete *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height_node(x->left), height_node(x->right)) + 1;
    y->height = max(height_node(y->left), height_node(y->right)) + 1;
    return y;
}

NodeComplete* right_rotate_complete(NodeComplete *y) {
    NodeComplete *x = y->left;
    NodeComplete *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height_node(y->left), height_node(y->right)) + 1;
    x->height = max(height_node(x->left), height_node(x->right)) + 1;
    return x;
}

NodeComplete* insert_node_complete_avl_recursive(NodeComplete *root, WordInfo *info) {
    if (root == NULL) {
        return create_node_complete(info);
    }

    int comparison = strcmp(info->word, root->info->word);

    if (comparison < 0) {
        root->left = insert_node_complete_avl_recursive(root->left, info);
    } else if (comparison > 0) {
        root->right = insert_node_complete_avl_recursive(root->right, info);
    } else {
        for (int i = 0; i < info->num_songs; i++) {
            add_song_to_word(root->info, 
                            info->songs[i].title,
                            info->songs[i].author,
                            info->songs[i].verse_snippet,
                            info->songs[i].frequency_in_song);
        }
        return root;
    }


    root->height = 1 + max(height_node(root->left), height_node(root->right));


    int balance = get_balance(root);


    if (balance > 1 && comparison < 0) {
        return right_rotate_complete(root);
    }
    if (balance < -1 && comparison > 0) {
        return left_rotate_complete(root);
    }
    if (balance > 1 && comparison > 0) {
        root->left = left_rotate_complete(root->left);
        return right_rotate_complete(root);
    }
    if (balance < -1 && comparison < 0) {
        root->right = right_rotate_complete(root->right);
        return left_rotate_complete(root);
    }

    return root;
}

void insert_node_complete_avl(TreeComplete *tree, WordInfo *info) {
    if (tree == NULL || info == NULL) return;
    tree->root = insert_node_complete_avl_recursive(tree->root, info);
}

NodeComplete* search_tree_complete(NodeComplete *root, const char *word) {
    if (root == NULL || word == NULL) return NULL;
    
    int comparison = strcmp(word, root->info->word);
    if (comparison == 0) {
        return root;
    } else if (comparison < 0) {
        return search_tree_complete(root->left, word);
    } else {
        return search_tree_complete(root->right, word);
    }
}


double measure_insertion_time(void (*insert_func)(void*), void *data) {
    clock_t start = clock();
    insert_func(data);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double measure_search_time(Node* (*search_func)(Node*, const char*), Node *root, const char *word) {
    clock_t start = clock();
    search_func(root, word);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}
