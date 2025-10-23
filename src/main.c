#include <stdio.h>
#include <stdlib.h>
#include "preprocessor.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo.c>\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = "temp_preprocessed.c";
    
    printf("Preprocesando: %s\n", input_file);
    
    if (preprocess(input_file, output_file)) {
        printf("✅ Preprocesamiento exitoso!\n");
        printf("📁 Salida guardada en: %s\n", output_file);
        
        // Mostrar contenido
        printf("\n=== CONTENIDO PREPROCESADO ===\n");
        FILE *f = fopen(output_file, "r");
        if (f) {
            char line[1024];
            while (fgets(line, sizeof(line), f)) {
                printf("%s", line);
            }
            fclose(f);
        }
    } else {
        printf("❌ Error en preprocesamiento\n");
        return 1;
    }
    
    return 0;
}