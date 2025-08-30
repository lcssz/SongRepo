/**
 * @file main.c
 * @brief Programa principal do repositório de letras de músicas
 * @author Davi Santos Oliveira, Matheus Silva Escarpinatti, Lucas Souza Soares
 * @version 1.0
 * 
 * @details Este arquivo contém a função main e o loop principal do programa,
 * que gerencia o menu de opções e coordena as operações do repositório.
 */

#include "../include/structures/tree.h"
#include "../include/core/repository.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Função principal do programa
 * 
 * @return 0 em caso de sucesso, 1 em caso de erro
 * 
 * @details Inicializa o repositório e executa o loop principal do menu,
 * permitindo ao usuário carregar arquivos, buscar palavras e analisar frequências.
 */
int main() {
    Repository *repo = create_repository();
    if (repo == NULL) {
        fprintf(stderr, "Erro ao criar repositório.\n");
        return 1;
    }
    
    int choice;
    
    printf("=== REPOSITÓRIO DE LETRAS DE MÚSICAS ===\n");
    printf("Sistema implementado com:\n");
    printf("- Vetor com pesquisa binária\n");
    printf("- Árvore Binária de Busca\n");
    printf("- Árvore AVL\n");
    printf("========================================\n\n");
    
    do {
        show_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                load_files_menu(repo);
                break;
            case 2:
                search_word_menu(repo);
                break;
            case 3:
                search_frequency_menu(repo);
                break;
            case 4:
                clear_screen();
                break;
            case 5:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (choice != 5);
    
    free_repository(repo);
    return 0;
}