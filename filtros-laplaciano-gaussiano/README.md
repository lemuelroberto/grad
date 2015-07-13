# SCC0251 - Processamento de Imagens

## Filtros Gaussiano, Laplaciano e Laplaciano do Gaussiano

1. DESCRIÇÃO
------------

  >Aplica um filtro escolhido à uma image de origem e salva em uma imagem destino.
  Filtros possíveis:

  >- Gaussiano
  >- Laplaciano
  >- Laplaciano do Gaussiano

## 2. CONTEÚDO

  ./bin               : Diretório para arquivos binários

  ./inc/              : Diretório com headers

  ./inc/dbg.h         : Definições para debug

  ./inc/filters.hpp   : Definições para as funções de filtro

  ./inc/opt.h         : Definições para tratamento de linha de comando

  ./src/              : Diretório com arquivos fonte

  ./src/filters.cpp   : Implentação dos filtros

  ./src/main.cpp      : Programa principal

  ./src/opt.c         : Implementação das funções que tratam linha de comando

  ./tests             :  Casos de teste

  ./Makefile          : Makefile que compila e gera executável ./bin/t2.bin

  ./README.md         : Este arquivo

## 3. COMO COMPILAR

  Em sistemas que aceitam makefiles GNU, basta rodar o comando `make` na raíz do
  diretório para criar o executável.
  Implicitamente os seguintes comandos são realizados pelo Makefile:

    - CFLAGS = `pkg-config --cflags opencv`
    - LIBS = `pkg-config --libs opencv`

  `g++ -ggdb -Wall -I./inc $(CFLAGS) -o .bin/t2.bin .src/main.cpp .src/opt.c .src/filters.cpp $(LIBS)`

  O projeto foi desenvolvido, compilado e testado em GNU/Linux e não foi
  testado em nenhum outro sistema operacional.

## 4. COMO EXECUTAR

  `./bin/t2 [FILTROS]... [ARQUIVOS]...`

  - Filtros:

    - **-g:** aplica o filtro Gaussiano
    - **-l:** aplica o filtro Laplaciano
    - **-lg:** aplica o filtro Laplaciano do Gaussiano

  - Arquivos:

    - **-i ARQUIVO:** arquivo (imagem) de entrada
    - **-o ARQUIVO:** arquivo (imagem) de saída
