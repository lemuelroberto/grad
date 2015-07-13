# Directories
SRC=./src
INC=./inc
BIN=./bin
TEST=./test

# Compiler settings
CC=g++
CXXFLAGS=-ggdb -Wall -I$(INC)

CFLAGS=`pkg-config --cflags opencv`
LIBS=`pkg-config --libs opencv`

DEGUBFLAG= #use '-DNDEBUG' to not debug

# Files
SOURCES=$(SRC)/main.cpp $(SRC)/opt.c $(SRC)/filters.cpp
EXECUTABLE=t2.bin

all: $(SOURCES)
	@echo "Compilando o trabalho 2..."
	@mkdir -p $(BIN)
	$(CC) $(CXXFLAGS) $(CFLAGS) -o $(BIN)/$(EXECUTABLE) $(SOURCES) $(LIBS) $(DEGUBFLAG)
	@echo "Compilado com sucesso."

run-tests: all
	@echo
	@echo "TEST-01: Lenna-original.jpg to Lenna-filtered.jpg"
	$(BIN)/$(EXECUTABLE) -l -i $(TEST)/Lenna-original.jpg -o $(TEST)/Lenna-filtered.jpg

	@echo
	@echo "TEST-02: opencv-logo-original.jpg to opencv-logo-filtered.jpg"
	$(BIN)/$(EXECUTABLE) -lg -i $(TEST)/opencv-logo-original.png -o $(TEST)/opencv-logo-filtered.png

clean:
	@echo "Apagando arquivos binários..."
	rm -f $(BIN)/*.bin
	@echo "Pronto"

.PHONY: clean
