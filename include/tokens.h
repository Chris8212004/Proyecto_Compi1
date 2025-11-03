#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_ERROR = 0,
    TOKEN_EOF,
    
    // Palabras reservadas
    TOKEN_AUTO,
    TOKEN_BREAK,
    TOKEN_CASE,
    TOKEN_CHAR,
    TOKEN_CONST,
    TOKEN_CONTINUE,
    TOKEN_DEFAULT,
    TOKEN_DO,
    TOKEN_DOUBLE,
    TOKEN_ELSE,
    TOKEN_ENUM,
    TOKEN_EXTERN,
    TOKEN_FLOAT,
    TOKEN_FOR,
    TOKEN_GOTO,
    TOKEN_IF,
    TOKEN_INT,
    TOKEN_LONG,
    TOKEN_REGISTER,
    TOKEN_RETURN,
    TOKEN_SHORT,
    TOKEN_SIGNED,
    TOKEN_SIZEOF,
    TOKEN_STATIC,
    TOKEN_STRUCT,
    TOKEN_SWITCH,
    TOKEN_TYPEDEF,
    TOKEN_UNION,
    TOKEN_UNSIGNED,
    TOKEN_VOID,
    TOKEN_VOLATILE,
    TOKEN_WHILE,
    
    // Identificadores y constantes
    TOKEN_ID,
    TOKEN_CONST_ENTERO,
    TOKEN_CONST_FLOTANTE,
    TOKEN_CONST_CARACTER,
    TOKEN_CONST_CADENA,
    
    // Operadores
    TOKEN_MAS,
    TOKEN_MENOS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_MOD,
    TOKEN_ASIGN,
    TOKEN_IGUAL,
    TOKEN_DIF,
    TOKEN_MENOR,
    TOKEN_MAYOR,
    TOKEN_MENOR_IGUAL,
    TOKEN_MAYOR_IGUAL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_AND_BIT,
    TOKEN_OR_BIT,
    TOKEN_XOR,
    TOKEN_DESPL_IZQ,
    TOKEN_DESPL_DER,
    TOKEN_INC,
    TOKEN_DEC,
    TOKEN_TERN_IF,
    
    //Operadores asignacion
    TOKEN_ADD_ASSIGN,
    TOKEN_SUB_ASSIGN,
    TOKEN_MUL_ASSIGN,
    TOKEN_DIV_ASSIGN,
    TOKEN_MOD_ASSIGN,
    TOKEN_SHL_ASSIGN,
    TOKEN_SHR_ASSIGN,
    TOKEN_AND_ASSIGN,
    TOKEN_XOR_ASSIGN,
    TOKEN_OR_ASSIGN,
    // Puntuación
    TOKEN_PUNTO_COMA,
    TOKEN_GATO,
    TOKEN_TILDE,
    TOKEN_DOS_PUNTO,
    TOKEN_COMA,
    TOKEN_PARENT_IZQ,
    TOKEN_PARENT_DER,
    TOKEN_LLAVE_IZQ,
    TOKEN_LLAVE_DER,
    TOKEN_CORCH_IZQ,
    TOKEN_CORCH_DER,
    TOKEN_PUNTO,
    TOKEN_FLECHA
    
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
    int line;
    int column;
    union {
        int valorEntero;
        double valorFlotante;
        char caracter;
        char* cadena;
    } valor;
} Token;


#endif
