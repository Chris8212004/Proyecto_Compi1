#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    // Palabras reservadas
    TOKEN_INT = 1,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_VOID,
    TOKEN_DO,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_DEFAULT,
    
    // Identificadores
    TOKEN_ID,
    
    // Constantes
    TOKEN_CONST_ENTERO,      // 123, 0xFF, 077
    TOKEN_CONST_FLOTANTE,    // 3.14, 2.5e-3
    TOKEN_CONST_CARACTER,    // 'a', '\n'
    TOKEN_CONST_CADENA,      // "hola mundo"
    
    // Operadores
    TOKEN_MAS,              // +
    TOKEN_MENOS,            // -
    TOKEN_MULT,             // *
    TOKEN_DIV,              // /
    TOKEN_ASIGN,            // =
    TOKEN_IGUAL,            // ==
    TOKEN_DIF,              // !=
    TOKEN_MENOR,            // <
    TOKEN_MAYOR,            // >
    TOKEN_MENOR_IGUAL,      // <=
    TOKEN_MAYOR_IGUAL,      // >=
    TOKEN_AND,              // &&
    TOKEN_OR,               // ||
    TOKEN_NOT,              // !
    
    // Operadores de bits
    TOKEN_AND_BIT,          // &
    TOKEN_OR_BIT,           // |
    TOKEN_XOR,              // ^
    TOKEN_DESPL_IZQ,        // <<
    TOKEN_DESPL_DER,        // >>
    
    // Incremento/Decremento
    TOKEN_INC,              // ++
    TOKEN_DEC,              // --
    
    // Puntuación
    TOKEN_PUNTO_COMA,       // ;
    TOKEN_COMA,             // ,
    TOKEN_PARENT_IZQ,       // (
    TOKEN_PARENT_DER,       // )
    TOKEN_LLAVE_IZQ,        // {
    TOKEN_LLAVE_DER,        // }
    TOKEN_CORCH_IZQ,        // [
    TOKEN_CORCH_DER,        // ]
    TOKEN_PUNTO,            // .
    TOKEN_FLECHA,           // ->
    
    // Especiales
    TOKEN_EOF,              // Fin de archivo
    TOKEN_ERROR             // Error léxico
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;           // Cadena del token
    int line;              // Línea donde aparece
    int column;            // Columna donde aparece
    union {
        int int_value;       // Para constantes enteras
        float float_value;   // Para constantes flotantes
        char* string_value;  // Para cadenas
        char char_value;     // Para caracteres
    } value;
} Token;

// Prototipo de la función principal del scanner
Token Get_Token(void);

#endif