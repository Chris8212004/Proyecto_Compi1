# Makefile TEMPORAL para probar preprocesador
TARGET = bin/preprocessor_test

CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/preprocessor.c
OBJECTS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
 $(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
 $(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
 mkdir -p $(BIN_DIR)

$(OBJ_DIR):
 mkdir -p $(OBJ_DIR)

clean:
 rm -rf $(OBJ_DIR) $(BIN_DIR) temp_preprocessed.c

.PHONY: all clean
