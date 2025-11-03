%{
#include <stdio.h>
#include <string.h>
#include "tokens.h"
int line = 1;
int column = 1;
int yylval_int;
double yylval_double;
char yylval_char;
char* yylval_string;
%}

DIGITO      [0-9]
HEXA        (0[xX][0-9a-fA-F]+)
LETRA       [a-zA-Z_]
ID          {LETRA}({LETRA}|{DIGITO})*
ESPACIOS    [ \t]+
NUEVALINEA  (\r\n|\r|\n)
CADENA      \"([^\\\"]|\\.)*\"
CARACTER    \'([^\\\']|\\.)\'
COMENTLINE  \/\/.*
COMENTBLOCK \/\*([^*]|\*+[^*/])*\*+\/

%%
"auto"          { return TOKEN_AUTO; }
"break"         { return TOKEN_BREAK; }
"case"          { return TOKEN_CASE; }
"char"          { return TOKEN_CHAR; }
"const"         { return TOKEN_CONST; }
"continue"      { return TOKEN_CONTINUE; }
"default"       { return TOKEN_DEFAULT; }
"do"            { return TOKEN_DO; }
"double"        { return TOKEN_DOUBLE; }
"else"          { return TOKEN_ELSE; }
"enum"          { return TOKEN_ENUM; }
"extern"        { return TOKEN_EXTERN; }
"float"         { return TOKEN_FLOAT; }
"for"           { return TOKEN_FOR; }
"goto"          { return TOKEN_GOTO; }
"if"            { return TOKEN_IF; }
"int"           { return TOKEN_INT; }
"long"          { return TOKEN_LONG; }
"register"      { return TOKEN_REGISTER; }
"return"        { return TOKEN_RETURN; }
"short"         { return TOKEN_SHORT; }
"signed"        { return TOKEN_SIGNED; }
"sizeof"        { return TOKEN_SIZEOF; }
"static"        { return TOKEN_STATIC; }
"struct"        { return TOKEN_STRUCT; }
"switch"        { return TOKEN_SWITCH; }
"typedef"       { return TOKEN_TYPEDEF; }
"union"         { return TOKEN_UNION; }
"unsigned"      { return TOKEN_UNSIGNED; }
"void"          { return TOKEN_VOID; }
"volatile"      { return TOKEN_VOLATILE; }
"while"         { return TOKEN_WHILE; }
{ID}            { return TOKEN_ID; }
{HEXA}      { yylval_int = (int)strtol(yytext, NULL, 16); return TOKEN_CONST_ENTERO; }
{DIGITO}+  { yylval_int = atoi(yytext); return TOKEN_CONST_ENTERO; }
{DIGITO}+"."{DIGITO}+  { yylval_double = atof(yytext); return TOKEN_CONST_FLOTANTE; }
{CARACTER}  { yylval_char = yytext[1]; return TOKEN_CONST_CARACTER; } 
{CADENA}    { yylval_string = strdup(yytext); return TOKEN_CONST_CADENA; }
"+="    	{ return TOKEN_ADD_ASSIGN; }
"-="    	{ return TOKEN_SUB_ASSIGN; }
"*="    	{ return TOKEN_MUL_ASSIGN; }
"/="    	{ return TOKEN_DIV_ASSIGN; }
"%="    	{ return TOKEN_MOD_ASSIGN; }
"<<="   	{ return TOKEN_SHL_ASSIGN; }
">>="   	{ return TOKEN_SHR_ASSIGN; }
"&="    	{ return TOKEN_AND_ASSIGN; }
"^="    	{ return TOKEN_XOR_ASSIGN; }
"|="    	{ return TOKEN_OR_ASSIGN; }
"++"            { return TOKEN_INC; }
"--"            { return TOKEN_DEC; }
"?"		{ return TOKEN_TERN_IF; }
"+"             { return TOKEN_MAS; }
"%"             { return TOKEN_MOD; }
"-"             { return TOKEN_MENOS; }
"*"             { return TOKEN_MULT; }
"/"             { return TOKEN_DIV; }
"="             { return TOKEN_ASIGN; }
"=="            { return TOKEN_IGUAL; }
"!="            { return TOKEN_DIF; }
"<"             { return TOKEN_MENOR; }
">"             { return TOKEN_MAYOR; }
"<="            { return TOKEN_MENOR_IGUAL; }
">="            { return TOKEN_MAYOR_IGUAL; }
"&&"            { return TOKEN_AND; }
"||"            { return TOKEN_OR; }
"!"             { return TOKEN_NOT; }
"&"             { return TOKEN_AND_BIT; }
"|"             { return TOKEN_OR_BIT; }
"^"             { return TOKEN_XOR; }
"<<"            { return TOKEN_DESPL_IZQ; }
">>"            { return TOKEN_DESPL_DER; }
";"             { return TOKEN_PUNTO_COMA; }
":"             { return TOKEN_DOS_PUNTO; }
"#"		{ return TOKEN_GATO; }
","             { return TOKEN_COMA; }
"~"		{ return TOKEN_TILDE; }
"("             { return TOKEN_PARENT_IZQ; }
")"             { return TOKEN_PARENT_DER; }
"{"             { return TOKEN_LLAVE_IZQ; }
"}"             { return TOKEN_LLAVE_DER; }
"["             { return TOKEN_CORCH_IZQ; }
"]"             { return TOKEN_CORCH_DER; }
"."             { return TOKEN_PUNTO; }
"->"            { return TOKEN_FLECHA; }
{COMENTLINE}    {}
{COMENTBLOCK}   {}
{ESPACIOS}      { column += yyleng; }/*Sirve para mantener el orden de donde se encontraron las palabras*/
{NUEVALINEA}    { line++; column = 1; }
<<EOF>>		{ return TOKEN_EOF;}
.               { return TOKEN_ERROR; }
%%

Token Get_Token(void) {
    Token token;
    token.type = yylex();
    token.lexeme = strdup(yytext);
    token.line = line;
    token.column = column;

    switch (token.type) {
        case TOKEN_CONST_ENTERO:
            token.valor.valorEntero = yylval_int;
            break;
        case TOKEN_CONST_FLOTANTE:
            token.valor.valorFlotante = yylval_double;
            break;
        case TOKEN_CONST_CARACTER:
            token.valor.caracter = yylval_char;
            break;
        case TOKEN_CONST_CADENA:
            token.valor.cadena = yylval_string;
            break;
        default:
            break;
    }
    return token;
}
