#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "preprocessor.h"

static char* system_include_paths[] = {
    "/usr/include/linux",
    "/usr/include/x86_64-linux-gnu",
    "/usr/lib/gcc/x86_64-linux-gnu/13/include",
    "/usr/lib/gcc/x86_64-linux-gnu/12/include",
    "/usr/include",
    "/usr/local/include", 
    "/usr/lib/gcc/x86_64-linux-gnu/11/include",
    "/usr/lib/gcc/x86_64-linux-gnu/10/include",
    NULL
};

int preprocess(const char* input_filename, const char* output_filename, int enable_preprocessing) {
    PreprocessorState state;
    state.define_count = 0;
    state.current_line = 0;
    state.current_column = 0;
    strcpy(state.current_file, input_filename);
    
    FILE* output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: No se puede crear archivo de salida %s\n", output_filename);
        return 0;
    }
    
    int success = preprocess_file(input_filename, output, &state, enable_preprocessing);
    fclose(output);
    
    return success;
}

int preprocess_file(const char* filename, FILE* output, PreprocessorState* state, int enable_preprocessing) {
    FILE* input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "Error: No se puede abrir archivo %s\n", filename);
        return 0;
    }
    
    char line[4096];
    char original_file[MAX_FILENAME];
    strcpy(original_file, state->current_file);
    strcpy(state->current_file, filename);
    state->current_line = 0;
    
    while (fgets(line, sizeof(line), input)) {
        state->current_line++;
        state->current_column = 0;
        
        char* trimmed = line;
        while (isspace(*trimmed)) {
            trimmed++;
            state->current_column++;
        }
        
        if (trimmed[0] == '#') {
            fputs(line, output);
            
            if (enable_preprocessing) {
                if (strncmp(trimmed, "#include", 8) == 0) {
                    if (!process_include_directive(state, trimmed, output, enable_preprocessing)) {
                        fclose(input);
                        return 0;
                    }
                } else if (strncmp(trimmed, "#define", 7) == 0) {
                    process_define_directive(state, trimmed);
                }
            }
        } 
        else if (strncmp(trimmed, "//", 2) == 0 || strncmp(trimmed, "/*", 2) == 0) {
            fputs(line, output);
        }
        else {
            if (enable_preprocessing) {
                char* expanded = expand_defines(state, line);
                fputs(expanded, output);
                free(expanded);
            } else {
                fputs(line, output);
            }
        }
    }
    
    fclose(input);
    strcpy(state->current_file, original_file);
    return 1;
}

int process_include_directive(PreprocessorState* state, const char* line, FILE* output, int enable_preprocessing) {
    char filename[MAX_FILENAME];
    const char* quote = strchr(line, '"');
    const char* angle = strchr(line, '<');
    const char* start, *end;
    int is_system_header = 0;
    
    if (angle && (!quote || angle < quote)) {
        // #include <archivo.h>
        is_system_header = 1;
        start = angle + 1;
        end = strchr(start, '>');
    } else if (quote) {
        // #include "archivo.h"  
        start = quote + 1;
        end = strchr(start, '"');
    } else {
        fprintf(stderr, "Error: #include sin comillas o <> en línea %d\n", state->current_line);
        return 0;
    }
    
    if (!end) {
        fprintf(stderr, "Error: #include sin cierre en línea %d\n", state->current_line);
        return 0;
    }
    
    int len = end - start;
    strncpy(filename, start, len);
    filename[len] = '\0';
    
    if (enable_preprocessing) {
        if (is_system_header) {
            return process_system_include(state, filename, output, enable_preprocessing);
        } else {
            FILE* test_file = fopen(filename, "r");
            if (test_file) {
                fclose(test_file);
                return preprocess_file(filename, output, state, enable_preprocessing);
            }
            
            char include_path[MAX_FILENAME];
            snprintf(include_path, sizeof(include_path), "include/%s", filename);
            test_file = fopen(include_path, "r");
            if (test_file) {
                fclose(test_file);
                return preprocess_file(include_path, output, state, enable_preprocessing);
            }
            
            fprintf(stderr, "Error: No se puede encontrar archivo %s\n", filename);
            return 0;
        }
    } else {
        fputs(line, output);
        return 1;
    }
}


int process_system_include(PreprocessorState* state, const char* filename, FILE* output, int enable_preprocessing) {
    for (int i = 0; system_include_paths[i] != NULL; i++) {
        char full_path[MAX_FILENAME];
        snprintf(full_path, sizeof(full_path), "%s/%s", system_include_paths[i], filename);
        

        FILE* test = fopen(full_path, "r");
        if (test) {
            fclose(test);
            return preprocess_file(full_path, output, state, enable_preprocessing);
        } 
    }
    
    fprintf(stderr, "No se encuentra header: %s\n", filename);
    fprintf(output,"/* Header no encontrado: %s */\n", filename);
    return 1;
}

void process_define_directive(PreprocessorState* state, const char* line) {
    char name[MAX_FILENAME];
    char value[MAX_DEFINE_VALUE] = "";
    
    const char* ptr = line + 7;
    while (isspace(*ptr)) ptr++;
    
    int i = 0;
    while (isalnum(*ptr) || *ptr == '_') {
        if (i < MAX_FILENAME - 1) name[i++] = *ptr;
        ptr++;
    }
    name[i] = '\0';
    
    while (isspace(*ptr)) ptr++;
    strncpy(value, ptr, MAX_DEFINE_VALUE - 1);
    value[MAX_DEFINE_VALUE - 1] = '\0';
    
    char* newline = strchr(value, '\n');
    if (newline) *newline = '\0';
    
    add_define(state, name, value);
}

void add_define(PreprocessorState* state, const char* name, const char* value) {
    if (state->define_count >= MAX_DEFINES) {
        fprintf(stderr, "Error: Demasiados #defines\n");
        return;
    }
    
    for (int i = 0; i < state->define_count; i++) {
        if (strcmp(state->defines[i].name, name) == 0) {
            strcpy(state->defines[i].value, value);
            return;
        }
    }
    
    strcpy(state->defines[state->define_count].name, name);
    strcpy(state->defines[state->define_count].value, value);
    state->define_count++;
}

const char* find_define(PreprocessorState* state, const char* name) {
    for (int i = 0; i < state->define_count; i++) {
        if (strcmp(state->defines[i].name, name) == 0) {
            return state->defines[i].value;
        }
    }
    return NULL;
}

char* expand_defines(PreprocessorState* state, const char* text) {
    char* result = malloc(strlen(text) * 2 + 1);
    result[0] = '\0';
    
    const char* ptr = text;
    char word[256];
    int expanded;
    
    do {
        expanded = 0;
        char* temp_result = malloc(strlen(text) * 2 + 1);
        temp_result[0] = '\0';
        ptr = text;
        
        while (*ptr) {
            if (isalpha(*ptr) || *ptr == '_') {
                int i = 0;
                word[i++] = *ptr++;
                
                while (isalnum(*ptr) || *ptr == '_') {
                    if (i < 255) word[i++] = *ptr;
                    ptr++;
                }
                word[i] = '\0';
                
                const char* expansion = find_define(state, word);
                if (expansion) {
                    strcat(temp_result, expansion);
                    expanded = 1;
                } else {
                    strcat(temp_result, word);
                }
            } else {
                char temp[2] = {*ptr, '\0'};
                strcat(temp_result, temp);
                ptr++;
            }
        }
        
        free(result);
        result = temp_result;
        text = result;
        
    } while (expanded);
    
    return result;
}
