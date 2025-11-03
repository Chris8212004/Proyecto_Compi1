#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
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
    "TOKEN_MOD",
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
    "TOKEN_TERN_IF",
    "TOKEN_ADD_ASSIGN",
    "TOKEN_SUB_ASSIGN",
    "TOKEN_MUL_ASSIGN",
    "TOKEN_DIV_ASSIGN" ,
    "TOKEN_MOD_ASSIGN" ,
    "TOKEN_SHL_ASSIGN" ,
    "TOKEN_SHR_ASSIGN" ,
    "TOKEN_AND_ASSIGN" ,
    "TOKEN_XOR_ASSIGN",
    "TOKEN_OR_ASSIGN",
    "TOKEN_PUNTO_COMA",
    "TOKEN_GATO",
    "TOKEN_TILDE",
    "TOKEN_DOS_PUNTO",
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
int max_tokens = 1000000;

// Función para guardar tokens
void save_token(Token token) {
    if (all_tokens == NULL) {
        all_tokens = malloc(max_tokens * sizeof(Token));
    }
    
    if (total_tokens_captured < max_tokens) {
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
    
    
    if (preprocess(input_file, output_file)) {

        yyin = fopen(output_file, "r");
        if (!yyin) {
            perror("Error al abrir el archivo preprocesado");
            return 1;
        }

        Token token;
        int total_tokens = 0;
        int token_counts[85] = {0};
        
        // Reinicializar el array de tokens
        all_tokens = NULL;
        total_tokens_captured = 0;
        
        while ((token = Get_Token()).type != TOKEN_EOF) {

            save_token(token);
            
            // Contar tokens para estadísticas
            if (token.type < 85) {
                token_counts[token.type]++;
            }
            total_tokens++;
            
           
        }

        fclose(yyin);

        // Guardar tokens en archivo para el Beamer
        FILE* token_file = fopen("all_tokens.dat", "w");
        if (token_file) {
            for (int i = 0; i < total_tokens_captured; i++) {
                fprintf(token_file, "%dª%sª%dª%d\n", 
                        all_tokens[i].type, 
                        all_tokens[i].lexeme,
                        all_tokens[i].line,
                        all_tokens[i].column);
            }
            fclose(token_file);
        }

        
        
        int palabras_clave =token_counts[TOKEN_AUTO] + token_counts[TOKEN_BREAK] + token_counts[TOKEN_CASE] +
                                token_counts[TOKEN_CHAR] + token_counts[TOKEN_CONST] + token_counts[TOKEN_CONTINUE] +
                                token_counts[TOKEN_DEFAULT] + token_counts[TOKEN_DO] + token_counts[TOKEN_DOUBLE] +
                                token_counts[TOKEN_ELSE] + token_counts[TOKEN_ENUM] + token_counts[TOKEN_EXTERN] +
                                token_counts[TOKEN_FLOAT] + token_counts[TOKEN_FOR] + token_counts[TOKEN_GOTO] +
                                token_counts[TOKEN_IF] + token_counts[TOKEN_INT] + token_counts[TOKEN_LONG] +
                                token_counts[TOKEN_REGISTER] + token_counts[TOKEN_RETURN] + token_counts[TOKEN_SHORT] +
                                token_counts[TOKEN_SIGNED] + token_counts[TOKEN_SIZEOF] +
                                token_counts[TOKEN_STATIC] + token_counts[TOKEN_STRUCT] + token_counts[TOKEN_SWITCH] +
                                token_counts[TOKEN_TYPEDEF] + token_counts[TOKEN_UNION] + token_counts[TOKEN_UNSIGNED] +
                                token_counts[TOKEN_VOID] + token_counts[TOKEN_VOLATILE] +token_counts[TOKEN_WHILE];

            
            int constantes = token_counts[TOKEN_CONST_ENTERO] + token_counts[TOKEN_CONST_FLOTANTE] +
                           token_counts[TOKEN_CONST_CARACTER] + token_counts[TOKEN_CONST_CADENA];
            
            int operadores = token_counts[TOKEN_MAS] + token_counts[TOKEN_MENOS] + token_counts[TOKEN_MULT] +
                           token_counts[TOKEN_DIV] + token_counts[TOKEN_ASIGN] + token_counts[TOKEN_IGUAL] +
                           token_counts[TOKEN_DIF] + token_counts[TOKEN_MENOR] + token_counts[TOKEN_MAYOR] +
                           token_counts[TOKEN_MENOR_IGUAL] + token_counts[TOKEN_MAYOR_IGUAL] + token_counts[TOKEN_AND] +
                           token_counts[TOKEN_OR] + token_counts[TOKEN_NOT] + token_counts[TOKEN_AND_BIT] +
                           token_counts[TOKEN_OR_BIT] + token_counts[TOKEN_XOR] + token_counts[TOKEN_DESPL_IZQ] +
                           token_counts[TOKEN_DESPL_DER] + token_counts[TOKEN_INC] + token_counts[TOKEN_DEC] +
                           token_counts[TOKEN_ADD_ASSIGN] + token_counts[TOKEN_SUB_ASSIGN] + token_counts[TOKEN_MUL_ASSIGN] +
                           token_counts[TOKEN_DIV_ASSIGN] + token_counts[TOKEN_SHL_ASSIGN] + token_counts[TOKEN_SHR_ASSIGN] +
                           token_counts[TOKEN_AND_ASSIGN] + token_counts[TOKEN_XOR_ASSIGN] + token_counts[TOKEN_MOD_ASSIGN] +
                           token_counts[TOKEN_OR_ASSIGN] + token_counts[TOKEN_TERN_IF] + token_counts[TOKEN_MOD];
            
            int puntuacion = token_counts[TOKEN_PUNTO_COMA] + token_counts[TOKEN_COMA] +
                           token_counts[TOKEN_PARENT_IZQ] + token_counts[TOKEN_PARENT_DER] +
                           token_counts[TOKEN_LLAVE_IZQ] + token_counts[TOKEN_LLAVE_DER] +
                           token_counts[TOKEN_CORCH_IZQ] + token_counts[TOKEN_CORCH_DER] +
                           token_counts[TOKEN_PUNTO] + token_counts[TOKEN_FLECHA] + token_counts[TOKEN_DOS_PUNTO] +
                           token_counts[TOKEN_TILDE] + token_counts[TOKEN_GATO]                         ;
            
            

        generar_beamer(palabras_clave, token_counts[TOKEN_ID], constantes, operadores, puntuacion);

        
        // Liberar memoria de tokens
        if (all_tokens) {
            for (int i = 0; i < total_tokens_captured; i++) {
                free(all_tokens[i].lexeme);
            }
            free(all_tokens);
            all_tokens = NULL;
        }
        
    } else {
        printf("Error en preprocesamiento\n");
        return 1;
    }
    
    return 0;
}
