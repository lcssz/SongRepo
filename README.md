# Repositório de Letras de Músicas - AED2

Este projeto implementa um sistema de repositório de letras de músicas utilizando três estruturas de dados diferentes:

## Estruturas de Dados Implementadas

1. **Vetor com Pesquisa Binária** - Para busca eficiente em dados ordenados
2. **Árvore Binária de Busca (ABB)** - Para organização hierárquica dos dados
3. **Árvore AVL** - Árvore balanceada para melhor performance

## Funcionalidades

### A. Carregamento de Arquivos
- **Arquivo individual**: Lê um arquivo .txt específico
- **Pasta completa**: Carrega todos os arquivos .txt de uma pasta automaticamente
- Processa palavras com 4 ou mais caracteres
- Remove pontuação e converte para minúsculo
- Armazena informações completas: título, autor, trecho, frequências
- Mede tempo de inserção em cada estrutura
- **Suporte a múltiplos formatos**: Formato original e formato da pasta `examples/LetrasMusicas`

### B. Busca de Palavras
- Busca uma palavra nas três estruturas simultaneamente
- Retorna informações de **todas as músicas** que contêm a palavra
- Mede e compara tempo de busca em cada estrutura
- Mostra **frequência real** da palavra em cada música
- Exibe **frequência total** somando todas as ocorrências

### C. Busca por Frequência
- Busca palavras com frequência mínima especificada
- **Busca em todas as três estruturas** (vetor, árvore binária, árvore AVL)
- Mede e compara tempo de busca em cada estrutura
- Permite análise de palavras mais recorrentes
- Mostra resumo de performance das três estruturas

## Como Compilar e Executar

### Compilação:
```bash
# Criar pasta build (se não existir)
mkdir -p build

# Compilar arquivos objeto na pasta build
gcc -Wall -Wextra -std=c99 -g -I./include/core -I./include/structures -c src/main.c -o build/main.o
gcc -Wall -Wextra -std=c99 -g -I./include/core -I./include/structures -c src/core/repository.c -o build/repository.o
gcc -Wall -Wextra -std=c99 -g -I./include/core -I./include/structures -c src/structures/tree.c -o build/tree.o
gcc -Wall -Wextra -std=c99 -g -I./include/core -I./include/structures -c src/structures/vector.c -o build/vector.o

gcc build/main.o build/repository.o build/tree.o build/vector.o build/word_info.o -o build/repositorio_musicas
```

```bash
# Windows
./build/repositorio_musicas.exe

# Linux/Mac
./build/repositorio_musicas
```

## Formatos dos Arquivos de Música

### Formato Original
```
Título da Música
Nome do Autor
Linha 1 da letra
Linha 2 da letra
...
```

### Formato LetrasMusicas (examples/LetrasMusicas/)
```
Título da Música
Nome do Autor
[linha em branco]
Linha 1 da letra
Linha 2 da letra
...
```

Exemplo:
```
Lança Perfume
Rita Lee

Lança, menina, lança todo esse perfume
Desbaratina, não dá pra ficar imune
Ao teu amor que tem cheiro de coisa maluca
...
```

## Menu do Sistema

1. **Carregar arquivos .txt** - Carrega arquivo individual ou pasta completa
2. **Buscar palavra** - Busca uma palavra específica nas estruturas
3. **Buscar por frequência** - Encontra palavras com frequência mínima em todas as estruturas
4. **Limpar tela** - Apaga buscas passadas e limpa o terminal
5. **Sair** - Encerra o programa

## Exemplo de Uso

> **⚠️ Importante**: Configure o terminal para UTF-8 antes de executar o programa para exibir corretamente caracteres especiais.

1. Execute o programa: `./build/repositorio_musicas.exe`
2. Escolha opção 1 para carregar arquivos
3. **Opção 1**: Carregar arquivo individual
   - Digite o caminho (ex: `examples/exemplo_musica.txt`)
4. **Opção 2**: Carregar pasta completa
   - Digite o caminho da pasta (ex: `examples/LetrasMusicas`)
   - Todos os arquivos .txt serão processados automaticamente
5. Escolha opção 2 para buscar uma palavra
6. Digite a palavra desejada (ex: `perfume`)
7. Veja **todas as músicas** onde a palavra aparece com frequências reais
8. Escolha opção 3 para buscar por frequência
9. Digite a frequência mínima (ex: `2`)
10. Veja **todas as estruturas** sendo comparadas com tempos de busca
11. Use opção 4 para limpar a tela e apagar buscas passadas

### Exemplo de Saída da Busca por Palavra:
```
=== Resultados da busca para 'perfume' ===

1. Vetor (Busca Binária):
Tempo: 0.000123 segundos
Palavra encontrada!
Frequência total: 15
Número de músicas: 1
---
Músicas onde aparece:
1. Música: Lança Perfume
   Autor: Rita Lee
   Trecho: Lança, menina, lança todo esse perfume
   Frequência nesta música: 15
   ---
```

### Exemplo de Saída da Busca por Frequência:
```
=== Palavras com frequência >= 10 ===

1. Busca no Vetor:
Palavra: perfume
Frequência total: 15
Número de músicas: 1
---
Música 1: Lança Perfume
Autor: Rita Lee
Trecho: Lança, menina, lança todo esse perfume
Frequência nesta música: 15
---
Tempo: 0.000045 segundos | Palavras encontradas: 1

2. Busca na Árvore Binária:
[mesmas informações]
Tempo: 0.000067 segundos | Palavras encontradas: 1

3. Busca na Árvore AVL:
[mesmas informações]
Tempo: 0.000052 segundos | Palavras encontradas: 1

=== Resumo da Performance ===
Vetor: 0.000045 segundos (1 palavras)
Árvore Binária: 0.000067 segundos (1 palavras)
Árvore AVL: 0.000052 segundos (1 palavras)
```

## Estrutura do Projeto

```
Trabalho/
├── src/                          # Código fonte
│   ├── main.c                    # Programa principal com menu
│   ├── core/
│   │   └── repository.c          # Lógica do repositório e processamento
│   └── structures/
│       ├── tree.c                # Implementação das árvores (ABB e AVL)
│       └── vector.c              # Implementação do vetor com busca binária
├── include/                      # Headers das estruturas de dados
│   ├── core/
│   │   └── repository.h          # Definições do repositório
│   └── structures/
│       ├── word_info.h           # Estrutura WordInfo
│       ├── llist.h               # Lista encadeada e vetor
│       └── tree.h                # Árvores binárias e AVL
├── docs/                         # Documentação
│   ├── README.md                 # Este arquivo
│   ├── Documentação.html         # Acesso à documentação Doxygen
│   └── html/                     # Documentação Doxygen completa
├── examples/                     # Arquivos de exemplo
│   ├── exemplo_musica.txt
│   └── exemplo_musica2.txt
├── build/                        # Arquivos compilados (gerados)
├── Doxyfile                      # Configuração do Doxygen
└── .gitignore                    # Arquivos ignorados pelo Git
```

## Requisitos

- **Compilador GCC** (ou compatível)
- **Sistema**: Windows, Linux ou macOS
- **Arquivos .txt** com letras de músicas no formato especificado
- **Doxygen** (opcional, para gerar documentação)

## Configuração UTF-8

Para exibir caracteres especiais corretamente:

**Windows (PowerShell/CMD):**
```bash
chcp 65001
```

**Linux/macOS:**
```bash
export LANG=en_US.UTF-8
```

## Documentação

O projeto inclui documentação completa gerada com Doxygen:

### Gerar Documentação
```bash
# Instalar Doxygen (Windows)
install_doxygen.bat

# Ou instalar manualmente
# Baixar de: https://www.doxygen.nl/download.html

# Gerar documentação
doxygen Doxyfile
```

### Acessar Documentação
- **HTML Principal**: `docs/Documentação.html` - Redirecionamento automático para a documentação completa
- **HTML Completo**: `docs/html/index.html` - Documentação Doxygen completa

### Estrutura da Documentação
- **Estruturas de dados**: Definições completas de todas as estruturas
- **Funções**: Documentação detalhada de parâmetros e retornos
- **Arquivos**: Descrição de cada arquivo do projeto
- **Gráficos**: Diagramas de dependências e hierarquias
- **Busca**: Sistema de busca integrado para encontrar funções e estruturas

## Controle de Versão

O projeto inclui um arquivo `.gitignore` que ignora:
- Arquivos binários (*.exe, *.o, *.obj)
- Pasta build/
- Arquivos temporários
- Arquivos de IDE/Editor
- Arquivos do sistema

## Análise de Performance

O sistema permite comparar a performance das três estruturas:
- **Vetor**: O(1) inserção, O(log n) busca (após ordenação)
- **ABB**: O(log n) inserção e busca (caso médio)
- **AVL**: O(log n) inserção e busca (garantido)

## Autores

**Davi Santos Oliveira** - 12411BCC088  
**Matheus Silva Escarpinatti** - 12411BCC064  
**Lucas Souza Soares** - 12421BCC054  

Projeto desenvolvido para AED2 (Algoritmos e Estruturas de Dados II)
