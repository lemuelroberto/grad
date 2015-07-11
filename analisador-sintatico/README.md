T2 - Análise Sintática
======================

1. DESENVOLVEDORES
------------------
  - Erick Savastano Becker
  - Lemuel Roberto Bonifácio
  - Matheus Martins Teixeira

2. CONTEÚDO
-----------

>  ./inc/              : Diretório com headers

>  ./spec/             : Diretório com especificações do projeto

>  ./inc/opt.h         : Definições para processamento de linha de comando

>  ./src/              : Diretório com arquivos fonte

>  ./src/main.c        : Programa principal compilador

>  ./src/opt.h         : Implementação do processamento de linha de comando

>  ./src/parser.y      : Regras do analisador sintático

>  ./src/scanner.l     : Regras do analisador léxico

>  ./tests/            : Diretório com os casos de teste (entradas e saídas)

>  ./tests/input/      : Diretório com as entradas dos casos de teste

>  ./Makefile          : Makefile

>  ./README            : Este arquivo

3. COMO COMPILAR
----------------
  Em sistemas que aceitam makefiles GNU, basta rodar o comando `make` na
  raíz do diretório para criar o analisador sintático.
  Implicitamente os seguintes comandos são realizados pelo Makefile:

  `bison --defines=./inc/yacc.yy.h -o $(SRC)/yacc.yy.c ./src/parser.y`

  `flex -o ./src/lex.yy.c ./src/scanner.l`

  `g++ -o ./bin/metabee.bin ./src/lex.yy.c ./src/yacc.yy.c ./src/opt.c src/main.c\
    -I./inc -std=c++11 -Wall`

  O projeto foi desenvolvido, compilado e testado em GNU/Linux e não foi
  testado em nenhum outro sistema operacional.

4. COMO EXECUTAR
----------------
  Basta executar o programa gerado `./bin/metabee.bin`. O aplicativo recebe o
  código-fonte diretamente de stdin, procéssa-os e exibe o resultado em stdout.

  **Para executar todos os casos de teste:**

  `make run-tests`

  **Para executar um caso de teste:**

  `./bin/metabee.bin -i arquivo_entrada`

  `./bin/metabee.bin < arquivo_entrada` (redirecionar o arquivo para stdin)

  **Para redirecionar a saída para outro arquivo:**

  `./bin/metabee.bin < arquivo_entrada > arquivo_saida`

  **Para instruções de uso:**

  `./bin/metabee.bin -h`

  Opções:

  -i ARQUIVO  : especifica arquivo de entrada

  -h          : exibe mensagem de ajuda
