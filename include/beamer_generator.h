#ifndef BEAMER_GENERATOR_H
#define BEAMER_GENERATOR_H

#include <stdio.h>
#include "tokens.h"

void generar_portada(FILE* beamer);
void generar_flex(FILE* beamer);
void generar_codigo(FILE* beamer);
void generar_histograma(FILE* beamer);
void generar_beamer( int cnt_pClaves, int cnt_id, int cnt_cons, int cnt_op, int cnt_puntu);
const char* get_token_color(TokenType type);
void generar_pie(FILE* beamer);

#endif
