#ifndef TOKENS_H
#define TOKENS_H
//Los tokens provisionales, realmente no se si son todos
typedef enum {
    TOKEN_ERROR,
    TOKEN_EOF,
    // Palabras clave C89
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

    // Identificadores
    TOKEN_ID,

    // Constantes
    TOKEN_CONST_ENTERO,
    TOKEN_CONST_FLOTANTE,
    TOKEN_CONST_CARACTER,
    TOKEN_CONST_CADENA,

    // Operadores
    TOKEN_MAS,
    TOKEN_MENOS,
    TOKEN_MULT,
    TOKEN_DIV,
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

    // Operadores de bits
    TOKEN_AND_BIT,
    TOKEN_OR_BIT,
    TOKEN_XOR,
    TOKEN_DESPL_IZQ,
    TOKEN_DESPL_DER,

    // Incremento/Decremento
    TOKEN_INC,
    TOKEN_DEC,

    // Puntuación
    TOKEN_PUNTO_COMA,
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
        int int_value;
        float float_value;
        char char_value;
        char* string_value;
    } value;
} Token;

Token Get_Token(void);

#endif
