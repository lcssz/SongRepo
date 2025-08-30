# SongRepo
Este projeto é um sistema de gerenciamento de um repositório de letras de músicas, implementado em C. Ele utiliza árvores de busca binária (BST), árvores AVL e arrays ordenados para armazenar e buscar palavras eficientemente.
Este repositório representa o culminar de nossos esforços e aprendizado na disciplina de AED2, onde exploramos estruturas de dados avançadas para resolver um problema prático e interessante.

## Pré-requisitos

Para compilar e gerar a documentação, você precisará ter os seguintes programas instalados:

* `gcc` (GNU Compiler Collection)
* `make`
* `doxygen` (para a documentação)

## Como Compilar o Programa

O projeto utiliza um `Makefile` para simplificar a compilação.

1.  Navegue até o diretório raiz do projeto.
2.  Execute o seguinte comando no terminal:

    ```sh
    make
    ```
3.  Isso irá compilar todos os arquivos-fonte e gerar um executável chamado `music_repository`.

4.  Para limpar os arquivos compilados (arquivos `.o` e o executável), execute:
    ```sh
    make clean
    ```

## Como Gerar a Documentação

A documentação do código é gerada usando o Doxygen.

1.  Certifique-se de estar no diretório raiz do projeto e possuir o Doxygen instalado.
2.  Após isso gere a documentação executando:
    ```sh
    doxygen Doxyfile
    ```
3.  A documentação será criada na pasta `docs/html/`. Abra o arquivo `index.html` em um navegador para visualizar.

## Como Executar o Programa

Após compilar o projeto, você pode executá-lo a partir do terminal:

```sh
./song_repo
```

Um menu interativo será exibido, permitindo carregar arquivos de música e realizar buscas.

## Autores

Este projeto foi cuidadosamente desenvolvido e implementado por:

* Lucas Souza Soares
* Matheus Escarpinati
