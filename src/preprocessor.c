#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preprocessor.h"

static int process_include_directive(PreprocessorState* state, 
                                   const char* line, 
                                   FILE* output) {
    char filename[MAX_FILENAME];
    
    // Buscar comillas después de #include
    const char* quote = strchr(line, '"');
    if (!quote) {
        fprintf(stderr, "Error: #include sin comillas en línea %d\n", state->current_line);
        return 0;
    }
    
    // Extraer nombre del archivo entre comillas
    const char* end_quote = strchr(quote + 1, '"');
    if (!end_quote) {
        fprintf(stderr, "Error: #include sin comilla de cierre en línea %d\n", state->current_line);
        return 0;
    }
    
    int len = end_quote - (quote + 1);
    strncpy(filename, quote + 1, len);
    filename[len] = '\0';
    
    // ✅ LLAMADA CORRECTA ahora - solo 3 parámetros
    return preprocess_file(filename, output, state);
}

static int preprocess_file(const char* filename, FILE* output, PreprocessorState* state) {
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
        
        // Eliminar espacios al inicio
        char* trimmed = line;
        while (isspace(*trimmed)) {
            trimmed++;
            state->current_column++;
        }
        
        // Verificar si es directiva de preprocesador
        if (trimmed[0] == '#') {
            if (strncmp(trimmed, "#include", 8) == 0) {
                // ✅ LLAMADA CORRECTA ahora
                if (!process_include_directive(state, trimmed, output)) {
                    fclose(input);
                    return 0;
                }
            } else if (strncmp(trimmed, "#define", 7) == 0) {
                process_define_directive(state, trimmed);
                // Los #define no se escriben al output
            } else {
                // Otra directiva - escribir tal cual
                fputs(line, output);
            }
        } else {
            // Línea normal - expandir macros y escribir
            char* expanded = expand_defines(state, line);
            fputs(expanded, output);
            free(expanded);
        }
    }
    
    fclose(input);
    strcpy(state->current_file, original_file);
    return 1;
}

// En preprocessor.c
static void process_define_directive(PreprocessorState* state, const char* line) {
    char name[MAX_FILENAME];
    char value[MAX_DEFINE_VALUE] = "";
    
    // Saltar #define y espacios
    const char* ptr = line + 7; // después de "#define"
    while (isspace(*ptr)) ptr++;
    
    // Leer nombre del macro
    int i = 0;
    while (isalnum(*ptr) || *ptr == '_') {
        name[i++] = *ptr++;
    }
    name[i] = '\0';
    
    // Saltar espacios
    while (isspace(*ptr)) ptr++;
    
    // Leer valor (resto de la línea)
    strncpy(value, ptr, MAX_DEFINE_VALUE - 1);
    value[MAX_DEFINE_VALUE - 1] = '\0';
    
    // Eliminar newline al final si existe
    char* newline = strchr(value, '\n');
    if (newline) *newline = '\0';
    
    add_define(state, name, value);
}

void add_define(PreprocessorState* state, const char* name, const char* value) {
    if (state->define_count >= MAX_DEFINES) {
        fprintf(stderr, "Error: Demasiados #defines\n");
        return;
    }
    
    // Reemplazar si ya existe
    for (int i = 0; i < state->define_count; i++) {
        if (strcmp(state->defines[i].name, name) == 0) {
            strcpy(state->defines[i].value, value);
            return;
        }
    }
    
    // Agregar nuevo
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
// En preprocessor.c
char* expand_defines(PreprocessorState* state, const char* text) {
    char* result = malloc(strlen(text) * 2 + 1); // Buffer amplio
    result[0] = '\0';
    
    const char* ptr = text;
    char word[256];
    
    while (*ptr) {
        // Si es inicio de identificador
        if (isalpha(*ptr) || *ptr == '_') {
            int i = 0;
            word[i++] = *ptr++;
            
            // Completar el identificador
            while (isalnum(*ptr) || *ptr == '_') {
                if (i < 255) word[i++] = *ptr;
                ptr++;
            }
            word[i] = '\0';
            
            // Verificar si es macro
            const char* expansion = find_define(state, word);
            if (expansion) {
                strcat(result, expansion);
            } else {
                strcat(result, word);
            }
        } else {
            // Carácter normal - copiar tal cual
            char temp[2] = {*ptr, '\0'};
            strcat(result, temp);
            ptr++;
        }
    }
    
    return result;
}
// En preprocessor.c
int preprocess(const char* input_filename, const char* output_filename) {
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
    
    int success = preprocess_file(input_filename, output, &state);
    fclose(output);
    
    return success;
}
