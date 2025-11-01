#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "tokens.h"
#include "beamer_generator.h"

// Estas variables vienen de Flex
extern FILE *yyin;
extern int yylex(void);
extern char *yytext;
Token Get_Token(void);

const char* token_names[] = {
    "TOKEN_ERROR",
    "TOKEN_EOF",
    "TOKEN_AUTO",
    "TOKEN_BREAK",
    "TOKEN_CASE",
    "TOKEN_CHAR",
    "TOKEN_CONST",
    "TOKEN_CONTINUE",
    "TOKEN_DEFAULT",
    "TOKEN_DO",
    "TOKEN_DOUBLE",
    "TOKEN_ELSE",
    "TOKEN_ENUM",
    "TOKEN_EXTERN",
    "TOKEN_FLOAT",
    "TOKEN_FOR",
    "TOKEN_GOTO",
    "TOKEN_IF",
    "TOKEN_INT",
    "TOKEN_LONG",
    "TOKEN_REGISTER",
    "TOKEN_RETURN",
    "TOKEN_SHORT",
    "TOKEN_SIGNED",
    "TOKEN_SIZEOF",
    "TOKEN_STATIC",
    "TOKEN_STRUCT",
    "TOKEN_SWITCH",
    "TOKEN_TYPEDEF",
    "TOKEN_UNION",
    "TOKEN_UNSIGNED",
    "TOKEN_VOID",
    "TOKEN_VOLATILE",
    "TOKEN_WHILE",
    "TOKEN_ID",
    "TOKEN_CONST_ENTERO",
    "TOKEN_CONST_FLOTANTE",
    "TOKEN_CONST_CARACTER",
    "TOKEN_CONST_CADENA",
    "TOKEN_MAS",
    "TOKEN_MENOS",
    "TOKEN_MULT",
    "TOKEN_DIV",
    "TOKEN_ASIGN",
    "TOKEN_IGUAL",
    "TOKEN_DIF",
    "TOKEN_MENOR",
    "TOKEN_MAYOR",
    "TOKEN_MENOR_IGUAL",
    "TOKEN_MAYOR_IGUAL",
    "TOKEN_AND",
    "TOKEN_OR",
    "TOKEN_NOT",
    "TOKEN_AND_BIT",
    "TOKEN_OR_BIT",
    "TOKEN_XOR",
    "TOKEN_DESPL_IZQ",
    "TOKEN_DESPL_DER",
    "TOKEN_INC",
    "TOKEN_DEC",
    "TOKEN_PUNTO_COMA",
    "TOKEN_COMA",
    "TOKEN_PARENT_IZQ",
    "TOKEN_PARENT_DER",
    "TOKEN_LLAVE_IZQ",
    "TOKEN_LLAVE_DER",
    "TOKEN_CORCH_IZQ",
    "TOKEN_CORCH_DER",
    "TOKEN_PUNTO",
    "TOKEN_FLECHA"
};

// Variables para capturar tokens
Token* all_tokens = NULL;
int total_tokens_captured = 0;
int max_tokens = 1000;

// Función para guardar tokens
void save_token(Token token) {
    if (all_tokens == NULL) {
        all_tokens = malloc(max_tokens * sizeof(Token));
    }
    
    if (total_tokens_captured < max_tokens) {
        // Copiar el token (importante: duplicar el lexema)
        all_tokens[total_tokens_captured].type = token.type;
        all_tokens[total_tokens_captured].lexeme = strdup(token.lexeme);
        all_tokens[total_tokens_captured].line = token.line;
        all_tokens[total_tokens_captured].column = token.column;
        total_tokens_captured++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo.c>\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = "temp_preprocessed.c";
    const char* token_stats_file = "token_stats.dat";
    
    printf("Preprocesando: %s\n", input_file);
    
    if (preprocess(input_file, output_file)) {
        printf("✅ Preprocesamiento exitoso!\n");
        printf("📁 Salida guardada en: %s\n", output_file);

        // Mostrar contenido preprocesado
        printf("\n=== CONTENIDO PREPROCESADO ===\n");
        FILE *f = fopen(output_file, "r");
        if (f) {
            char line[1024];
            while (fgets(line, sizeof(line), f)) {
                printf("%s", line);
            }
            fclose(f);
        }

        // Ejecutar scanner sobre el código preprocesado
        printf("\n=== INICIANDO SCANNER ===\n");
        yyin = fopen(output_file, "r");
        if (!yyin) {
            perror("Error al abrir el archivo preprocesado");
            return 1;
        }

        Token token;
        int total_tokens = 0;
        int token_counts[70] = {0};
        
        // Reinicializar el array de tokens
        all_tokens = NULL;
        total_tokens_captured = 0;
        
        printf("🔍 Procesando tokens...\n");
        while ((token = Get_Token()).type != TOKEN_EOF) {
            printf("Token: %-20s Lexema: %-15s Linea: %d Columna: %d\n",
                   token_names[token.type], token.lexeme, token.line, token.column);
            
            // Guardar token para el Beamer
            save_token(token);
            
            // Contar tokens para estadísticas
            if (token.type < 70) {
                token_counts[token.type]++;
            }
            total_tokens++;
            
            // NO liberar aquí - se libera después de guardar en archivo
            // free(token.lexeme);
        }

        fclose(yyin);

        // Guardar tokens en archivo para el Beamer
        printf("\n=== GUARDANDO TOKENS PARA BEAMER ===\n");
        FILE* token_file = fopen("all_tokens.dat", "w");
        if (token_file) {
            printf("📝 Escribiendo %d tokens en all_tokens.dat...\n", total_tokens_captured);
            for (int i = 0; i < total_tokens_captured; i++) {
                fprintf(token_file, "%d|%s|%d|%d\n", 
                        all_tokens[i].type, 
                        all_tokens[i].lexeme,
                        all_tokens[i].line,
                        all_tokens[i].column);
            }
            fclose(token_file);
            printf("✅ Tokens guardados: %d\n", total_tokens_captured);
        } else {
            printf("❌ Error: No se pudo crear archivo de tokens\n");
        }

        // Generar archivo de estadísticas para el Beamer
        printf("\n=== GENERANDO ESTADÍSTICAS PARA BEAMER ===\n");
        FILE* stats = fopen(token_stats_file, "w");
        if (stats) {
            // Calcular estadísticas por categoría
            int palabras_clave = token_counts[TOKEN_INT] + token_counts[TOKEN_FLOAT] + token_counts[TOKEN_CHAR] +
                               token_counts[TOKEN_IF] + token_counts[TOKEN_ELSE] + token_counts[TOKEN_FOR] +
                               token_counts[TOKEN_WHILE] + token_counts[TOKEN_RETURN] + token_counts[TOKEN_VOID] +
                               token_counts[TOKEN_DO] + token_counts[TOKEN_SWITCH] + token_counts[TOKEN_CASE] +
                               token_counts[TOKEN_BREAK] + token_counts[TOKEN_CONTINUE] + token_counts[TOKEN_DEFAULT];
            
            int constantes = token_counts[TOKEN_CONST_ENTERO] + token_counts[TOKEN_CONST_FLOTANTE] +
                           token_counts[TOKEN_CONST_CARACTER] + token_counts[TOKEN_CONST_CADENA];
            
            int operadores = token_counts[TOKEN_MAS] + token_counts[TOKEN_MENOS] + token_counts[TOKEN_MULT] +
                           token_counts[TOKEN_DIV] + token_counts[TOKEN_ASIGN] + token_counts[TOKEN_IGUAL] +
                           token_counts[TOKEN_DIF] + token_counts[TOKEN_MENOR] + token_counts[TOKEN_MAYOR] +
                           token_counts[TOKEN_MENOR_IGUAL] + token_counts[TOKEN_MAYOR_IGUAL] + token_counts[TOKEN_AND] +
                           token_counts[TOKEN_OR] + token_counts[TOKEN_NOT] + token_counts[TOKEN_AND_BIT] +
                           token_counts[TOKEN_OR_BIT] + token_counts[TOKEN_XOR] + token_counts[TOKEN_DESPL_IZQ] +
                           token_counts[TOKEN_DESPL_DER] + token_counts[TOKEN_INC] + token_counts[TOKEN_DEC];
            
            int puntuacion = token_counts[TOKEN_PUNTO_COMA] + token_counts[TOKEN_COMA] +
                           token_counts[TOKEN_PARENT_IZQ] + token_counts[TOKEN_PARENT_DER] +
                           token_counts[TOKEN_LLAVE_IZQ] + token_counts[TOKEN_LLAVE_DER] +
                           token_counts[TOKEN_CORCH_IZQ] + token_counts[TOKEN_CORCH_DER] +
                           token_counts[TOKEN_PUNTO] + token_counts[TOKEN_FLECHA];
            
            fprintf(stats, "PalabrasClave %d\n", palabras_clave);
            fprintf(stats, "Identificadores %d\n", token_counts[TOKEN_ID]);
            fprintf(stats, "Constantes %d\n", constantes);
            fprintf(stats, "Operadores %d\n", operadores);
            fprintf(stats, "Puntuacion %d\n", puntuacion);
            fprintf(stats, "Total %d\n", total_tokens);
            fclose(stats);
            
            printf("📊 Estadísticas guardadas:\n");
            printf("   - Palabras clave: %d\n", palabras_clave);
            printf("   - Identificadores: %d\n", token_counts[TOKEN_ID]);
            printf("   - Constantes: %d\n", constantes);
            printf("   - Operadores: %d\n", operadores);
            printf("   - Puntuación: %d\n", puntuacion);
            printf("   - Total tokens: %d\n", total_tokens);
        }

        generar_beamer(output_file, token_stats_file);

        
        // Liberar memoria de tokens
        if (all_tokens) {
            for (int i = 0; i < total_tokens_captured; i++) {
                free(all_tokens[i].lexeme);
            }
            free(all_tokens);
            all_tokens = NULL;
        }
        
    } else {
        printf("❌ Error en preprocesamiento\n");
        return 1;
    }
    
    return 0;
}
