#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "preprocessor.h"

static int process_include_directive(PreprocessorState* state, const char* line, FILE* output) {
    char filename[MAX_FILENAME];
    
    // Buscar para #include "archivo" (comillas dobles)
    const char* quote = strchr(line, '"');
    // Buscar para #include <archivo> (comillas angulares)
    const char* angle = strchr(line, '<');
    
    if (!quote && !angle) {
        fprintf(stderr, "Error: #include sin comillas o <> en línea %d\n", state->current_line);
        return 0;
    }
    
    const char* start, *end;
    int is_system_header = 0;
    
    if (quote) {
        // #include "archivo.h"
        start = quote + 1;
        end = strchr(start, '"');
    } else {
        // #include <archivo.h>
        start = angle + 1;
        end = strchr(start, '>');
        is_system_header = 1;
    }
    
    if (!end) {
        fprintf(stderr, "Error: #include sin cierre en línea %d\n", state->current_line);
        return 0;
    }
    
    int len = end - start;
    strncpy(filename, start, len);
    filename[len] = '\0';
    
    printf("Buscando archivo: %s (system: %d)\n", filename, is_system_header);
    
    // Si es sistema (#include <...>), NO procesar recursivamente
    if (is_system_header) {
        printf("Ignorando header de sistema: %s\n", filename);
        return 1;  // Success pero no procesar recursivamente
    }
    
    // Para #include "..." buscar en directorios locales
    FILE* test_file = fopen(filename, "r");
    if (test_file) {
        fclose(test_file);
        return preprocess_file(filename, output, state);
    }
    
    char include_path[MAX_FILENAME];
    snprintf(include_path, sizeof(include_path), "include/%s", filename);
    test_file = fopen(include_path, "r");
    if (test_file) {
        fclose(test_file);
        return preprocess_file(include_path, output, state);
    }
    
    fprintf(stderr, "Error: No se puede encontrar archivo %s\n", filename);
    return 0;
}

int preprocess_file(const char* filename, FILE* output, PreprocessorState* state) {
    printf("Procesando archivo: %s\n", filename);

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
void process_define_directive(PreprocessorState* state, const char* line) {
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
        text = result;  // Usar el resultado como nueva entrada para siguiente iteración
        
    } while (expanded);  // Repetir hasta que no haya más expansiones
    
    return result;
}
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
