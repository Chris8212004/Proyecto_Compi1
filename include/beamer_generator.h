#ifndef BEAMER_GENERATOR_H
#define BEAMER_GENERATOR_H

#include <stdio.h>
#include "tokens.h"

void generar_portada(FILE* beamer);
void generar_flex(FILE* beamer);
void generar_codigo(FILE* beamer, const char* preprocessed_file);
void generar_histograma(FILE* beamer, const char* token_stats);
void generar_beamer(const char* preprocessed_file, const char* token_stats);
const char* get_token_color(TokenType type);

#endif