# Directories
SRC=./src
INC=./inc
BIN=./bin
INPUT=./test/input
OUTPUT=./test/output

# Compiler settings
CC=g++
CXXFLAGS=-I$(INC) -std=c++11 -Wall

# Files
SOURCES=$(SRC)/lex.yy.c $(SRC)/yacc.yy.c $(SRC)/opt.c $(SRC)/main.c

all: bison flex $(SOURCES)
	@echo "Compilando o analisador léxico..."
	@mkdir -p $(BIN)
	$(CC) -o $(BIN)/metabee.bin $(SOURCES) $(CXXFLAGS)
	@echo "COMPILADO COM SUCESSO!"

bison: $(SRC)/parser.y
	@echo "Gerando o codigo fonte do analisador sintático..."
	bison --defines=$(INC)/yacc.yy.h -o $(SRC)/yacc.yy.c $(SRC)/parser.y -Wno-conflicts-sr
	@echo "Pronto"

flex: $(SRC)/scanner.l
	@echo "Gerando o codigo fonte do analisador léxico..."
	flex -o $(SRC)/lex.yy.c $(SRC)/scanner.l
	@echo "Pronto"

clean:
	@echo "Apagando arquivos binários, gerados pelo flex e resultados de testes..."
	rm -f $(BIN)/metabee.bin $(INC)/yacc.yy.h $(SRC)/yacc.yy.c $(SRC)/lex.yy.c $(OUTPUT)/*.out
	@echo "Pronto"

.PHONY: clean

run-tests: all
	@rm -f $(OUTPUT)/*;
	@for TEST_NAME in $$(dir $(INPUT)); do \
		echo "Executando teste '$$TEST_NAME'"; \
		$(BIN)/metabee.bin < $(INPUT)/$$TEST_NAME > $(OUTPUT)/$$(basename $$TEST_NAME .in).out 2>&1; \
		# echo "Resultados para o teste '$$TEST_NAME'\n\n$$(cat $(OUTPUT)/$$(basename $$TEST_NAME .in).out)\n"; \
	done;
