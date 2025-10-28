#include <stdio.h>
#include <stdlib.h>
#include "preprocessor.h"
#include "tokens.h"//Creo que si es importante tenerlo aqui idk

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

        // 🔥 NUEVO: ejecutar scanner sobre el código preprocesado
        printf("\n=== INICIANDO SCANNER ===\n");
        yyin = fopen(output_file, "r");
        if (!yyin) {
            perror("Error al abrir el archivo preprocesado");
            return 1;
        }

        Token token;
        while ((token = Get_Token()).type != 0) { // 0 = EOF
            printf("Token: %-20s Lexema: %-15s Linea: %d Columna: %d\n",
       token_names[token.type], token.lexeme, token.line, token.column);
            free(token.lexeme);
        }

        fclose(yyin);
    } else {
        printf("❌ Error en preprocesamiento\n");
        return 1;
    }
    
    return 0;
}

