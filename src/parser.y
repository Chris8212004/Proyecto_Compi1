%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void yyerror(const char *s);
int yylex(void);

extern FILE *yyin;

/* Variables globales para el manejo de errores */
char *filename = NULL;
%}

/* ======================
       ERRORES VERBOSE
   ====================== */
%define parse.error verbose
%locations

/* ======================
       DATOS DEL YACC
   ====================== */
%union {
    char* id;
    int entero;
    float real;
    char caracter;
    char* cadena;
}

/* ======================
          TOKENS
   ====================== */
%token TOKEN_AUTO TOKEN_DEFINE TOKEN_INCLUDE
%token TOKEN_BREAK TOKEN_CASE TOKEN_CHAR TOKEN_CONST TOKEN_CONTINUE
%token TOKEN_DEFAULT TOKEN_DO TOKEN_DOUBLE TOKEN_ELSE TOKEN_ENUM
%token TOKEN_EXTERN TOKEN_FLOAT TOKEN_FOR TOKEN_GOTO TOKEN_IF
%token TOKEN_INT TOKEN_LONG TOKEN_REGISTER TOKEN_RETURN TOKEN_SHORT
%token TOKEN_SIGNED TOKEN_SIZEOF TOKEN_STATIC TOKEN_STRUCT TOKEN_SWITCH
%token TOKEN_TYPEDEF TOKEN_UNION TOKEN_UNSIGNED TOKEN_VOID TOKEN_VOLATILE
%token TOKEN_WHILE

%token <id> TOKEN_ID
%token <entero> TOKEN_CONST_ENTERO
%token <real> TOKEN_CONST_FLOTANTE
%token <caracter> TOKEN_CONST_CARACTER
%token <cadena> TOKEN_CONST_CADENA

%token TOKEN_ADD_ASSIGN TOKEN_SUB_ASSIGN TOKEN_MUL_ASSIGN TOKEN_DIV_ASSIGN TOKEN_MOD_ASSIGN
%token TOKEN_SHL_ASSIGN TOKEN_SHR_ASSIGN TOKEN_AND_ASSIGN TOKEN_XOR_ASSIGN TOKEN_OR_ASSIGN

%token TOKEN_INC TOKEN_DEC TOKEN_TERN_IF
%token TOKEN_MAS TOKEN_MOD TOKEN_MENOS TOKEN_MULT TOKEN_DIV TOKEN_ASIGN
%token TOKEN_IGUAL TOKEN_DIF TOKEN_MENOR TOKEN_MAYOR TOKEN_MENOR_IGUAL TOKEN_MAYOR_IGUAL
%token TOKEN_AND TOKEN_OR TOKEN_NOT
%token TOKEN_AND_BIT TOKEN_OR_BIT TOKEN_XOR TOKEN_DESPL_IZQ TOKEN_DESPL_DER

%token TOKEN_PUNTO_COMA TOKEN_DOS_PUNTO TOKEN_GATO TOKEN_COMA TOKEN_TILDE
%token TOKEN_PARENT_IZQ TOKEN_PARENT_DER
%token TOKEN_LLAVE_IZQ TOKEN_LLAVE_DER
%token TOKEN_CORCH_IZQ TOKEN_CORCH_DER
%token TOKEN_PUNTO TOKEN_FLECHA
%token TOKEN_ERROR

/* ======================
          PRECEDENCIAS
   ====================== */

/* Para resolver dangling else */
%nonassoc LOWER_THAN_ELSE
%nonassoc TOKEN_ELSE

/* Precedencias aritméticas y lógicas */
%right TOKEN_ASIGN
%right TOKEN_TERN_IF

%left TOKEN_OR
%left TOKEN_AND

%left TOKEN_IGUAL TOKEN_DIF

%left TOKEN_MENOR TOKEN_MAYOR TOKEN_MENOR_IGUAL TOKEN_MAYOR_IGUAL

%left TOKEN_DESPL_IZQ TOKEN_DESPL_DER

%left TOKEN_MAS TOKEN_MENOS
%left TOKEN_MULT TOKEN_DIV TOKEN_MOD

%right TOKEN_NOT TOKEN_TILDE TOKEN_INC TOKEN_DEC
%right TOKEN_AND_BIT TOKEN_OR_BIT TOKEN_XOR

%left TOKEN_PUNTO TOKEN_FLECHA

/* ======================
          INICIO
   ====================== */
%start programa

%%

/* ======================
   PROGRAMA Y ARCHIVO
   ====================== */
programa:
    /* vacío */
    | unidades_traduccion
    ;

unidades_traduccion:
    unidad_traduccion
    | unidades_traduccion unidad_traduccion
    ;

unidad_traduccion:
    directiva_preprocesador
    | declaracion_externa
    ;

/* ======================
   DIRECTIVAS PREPROCESADOR
   ====================== */
directiva_preprocesador:
    TOKEN_GATO TOKEN_INCLUDE TOKEN_CONST_CADENA
    | TOKEN_GATO TOKEN_INCLUDE TOKEN_MENOR TOKEN_ID TOKEN_PUNTO TOKEN_ID TOKEN_MAYOR
    | TOKEN_GATO TOKEN_DEFINE TOKEN_ID
    | TOKEN_GATO TOKEN_DEFINE TOKEN_ID cuerpo_define
    ;

/* Cuerpo de un #define - puede ser macro con parámetros o secuencia de tokens */
cuerpo_define:
    macro_con_parametros
    | secuencia_tokens_define
    ;

/* Macro con parámetros: #define FOO(x,y) ... */
macro_con_parametros:
    TOKEN_PARENT_IZQ lista_parametros_macro TOKEN_PARENT_DER secuencia_tokens_define
    | TOKEN_PARENT_IZQ TOKEN_PARENT_DER secuencia_tokens_define
    ;

lista_parametros_macro:
    TOKEN_ID
    | lista_parametros_macro TOKEN_COMA TOKEN_ID
    ;

/* Secuencia de tokens para el cuerpo del define */
secuencia_tokens_define:
    token_define
    | secuencia_tokens_define token_define
    ;

token_define:
    TOKEN_ID
    | TOKEN_CONST_ENTERO
    | TOKEN_CONST_FLOTANTE
    | TOKEN_CONST_CARACTER
    | TOKEN_CONST_CADENA
    | TOKEN_PARENT_IZQ
    | TOKEN_PARENT_DER
    | TOKEN_CORCH_IZQ
    | TOKEN_CORCH_DER
    | TOKEN_MAS
    | TOKEN_MENOS
    | TOKEN_MULT
    | TOKEN_DIV
    | TOKEN_MOD
    | TOKEN_AND_BIT
    | TOKEN_OR_BIT
    | TOKEN_XOR
    | TOKEN_TILDE
    | TOKEN_NOT
    | TOKEN_IGUAL
    | TOKEN_DIF
    | TOKEN_MENOR
    | TOKEN_MAYOR
    | TOKEN_MENOR_IGUAL
    | TOKEN_MAYOR_IGUAL
    | TOKEN_AND
    | TOKEN_OR
    | TOKEN_DESPL_IZQ
    | TOKEN_DESPL_DER
    | TOKEN_INC
    | TOKEN_DEC
    | TOKEN_PUNTO
    | TOKEN_FLECHA
    | TOKEN_TERN_IF
    | TOKEN_DOS_PUNTO
    | TOKEN_COMA
    ;

/* ======================
   DECLARACIONES EXTERNAS
   ====================== */
declaracion_externa:
    declaracion_variable
    | definicion_funcion
    | declaracion_funcion
    ;

declaracion_variable:
    especificador_declaracion lista_declaradores TOKEN_PUNTO_COMA
    | declaracion_struct TOKEN_PUNTO_COMA
    | declaracion_enum TOKEN_PUNTO_COMA
    ;

/* Declaración de función (prototipo) */
declaracion_funcion:
    especificador_declaracion punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ lista_parametros TOKEN_PARENT_DER TOKEN_PUNTO_COMA
    | especificador_declaracion punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ TOKEN_PARENT_DER TOKEN_PUNTO_COMA
    | tipo_simple punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ lista_parametros TOKEN_PARENT_DER TOKEN_PUNTO_COMA
    | tipo_simple punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ TOKEN_PARENT_DER TOKEN_PUNTO_COMA
    ;

/* Especificador de declaración unificado - combina calificadores y tipo */
especificador_declaracion:
    tipo_simple
    | calificador tipo_simple
    | calificador calificador tipo_simple
    | calificador calificador calificador tipo_simple
    ;

calificador:
    TOKEN_CONST
    | TOKEN_STATIC
    | TOKEN_EXTERN
    | TOKEN_VOLATILE
    | TOKEN_AUTO
    | TOKEN_REGISTER
    ;

tipo_simple:
    TOKEN_VOID
    | TOKEN_CHAR
    | TOKEN_SHORT
    | TOKEN_INT
    | TOKEN_LONG
    | TOKEN_FLOAT
    | TOKEN_DOUBLE
    | TOKEN_SIGNED
    | TOKEN_UNSIGNED
    | TOKEN_SIGNED TOKEN_CHAR
    | TOKEN_UNSIGNED TOKEN_CHAR
    | TOKEN_SIGNED TOKEN_INT
    | TOKEN_UNSIGNED TOKEN_INT
    | TOKEN_SIGNED TOKEN_SHORT
    | TOKEN_UNSIGNED TOKEN_SHORT
    | TOKEN_SIGNED TOKEN_LONG
    | TOKEN_UNSIGNED TOKEN_LONG
    | TOKEN_LONG TOKEN_LONG
    | TOKEN_STRUCT TOKEN_ID
    | TOKEN_ENUM TOKEN_ID
    ;

punteros_opcionales:
    /* vacío */
    | TOKEN_MULT
    | TOKEN_MULT TOKEN_MULT
    | TOKEN_MULT TOKEN_MULT TOKEN_MULT
    ;

lista_declaradores:
    declarador
    | lista_declaradores TOKEN_COMA declarador
    ;

declarador:
    punteros_opcionales declarador_directo
    | punteros_opcionales declarador_directo TOKEN_ASIGN inicializador
    ;

declarador_directo:
    TOKEN_ID
    | TOKEN_ID TOKEN_CORCH_IZQ expresion_condicional TOKEN_CORCH_DER
    | TOKEN_ID TOKEN_CORCH_IZQ TOKEN_CORCH_DER
    ;

inicializador:
    expresion_asignacion
    | TOKEN_LLAVE_IZQ lista_inicializadores TOKEN_LLAVE_DER
    | TOKEN_LLAVE_IZQ lista_inicializadores TOKEN_COMA TOKEN_LLAVE_DER
    ;

lista_inicializadores:
    inicializador
    | lista_inicializadores TOKEN_COMA inicializador
    ;

/* ======================
   DEFINICIÓN DE FUNCIÓN
   ====================== */
definicion_funcion:
    especificador_declaracion punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ lista_parametros TOKEN_PARENT_DER bloque_compuesto
    | especificador_declaracion punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ TOKEN_PARENT_DER bloque_compuesto
    | tipo_simple punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ lista_parametros TOKEN_PARENT_DER bloque_compuesto
    | tipo_simple punteros_opcionales TOKEN_ID TOKEN_PARENT_IZQ TOKEN_PARENT_DER bloque_compuesto
    ;

lista_parametros:
    parametro
    | lista_parametros TOKEN_COMA parametro
    ;

parametro:
    especificador_declaracion punteros_opcionales TOKEN_ID
    | especificador_declaracion punteros_opcionales TOKEN_ID TOKEN_CORCH_IZQ TOKEN_CORCH_DER
    | especificador_declaracion punteros_opcionales
    | tipo_simple punteros_opcionales TOKEN_ID
    | tipo_simple punteros_opcionales TOKEN_ID TOKEN_CORCH_IZQ TOKEN_CORCH_DER
    | tipo_simple punteros_opcionales
    ;

/* ======================
   STRUCT Y ENUM
   ====================== */
declaracion_struct:
    TOKEN_STRUCT TOKEN_ID TOKEN_LLAVE_IZQ miembros_struct TOKEN_LLAVE_DER
    | TOKEN_STRUCT TOKEN_LLAVE_IZQ miembros_struct TOKEN_LLAVE_DER
    ;

miembros_struct:
    miembro_struct
    | miembros_struct miembro_struct
    ;

miembro_struct:
    especificador_declaracion lista_declaradores_struct TOKEN_PUNTO_COMA
    | tipo_simple lista_declaradores_struct TOKEN_PUNTO_COMA
    ;

lista_declaradores_struct:
    punteros_opcionales declarador_directo
    | lista_declaradores_struct TOKEN_COMA punteros_opcionales declarador_directo
    ;

declaracion_enum:
    TOKEN_ENUM TOKEN_ID TOKEN_LLAVE_IZQ lista_enumeradores TOKEN_LLAVE_DER
    | TOKEN_ENUM TOKEN_LLAVE_IZQ lista_enumeradores TOKEN_LLAVE_DER
    ;

lista_enumeradores:
    enumerador
    | lista_enumeradores TOKEN_COMA enumerador
    ;

enumerador:
    TOKEN_ID
    | TOKEN_ID TOKEN_ASIGN TOKEN_CONST_ENTERO
    ;

/* ======================
   SENTENCIAS
   ====================== */
sentencia:
    sentencia_expresion
    | bloque_compuesto
    | sentencia_if
    | sentencia_switch
    | sentencia_while
    | sentencia_do
    | sentencia_for
    | sentencia_salto
    | sentencia_case
    | sentencia_default
    | declaracion_variable
    ;

sentencia_expresion:
    TOKEN_PUNTO_COMA
    | expresion TOKEN_PUNTO_COMA
    ;

bloque_compuesto:
    TOKEN_LLAVE_IZQ TOKEN_LLAVE_DER
    | TOKEN_LLAVE_IZQ lista_sentencias TOKEN_LLAVE_DER
    ;

lista_sentencias:
    sentencia
    | lista_sentencias sentencia
    ;

/* ======================
   SENTENCIAS DE CONTROL
   ====================== */
sentencia_if:
    TOKEN_IF TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER sentencia %prec LOWER_THAN_ELSE
    | TOKEN_IF TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER sentencia TOKEN_ELSE sentencia
    ;

sentencia_switch:
    TOKEN_SWITCH TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER sentencia
    ;

sentencia_while:
    TOKEN_WHILE TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER sentencia
    ;

sentencia_do:
    TOKEN_DO sentencia TOKEN_WHILE TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER TOKEN_PUNTO_COMA
    ;

sentencia_for:
    TOKEN_FOR TOKEN_PARENT_IZQ expresion_for_init TOKEN_PUNTO_COMA expresion_opcional TOKEN_PUNTO_COMA expresion_opcional TOKEN_PARENT_DER sentencia
    ;

expresion_for_init:
    /* vacío */
    | expresion
    | especificador_declaracion lista_declaradores
    | tipo_simple lista_declaradores
    ;

expresion_opcional:
    /* vacío */
    | expresion
    ;

/* ======================
   SENTENCIAS DE SALTO
   ====================== */
sentencia_salto:
    TOKEN_GOTO TOKEN_ID TOKEN_PUNTO_COMA
    | TOKEN_CONTINUE TOKEN_PUNTO_COMA
    | TOKEN_BREAK TOKEN_PUNTO_COMA
    | TOKEN_RETURN TOKEN_PUNTO_COMA
    | TOKEN_RETURN expresion TOKEN_PUNTO_COMA
    ;

sentencia_case:
    TOKEN_CASE expresion_condicional TOKEN_DOS_PUNTO
    | TOKEN_CASE expresion_condicional TOKEN_DOS_PUNTO sentencia
    ;

sentencia_default:
    TOKEN_DEFAULT TOKEN_DOS_PUNTO
    | TOKEN_DEFAULT TOKEN_DOS_PUNTO sentencia
    ;

/* ======================
   EXPRESIONES
   ====================== */
expresion:
    expresion_asignacion
    | expresion TOKEN_COMA expresion_asignacion
    ;

expresion_asignacion:
    expresion_condicional
    | expresion_unaria operador_asignacion expresion_asignacion
    ;

operador_asignacion:
    TOKEN_ASIGN
    | TOKEN_ADD_ASSIGN
    | TOKEN_SUB_ASSIGN
    | TOKEN_MUL_ASSIGN
    | TOKEN_DIV_ASSIGN
    | TOKEN_MOD_ASSIGN
    | TOKEN_SHL_ASSIGN
    | TOKEN_SHR_ASSIGN
    | TOKEN_AND_ASSIGN
    | TOKEN_XOR_ASSIGN
    | TOKEN_OR_ASSIGN
    ;

expresion_condicional:
    expresion_logica_or
    | expresion_logica_or TOKEN_TERN_IF expresion TOKEN_DOS_PUNTO expresion_condicional
    ;

expresion_logica_or:
    expresion_logica_and
    | expresion_logica_or TOKEN_OR expresion_logica_and
    ;

expresion_logica_and:
    expresion_or_bit
    | expresion_logica_and TOKEN_AND expresion_or_bit
    ;

expresion_or_bit:
    expresion_xor_bit
    | expresion_or_bit TOKEN_OR_BIT expresion_xor_bit
    ;

expresion_xor_bit:
    expresion_and_bit
    | expresion_xor_bit TOKEN_XOR expresion_and_bit
    ;

expresion_and_bit:
    expresion_igualdad
    | expresion_and_bit TOKEN_AND_BIT expresion_igualdad
    ;

expresion_igualdad:
    expresion_relacional
    | expresion_igualdad TOKEN_IGUAL expresion_relacional
    | expresion_igualdad TOKEN_DIF expresion_relacional
    ;

expresion_relacional:
    expresion_desplazamiento
    | expresion_relacional TOKEN_MENOR expresion_desplazamiento
    | expresion_relacional TOKEN_MAYOR expresion_desplazamiento
    | expresion_relacional TOKEN_MENOR_IGUAL expresion_desplazamiento
    | expresion_relacional TOKEN_MAYOR_IGUAL expresion_desplazamiento
    ;

expresion_desplazamiento:
    expresion_aditiva
    | expresion_desplazamiento TOKEN_DESPL_IZQ expresion_aditiva
    | expresion_desplazamiento TOKEN_DESPL_DER expresion_aditiva
    ;

expresion_aditiva:
    expresion_multiplicativa
    | expresion_aditiva TOKEN_MAS expresion_multiplicativa
    | expresion_aditiva TOKEN_MENOS expresion_multiplicativa
    ;

expresion_multiplicativa:
    expresion_cast
    | expresion_multiplicativa TOKEN_MULT expresion_cast
    | expresion_multiplicativa TOKEN_DIV expresion_cast
    | expresion_multiplicativa TOKEN_MOD expresion_cast
    ;

expresion_cast:
    expresion_unaria
    | TOKEN_PARENT_IZQ tipo_simple TOKEN_PARENT_DER expresion_cast
    | TOKEN_PARENT_IZQ tipo_simple TOKEN_MULT TOKEN_PARENT_DER expresion_cast
    ;

expresion_unaria:
    expresion_postfija
    | TOKEN_INC expresion_unaria
    | TOKEN_DEC expresion_unaria
    | operador_unario expresion_cast
    | TOKEN_SIZEOF expresion_unaria
    | TOKEN_SIZEOF TOKEN_PARENT_IZQ tipo_simple TOKEN_PARENT_DER
    ;

operador_unario:
    TOKEN_AND_BIT
    | TOKEN_MULT
    | TOKEN_MAS
    | TOKEN_MENOS
    | TOKEN_TILDE
    | TOKEN_NOT
    ;

expresion_postfija:
    expresion_primaria
    | expresion_postfija TOKEN_CORCH_IZQ expresion TOKEN_CORCH_DER
    | expresion_postfija TOKEN_PARENT_IZQ TOKEN_PARENT_DER
    | expresion_postfija TOKEN_PARENT_IZQ lista_argumentos TOKEN_PARENT_DER
    | expresion_postfija TOKEN_PUNTO TOKEN_ID
    | expresion_postfija TOKEN_FLECHA TOKEN_ID
    | expresion_postfija TOKEN_INC
    | expresion_postfija TOKEN_DEC
    ;

lista_argumentos:
    expresion_asignacion
    | lista_argumentos TOKEN_COMA expresion_asignacion
    ;

expresion_primaria:
    TOKEN_ID
    | TOKEN_CONST_ENTERO
    | TOKEN_CONST_FLOTANTE
    | TOKEN_CONST_CARACTER
    | TOKEN_CONST_CADENA
    | TOKEN_PARENT_IZQ expresion TOKEN_PARENT_DER
    ;

%%

/* ======================
   FUNCIÓN DE ERROR
   ====================== */
void read_line_from_file(int line_num, char *buffer, size_t size) {
    if (!filename) {
        buffer[0] = '\0';
        return;
    }
    
    FILE *f = fopen(filename, "r");
    if (!f) {
        buffer[0] = '\0';
        return;
    }
    
    int current = 1;
    while (fgets(buffer, size, f)) {
        if (current == line_num) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
            }
            if (len > 1 && buffer[len-2] == '\r') {
                buffer[len-2] = '\0';
            }
            fclose(f);
            return;
        }
        current++;
    }
    fclose(f);
    buffer[0] = '\0';
}

const char* token_to_symbol(const char* token_name) {
    if (strstr(token_name, "TOKEN_PUNTO_COMA")) return "';'";
    if (strstr(token_name, "TOKEN_DOS_PUNTO")) return "':'";
    if (strstr(token_name, "TOKEN_COMA")) return "','";
    if (strstr(token_name, "TOKEN_PUNTO")) return "'.'";
    if (strstr(token_name, "TOKEN_GATO")) return "'#'";
    if (strstr(token_name, "TOKEN_TILDE")) return "'~'";
    
    if (strstr(token_name, "TOKEN_PARENT_IZQ")) return "'('";
    if (strstr(token_name, "TOKEN_PARENT_DER")) return "')'";
    if (strstr(token_name, "TOKEN_LLAVE_IZQ")) return "'{'";
    if (strstr(token_name, "TOKEN_LLAVE_DER")) return "'}'";
    if (strstr(token_name, "TOKEN_CORCH_IZQ")) return "'['";
    if (strstr(token_name, "TOKEN_CORCH_DER")) return "']'";
    
    if (strstr(token_name, "TOKEN_ADD_ASSIGN")) return "'+='";
    if (strstr(token_name, "TOKEN_SUB_ASSIGN")) return "'-='";
    if (strstr(token_name, "TOKEN_MUL_ASSIGN")) return "'*='";
    if (strstr(token_name, "TOKEN_DIV_ASSIGN")) return "'/='";
    if (strstr(token_name, "TOKEN_MOD_ASSIGN")) return "'%='";
    if (strstr(token_name, "TOKEN_SHL_ASSIGN")) return "'<<='";
    if (strstr(token_name, "TOKEN_SHR_ASSIGN")) return "'>>='";
    if (strstr(token_name, "TOKEN_AND_ASSIGN")) return "'&='";
    if (strstr(token_name, "TOKEN_XOR_ASSIGN")) return "'^='";
    if (strstr(token_name, "TOKEN_OR_ASSIGN")) return "'|='";
    
    if (strstr(token_name, "TOKEN_INC")) return "'++'";
    if (strstr(token_name, "TOKEN_DEC")) return "'--'";
    if (strstr(token_name, "TOKEN_FLECHA")) return "'->'";
    if (strstr(token_name, "TOKEN_TERN_IF")) return "'?'";
    if (strstr(token_name, "TOKEN_MAS")) return "'+'";
    if (strstr(token_name, "TOKEN_MENOS")) return "'-'";
    if (strstr(token_name, "TOKEN_MULT")) return "'*'";
    if (strstr(token_name, "TOKEN_DIV")) return "'/'";
    if (strstr(token_name, "TOKEN_MOD")) return "'%'";
    if (strstr(token_name, "TOKEN_ASIGN")) return "'='";
    
    if (strstr(token_name, "TOKEN_IGUAL")) return "'=='";
    if (strstr(token_name, "TOKEN_DIF")) return "'!='";
    if (strstr(token_name, "TOKEN_MENOR_IGUAL")) return "'<='";
    if (strstr(token_name, "TOKEN_MAYOR_IGUAL")) return "'>='";
    if (strstr(token_name, "TOKEN_MENOR")) return "'<'";
    if (strstr(token_name, "TOKEN_MAYOR")) return "'>'";
    
    if (strstr(token_name, "TOKEN_AND")) return "'&&'";
    if (strstr(token_name, "TOKEN_OR")) return "'||'";
    if (strstr(token_name, "TOKEN_NOT")) return "'!'";
    if (strstr(token_name, "TOKEN_AND_BIT")) return "'&'";
    if (strstr(token_name, "TOKEN_OR_BIT")) return "'|'";
    if (strstr(token_name, "TOKEN_XOR")) return "'^'";
    if (strstr(token_name, "TOKEN_DESPL_IZQ")) return "'<<'";
    if (strstr(token_name, "TOKEN_DESPL_DER")) return "'>>'";
    
    if (strstr(token_name, "TOKEN_AUTO")) return "'auto'";
    if (strstr(token_name, "TOKEN_BREAK")) return "'break'";
    if (strstr(token_name, "TOKEN_CASE")) return "'case'";
    if (strstr(token_name, "TOKEN_CHAR")) return "'char'";
    if (strstr(token_name, "TOKEN_CONST")) return "'const'";
    if (strstr(token_name, "TOKEN_CONTINUE")) return "'continue'";
    if (strstr(token_name, "TOKEN_DEFAULT")) return "'default'";
    if (strstr(token_name, "TOKEN_DEFINE")) return "'define'";
    if (strstr(token_name, "TOKEN_DO")) return "'do'";
    if (strstr(token_name, "TOKEN_DOUBLE")) return "'double'";
    if (strstr(token_name, "TOKEN_ELSE")) return "'else'";
    if (strstr(token_name, "TOKEN_ENUM")) return "'enum'";
    if (strstr(token_name, "TOKEN_EXTERN")) return "'extern'";
    if (strstr(token_name, "TOKEN_FLOAT")) return "'float'";
    if (strstr(token_name, "TOKEN_FOR")) return "'for'";
    if (strstr(token_name, "TOKEN_GOTO")) return "'goto'";
    if (strstr(token_name, "TOKEN_IF")) return "'if'";
    if (strstr(token_name, "TOKEN_INCLUDE")) return "'include'";
    if (strstr(token_name, "TOKEN_INT")) return "'int'";
    if (strstr(token_name, "TOKEN_LONG")) return "'long'";
    if (strstr(token_name, "TOKEN_REGISTER")) return "'register'";
    if (strstr(token_name, "TOKEN_RETURN")) return "'return'";
    if (strstr(token_name, "TOKEN_SHORT")) return "'short'";
    if (strstr(token_name, "TOKEN_SIGNED")) return "'signed'";
    if (strstr(token_name, "TOKEN_SIZEOF")) return "'sizeof'";
    if (strstr(token_name, "TOKEN_STATIC")) return "'static'";
    if (strstr(token_name, "TOKEN_STRUCT")) return "'struct'";
    if (strstr(token_name, "TOKEN_SWITCH")) return "'switch'";
    if (strstr(token_name, "TOKEN_TYPEDEF")) return "'typedef'";
    if (strstr(token_name, "TOKEN_UNION")) return "'union'";
    if (strstr(token_name, "TOKEN_UNSIGNED")) return "'unsigned'";
    if (strstr(token_name, "TOKEN_VOID")) return "'void'";
    if (strstr(token_name, "TOKEN_VOLATILE")) return "'volatile'";
    if (strstr(token_name, "TOKEN_WHILE")) return "'while'";
    
    if (strstr(token_name, "TOKEN_ID")) return "identifier";
    if (strstr(token_name, "TOKEN_CONST_ENTERO")) return "integer constant";
    if (strstr(token_name, "TOKEN_CONST_FLOTANTE")) return "floating constant";
    if (strstr(token_name, "TOKEN_CONST_CARACTER")) return "character constant";
    if (strstr(token_name, "TOKEN_CONST_CADENA")) return "string literal";
    if (strstr(token_name, "TOKEN_ERROR")) return "invalid token";
    
    return token_name;
}

char* replace_tokens_in_message(const char* original) {
    static char result[1024];
    const char *pos = original;
    result[0] = '\0';
    char buffer[1024];
    int buf_idx = 0;
    
    while (*pos) {
        if (strncmp(pos, "TOKEN_", 6) == 0) {
            const char *token_start = pos;
            pos += 6;
            
            char token_name[128] = "TOKEN_";
            int i = 6;
            while (*pos && (isalnum(*pos) || *pos == '_')) {
                token_name[i++] = *pos++;
            }
            token_name[i] = '\0';
            
            buffer[buf_idx] = '\0';
            strcat(result, buffer);
            buf_idx = 0;
            
            strcat(result, token_to_symbol(token_name));
        } else {
            buffer[buf_idx++] = *pos++;
        }
    }
    
    buffer[buf_idx] = '\0';
    strcat(result, buffer);
    
    return result;
}

void yyerror(const char *s) {
    char line_content[1024];
    read_line_from_file(yylloc.first_line, line_content, sizeof(line_content));
    
    char *clean_message = replace_tokens_in_message(s);
    
    fprintf(stderr, "%s:%d:%d: error: %s\n",
            filename ? filename : "input",
            yylloc.first_line,
            yylloc.first_column,
            clean_message);
    
    if (line_content[0] != '\0') {
        fprintf(stderr, " %s\n", line_content);
        
        fprintf(stderr, " ");
        for (int i = 1; i < yylloc.first_column; i++) {
            fprintf(stderr, " ");
        }
        fprintf(stderr, "^\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo.c>\n", argv[0]);
        return 1;
    }

    filename = argv[1];
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("❌ Error: no se puede abrir el archivo '%s'\n", filename);
        return 1;
    }

    yyin = file;

    printf("Compilador COMPI Iniciado...\n");

    int result = yyparse();

    if (result == 0)
        printf("✔ Código válido\n");
    else
        printf("✖ Código inválido\n");

    fclose(file);
    return result;
}
