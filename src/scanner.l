%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
extern YYLTYPE yylloc;
int line = 1;
int column = 1;
int debug_lex = 1;


/* ================= MACROS PARA REDUCIR CÓDIGO ================= */
#define YYSETLOC() yylloc.first_line = line; yylloc.first_column = column; column += yyleng
#define YYTOKEN(tok) {YYSETLOC(); yylval.id = strdup(yytext); DEBUG_TOKEN(#tok); return tok;}
#define YYLEX_ID(tok) {YYSETLOC(); yylval.id = strdup(yytext); DEBUG_TOKEN(#tok); return tok;}
#define YYLEX_INT(tok,val) {YYSETLOC(); yylval.entero = val; DEBUG_TOKEN(#tok); return tok;}
#define YYLEX_FLOAT(tok,val) {YYSETLOC(); yylval.real = val; DEBUG_TOKEN(#tok); return tok;}
#define YYLEX_CHAR(tok,val) {YYSETLOC(); yylval.caracter = val; DEBUG_TOKEN(#tok); return tok;}
#define YYLEX_STR(tok) {YYSETLOC(); yylval.cadena = strdup(yytext); DEBUG_TOKEN(#tok); return tok;}


#define DEBUG_TOKEN(tok) if(debug_lex) printf("[LEX] %-25s => %-30s (line %d, col %d)\n", yytext, tok, line, column);
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


"auto"          {YYTOKEN(TOKEN_AUTO);}
"define"        {YYTOKEN(TOKEN_DEFINE);}
"include"       {YYTOKEN(TOKEN_INCLUDE);}
"break"         {YYTOKEN(TOKEN_BREAK);}
"case"          {YYTOKEN(TOKEN_CASE);}
"char"          {YYTOKEN(TOKEN_CHAR);}
"const"         {YYTOKEN(TOKEN_CONST);}
"continue"      {YYTOKEN(TOKEN_CONTINUE);}
"default"       {YYTOKEN(TOKEN_DEFAULT);}
"do"            {YYTOKEN(TOKEN_DO);}
"double"        {YYTOKEN(TOKEN_DOUBLE);}
"else"          {YYTOKEN(TOKEN_ELSE);}
"enum"          {YYTOKEN(TOKEN_ENUM);}
"extern"        {YYTOKEN(TOKEN_EXTERN);}
"float"         {YYTOKEN(TOKEN_FLOAT);}
"for"           {YYTOKEN(TOKEN_FOR);}
"goto"          {YYTOKEN(TOKEN_GOTO);}
"if"            {YYTOKEN(TOKEN_IF);}
"int"           {YYTOKEN(TOKEN_INT);}
"long"          {YYTOKEN(TOKEN_LONG);}
"register"      {YYTOKEN(TOKEN_REGISTER);}
"return"        {YYTOKEN(TOKEN_RETURN);}
"short"         {YYTOKEN(TOKEN_SHORT);}
"signed"        {YYTOKEN(TOKEN_SIGNED);}
"sizeof"        {YYTOKEN(TOKEN_SIZEOF);}
"static"        {YYTOKEN(TOKEN_STATIC);}
"struct"        {YYTOKEN(TOKEN_STRUCT);}
"switch"        {YYTOKEN(TOKEN_SWITCH);}
"typedef"       {YYTOKEN(TOKEN_TYPEDEF);}
"union"         {YYTOKEN(TOKEN_UNION);}
"unsigned"      {YYTOKEN(TOKEN_UNSIGNED);}
"void"          {YYTOKEN(TOKEN_VOID);}
"volatile"      {YYTOKEN(TOKEN_VOLATILE);}
"while"         {YYTOKEN(TOKEN_WHILE);}


{ID}            {YYLEX_ID(TOKEN_ID);}
{HEXA}          {YYLEX_INT(TOKEN_CONST_ENTERO, strtol(yytext,NULL,16));}
{DIGITO}+       {YYLEX_INT(TOKEN_CONST_ENTERO, atoi(yytext));}
{DIGITO}+"."{DIGITO}+ {YYLEX_FLOAT(TOKEN_CONST_FLOTANTE, atof(yytext));}
{CARACTER}      {YYLEX_CHAR(TOKEN_CONST_CARACTER, yytext[1]);}
{CADENA}        {YYLEX_STR(TOKEN_CONST_CADENA);}


"+="            {YYTOKEN(TOKEN_ADD_ASSIGN);}
"-="            {YYTOKEN(TOKEN_SUB_ASSIGN);}
"*="            {YYTOKEN(TOKEN_MUL_ASSIGN);}
"/="            {YYTOKEN(TOKEN_DIV_ASSIGN);}
"%="            {YYTOKEN(TOKEN_MOD_ASSIGN);}
"<<="           {YYTOKEN(TOKEN_SHL_ASSIGN);}
">>="           {YYTOKEN(TOKEN_SHR_ASSIGN);}
"&="            {YYTOKEN(TOKEN_AND_ASSIGN);}
"^="            {YYTOKEN(TOKEN_XOR_ASSIGN);}
"|="            {YYTOKEN(TOKEN_OR_ASSIGN);}


"++"            {YYTOKEN(TOKEN_INC);}
"--"            {YYTOKEN(TOKEN_DEC);}
"?"             {YYTOKEN(TOKEN_TERN_IF);}
"+"             {YYTOKEN(TOKEN_MAS);}
"%"             {YYTOKEN(TOKEN_MOD);}
"-"             {YYTOKEN(TOKEN_MENOS);}
"*"             {YYTOKEN(TOKEN_MULT);}
"/"             {YYTOKEN(TOKEN_DIV);}
"="             {YYTOKEN(TOKEN_ASIGN);}
"=="            {YYTOKEN(TOKEN_IGUAL);}
"!="            {YYTOKEN(TOKEN_DIF);}
"<"             {YYTOKEN(TOKEN_MENOR);}
">"             {YYTOKEN(TOKEN_MAYOR);}
"<="            {YYTOKEN(TOKEN_MENOR_IGUAL);}
">="            {YYTOKEN(TOKEN_MAYOR_IGUAL);}
"&&"            {YYTOKEN(TOKEN_AND);}
"||"            {YYTOKEN(TOKEN_OR);}
"!"             {YYTOKEN(TOKEN_NOT);}
"&"             {YYTOKEN(TOKEN_AND_BIT);}
"|"             {YYTOKEN(TOKEN_OR_BIT);}
"^"             {YYTOKEN(TOKEN_XOR);}
"<<"            {YYTOKEN(TOKEN_DESPL_IZQ);}
">>"            {YYTOKEN(TOKEN_DESPL_DER);}
";"             {YYTOKEN(TOKEN_PUNTO_COMA);}
":"             {YYTOKEN(TOKEN_DOS_PUNTO);}
"#"             {YYTOKEN(TOKEN_GATO);}
","             {YYTOKEN(TOKEN_COMA);}
"~"             {YYTOKEN(TOKEN_TILDE);}
"("             {YYTOKEN(TOKEN_PARENT_IZQ);}
")"             {YYTOKEN(TOKEN_PARENT_DER);}
"{"             {YYTOKEN(TOKEN_LLAVE_IZQ);}
"}"             {YYTOKEN(TOKEN_LLAVE_DER);}
"["             {YYTOKEN(TOKEN_CORCH_IZQ);}
"]"             {YYTOKEN(TOKEN_CORCH_DER);}
"."             {YYTOKEN(TOKEN_PUNTO);}
"->"            {YYTOKEN(TOKEN_FLECHA);}


{COMENTLINE}    { /* ignorar */ }
{COMENTBLOCK}   { /* ignorar */ }
{ESPACIOS}      { column += yyleng; }
{NUEVALINEA}    { line++; column = 1; }


. {
    yylloc.first_line = line;
    yylloc.first_column = column;
    column += yyleng;

    yylval.id = strdup(yytext);  /* Guardar como error */
    DEBUG_TOKEN("TOKEN_ERROR");
    return TOKEN_ERROR;
}


<<EOF>> { return 0; }

%%

int yywrap() { return 1; }
