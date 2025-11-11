#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

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

// Declaraciones
int preprocess(const char* input_filename, const char* output_filename, int enable_preprocessing);
int preprocess_file(const char* filename, FILE* output, PreprocessorState* state, int enable_preprocessing);
void add_define(PreprocessorState* state, const char* name, const char* value);
const char* find_define(PreprocessorState* state, const char* name);
char* expand_defines(PreprocessorState* state, const char* text);
void process_define_directive(PreprocessorState* state, const char* line);
int process_include_directive(PreprocessorState* state, const char* line, FILE* output, int enable_preprocessing);
int process_system_include(PreprocessorState* state, const char* filename, FILE* output, int enable_preprocessing);

#endif

