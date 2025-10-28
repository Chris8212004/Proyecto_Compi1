# Makefile para preprocesador + scanner (lex.yy.c en src/)
TARGET = bin/compiler

CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/preprocessor.c $(SRC_DIR)/lex.yy.c
OBJECTS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -lfl

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) temp_preprocessed.c $(SRC_DIR)/lex.yy.c

# Generar lex.yy.c automáticamente si no existe
$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/scanner.l
	flex -o $@ $<

.PHONY: all clean
