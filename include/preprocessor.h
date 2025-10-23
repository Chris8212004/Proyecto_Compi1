#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define MAX_DEFINES 1000
#define MAX_FILENAME 256
#define MAX_DEFINE_VALUE 1024

typedef struct {
    char name[MAX_FILENAME];
    char value[MAX_DEFINE_VALUE];
} Define;

typedef struct {
    Define defines[MAX_DEFINES];
    int define_count;
    int current_line;
    int current_column;
    char current_file[MAX_FILENAME];
} PreprocessorState;

// Declaración CORREGIDA - incluye todos los parámetros necesarios
int preprocess_file(const char* filename, FILE* output, PreprocessorState* state);
void add_define(PreprocessorState* state, const char* name, const char* value);
const char* find_define(PreprocessorState* state, const char* name);
char* expand_defines(PreprocessorState* state, const char* text);
int preprocess(const char* input_filename, const char* output_filename);

#endif