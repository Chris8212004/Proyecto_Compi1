#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "beamer_generator.h"
#include "tokens.h"

void generar_portada(FILE* beamer) {
    fprintf(beamer, "\\begin{frame}\n");
    fprintf(beamer, "    \\centering\n");
    fprintf(beamer, "    \\vspace{0.5cm}\n");
    fprintf(beamer, "    {\\Huge\\bfseries Analizador Léxico}\\\\\n");
    fprintf(beamer, "    \\vspace{0.3cm}\n");
    fprintf(beamer, "    {\\Large Proyecto 1 - Compiladores e Intérpretes}\\\\\n");
    fprintf(beamer, "    \\vspace{0.5cm}\n");
    fprintf(beamer, "    {\\large Instituto Tecnológico de Costa Rica}\\\\\n");
    fprintf(beamer, "    \\vspace{1cm}\n");
    fprintf(beamer, "    {\\large\\bfseries Grupo de Trabajo:}\\\\\n");
    fprintf(beamer, "    {\\large Cristopher Céspedes}\\\\\n");
    fprintf(beamer, "    {\\large Christopher Alvarado}\\\\\n");
    fprintf(beamer, "    {\\large Jesús Solís Mora}\\\\\n");
    fprintf(beamer, "    \\vspace{1cm}\n");
    fprintf(beamer, "    {\\large Semestre: II 2025}\\\\\n");
    fprintf(beamer, "\\end{frame}\n\n");
}

void generar_flex(FILE* beamer) {
    // Slide 1: Introducción a Flex
    fprintf(beamer, "\\begin{frame}{Herramienta Flex}\n");
    fprintf(beamer, "    \\begin{block}{¿Qué es Flex?}\n");
    fprintf(beamer, "        Flex (Fast Lexical Analyzer Generator) es una herramienta para generar analizadores léxicos\\\\\n");
    fprintf(beamer, "        que convierte texto fuente en una secuencia de tokens.\n");
    fprintf(beamer, "    \\end{block}\n");
    fprintf(beamer, "    \\begin{itemize}\n");
    fprintf(beamer, "        \\item Desarrollado en C\n");
    fprintf(beamer, "        \\item Usa expresiones regulares para definir patrones\n");
    fprintf(beamer, "        \\item Genera código C eficiente y portable\n");
    fprintf(beamer, "        \\item Ampliamente usado en compiladores e intérpretes\n");
    fprintf(beamer, "    \\end{itemize}\n");
    fprintf(beamer, "\\end{frame}\n\n");

    // Slide 2: Proceso de Scanning 
    fprintf(beamer, "\\begin{frame}{Proceso de Scanning}\n");
    fprintf(beamer, "    \\vspace{0.5cm}\n");
    fprintf(beamer, "    \\begin{enumerate}\n");
    fprintf(beamer, "        \\item El scanner lee el texto carácter por carácter\n");
    fprintf(beamer, "        \\item Identifica patrones usando expresiones regulares\n");
    fprintf(beamer, "        \\item Genera tokens con información del lexema\n");
    fprintf(beamer, "        \\item Maneja errores léxicos cuando encuentra patrones no válidos\n");
    fprintf(beamer, "    \\end{enumerate}\n");
    fprintf(beamer, "\\end{frame}\n\n");
}

const char* get_token_color(TokenType type) {
    switch (type) {
        // Palabras reservadas - AZUL
        case TOKEN_AUTO:
        case TOKEN_BREAK:
        case TOKEN_CASE:
        case TOKEN_CHAR:
        case TOKEN_CONST:
        case TOKEN_CONTINUE:
        case TOKEN_DEFAULT:
        case TOKEN_DO:
        case TOKEN_DOUBLE:
        case TOKEN_ELSE:
        case TOKEN_ENUM:
        case TOKEN_EXTERN:
        case TOKEN_FLOAT:
        case TOKEN_FOR:
        case TOKEN_GOTO:
        case TOKEN_IF:
        case TOKEN_INT:
        case TOKEN_LONG:
        case TOKEN_REGISTER:
        case TOKEN_RETURN:
        case TOKEN_SHORT:
        case TOKEN_SIGNED:
        case TOKEN_SIZEOF:
        case TOKEN_STATIC:
        case TOKEN_STRUCT:
        case TOKEN_SWITCH:
        case TOKEN_TYPEDEF:
        case TOKEN_UNION:
        case TOKEN_UNSIGNED:
        case TOKEN_VOID:
        case TOKEN_VOLATILE:
        case TOKEN_WHILE:
            return "blue";
        
        // Identificadores - ROJO
        case TOKEN_ID:
            return "red";
        
        // Constantes - NARANJA
        case TOKEN_CONST_ENTERO:
        case TOKEN_CONST_FLOTANTE:
        case TOKEN_CONST_CARACTER:
        case TOKEN_CONST_CADENA:
            return "orange";
        
        // Operadores - MORADO
        case TOKEN_MAS:
        case TOKEN_MENOS:
        case TOKEN_MULT:
        case TOKEN_DIV:
        case TOKEN_ASIGN:
        case TOKEN_IGUAL:
        case TOKEN_DIF:
        case TOKEN_MENOR:
        case TOKEN_MAYOR:
        case TOKEN_MENOR_IGUAL:
        case TOKEN_MAYOR_IGUAL:
        case TOKEN_AND:
        case TOKEN_OR:
        case TOKEN_NOT:
        case TOKEN_AND_BIT:
        case TOKEN_OR_BIT:
        case TOKEN_XOR:
        case TOKEN_DESPL_IZQ:
        case TOKEN_DESPL_DER:
        case TOKEN_INC:
        case TOKEN_DEC:
            return "purple";
        
        // Puntuación - VERDE AZUL (INCLUYENDO LLAVES)
        case TOKEN_PUNTO_COMA:
        case TOKEN_COMA:
        case TOKEN_PARENT_IZQ:
        case TOKEN_PARENT_DER:
        case TOKEN_LLAVE_IZQ:
        case TOKEN_LLAVE_DER:
        case TOKEN_CORCH_IZQ:
        case TOKEN_CORCH_DER:
        case TOKEN_PUNTO:
        case TOKEN_FLECHA:
            return "teal";
        
        // Por defecto - negro (incluye TOKEN_ERROR y TOKEN_EOF)
        case TOKEN_ERROR:
        case TOKEN_EOF:
        default:
            return "black";
    }
}

char* escape_latex_complete(const char* input) {
    if (!input) return NULL;
    
    size_t len = strlen(input);
    size_t max_len = len * 12 + 1;
    char* result = malloc(max_len);
    if (!result) return NULL;
    
    size_t pos = 0;
    
    for (size_t i = 0; i < len && pos < max_len - 10; i++) {
        unsigned char c = input[i];
        
        switch (c) {
            case '\\': 
                // Manejar secuencias de escape de C
                if (i + 1 < len) {
                    char next = input[i + 1];
                    if (next == 'n' || next == 't' || next == 'r' || next == '"' || next == '\\') {
                        // Mantener secuencias de escape de C intactas
                        result[pos++] = '\\';
                        result[pos++] = next;
                        i++; // Saltar el siguiente carácter ya que lo procesamos
                        break;
                    }
                }
                strcpy(result + pos, "\\textbackslash{}");
                pos += 16;
                break;
            case '{':  
                strcpy(result + pos, "\\{");
                pos += 2;
                break;
            case '}':  
                strcpy(result + pos, "\\}");
                pos += 2;
                break;
            case '&':  
                strcpy(result + pos, "\\&");
                pos += 2;
                break;
            case '%':  
                strcpy(result + pos, "\\%");  
                pos += 2;
                break;
            case '$':  
                strcpy(result + pos, "\\$");
                pos += 2;
                break;
            case '#':  
                strcpy(result + pos, "\\#");
                pos += 2;
                break;
            case '_':  
                strcpy(result + pos, "\\_");
                pos += 2;
                break;
            case '^':  
                strcpy(result + pos, "\\^{}");
                pos += 4;
                break;
            case '~':  
                strcpy(result + pos, "\\~{}");
                pos += 4;
                break;
            case '<':  
                strcpy(result + pos, "\\textless{}");
                pos += 11;
                break;
            case '>':  
                strcpy(result + pos, "\\textgreater{}");
                pos += 14;
                break;
            case '|':  
                strcpy(result + pos, "\\textbar{}");
                pos += 10;
                break;
            default:
                // Caracteres normales
                result[pos++] = c;
                break;
        }
    }
    
    result[pos] = '\0';
    return result;
}

void generar_codigo(FILE* beamer, const char* preprocessed_file) {
    // Leer tokens desde all_tokens.dat
    Token* tokens = NULL;
    int token_count = 0;
    int max_tokens = 10000;
    FILE* token_file = fopen("all_tokens.dat", "r");

    if (!token_file) {
        fprintf(beamer, "\\begin{frame}[fragile]{Código Preprocesado - Coloreado por Scanner}\n");
        fprintf(beamer, "\\begin{center}\n");
        fprintf(beamer, "{\\color{red}No se pudieron cargar los tokens desde all_tokens.dat}\n");
        fprintf(beamer, "\\end{center}\n");
        fprintf(beamer, "\\end{frame}\n\n");
        return;
    }

    tokens = malloc(max_tokens * sizeof(Token));
    if (!tokens) {
        printf("❌ Error de memoria\n");
        fclose(token_file);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), token_file) && token_count < max_tokens) {
        char* token_type_str = strtok(line, "|");
        char* lexeme = strtok(NULL, "|");
        char* line_str = strtok(NULL, "|");
        char* column_str = strtok(NULL, "|");

        if (token_type_str && lexeme) {
            lexeme[strcspn(lexeme, "\r\n")] = '\0';
            tokens[token_count].type = atoi(token_type_str);
            tokens[token_count].lexeme = strdup(lexeme);
            tokens[token_count].line = line_str ? atoi(line_str) : 1;
            tokens[token_count].column = column_str ? atoi(column_str) : 1;
            token_count++;
        }
    }
    fclose(token_file);
    

    if (token_count == 0) {
        fprintf(beamer, "\\begin{frame}[fragile]{Código Preprocesado - Coloreado por Scanner}\n");
        fprintf(beamer, "\\begin{center}\n");
        fprintf(beamer, "No se encontraron tokens para mostrar\n");
        fprintf(beamer, "\\end{center}\n");
        fprintf(beamer, "\\end{frame}\n\n");
        free(tokens);
        return;
    }

    // Encontrar el número máximo de líneas
    int max_line = 0;
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].line > max_line) {
            max_line = tokens[i].line;
        }
    }
    

    // Dividir en slides cada 20 líneas
    int lines_per_slide = 20;
    int total_slides = (max_line + lines_per_slide - 1) / lines_per_slide;
    

    for (int slide_num = 0; slide_num < total_slides; slide_num++) {
        int start_line = slide_num * lines_per_slide + 1;
        int end_line = (slide_num + 1) * lines_per_slide;
        
        if (end_line > max_line) {
            end_line = max_line;
        }

        
        fprintf(beamer, "\\begin{frame}[fragile]{Código Preprocesado - Líneas %d a %d}\n", start_line, end_line);
        fprintf(beamer, "\\scriptsize\n");
        fprintf(beamer, "\\begin{flushleft}\n");
        
        int current_line = start_line;
        int current_column = 1;
        bool started_current_line = false;

        for (int i = 0; i < token_count; i++) {
            // Saltar tokens fuera del rango de este slide
            if (tokens[i].line < start_line || tokens[i].line > end_line) {
                continue;
            }

            // Manejar cambio de línea 
            if (tokens[i].line > current_line) {
                // Completar líneas vacías entre tokens
                while (current_line < tokens[i].line) {
                    fprintf(beamer, "\\\\\n");
                    current_line++;
                    current_column = 1;
                    started_current_line = false;
                }
            }

            // indentación
            if (!started_current_line && current_column < tokens[i].column) {
                for (int s = current_column; s < tokens[i].column; s++) {
                    fprintf(beamer, "\\ ");
                }
                current_column = tokens[i].column;
            }

            // Obtener color según el tipo
            const char* color = get_token_color(tokens[i].type);

            // Escape para LaTeX
            char* escaped = escape_latex_complete(tokens[i].lexeme);
            
            // Aplicar formato según el tipo de token
            if (tokens[i].type == TOKEN_ID) {
                fprintf(beamer, "{\\color{%s}\\textit{%s}}", color, escaped);
            } else if (tokens[i].type >= TOKEN_AUTO && tokens[i].type <= TOKEN_WHILE) {
                fprintf(beamer, "{\\color{%s}\\textbf{%s}}", color, escaped);
            } else if (tokens[i].type >= TOKEN_CONST_ENTERO && tokens[i].type <= TOKEN_CONST_CADENA) {
                if (tokens[i].type == TOKEN_CONST_CADENA || tokens[i].type == TOKEN_CONST_CARACTER) {
                    fprintf(beamer, "{\\color{%s}\\verb|%s|}", color, tokens[i].lexeme);
                } else {
                    fprintf(beamer, "{\\color{%s}\\texttt{%s}}", color, escaped);
                }
            } else if (tokens[i].type >= TOKEN_MAS && tokens[i].type <= TOKEN_DEC) {
                fprintf(beamer, "{\\color{%s}\\textbf{%s}}", color, escaped);
            } else if (tokens[i].type >= TOKEN_PUNTO_COMA && tokens[i].type <= TOKEN_FLECHA) {
                fprintf(beamer, "{\\color{%s}\\textbf{%s}}", color, escaped);
            } else {
                fprintf(beamer, "{\\color{%s}%s}", color, escaped);
            }
            
            if (escaped) {
                free(escaped);
            }

            // Lógica para espacios entre tokens
            if (i < token_count - 1) {
                Token* next_token = &tokens[i + 1];
                
                // Solo considerar si el siguiente token está en el mismo slide y línea
                if (next_token->line >= start_line && next_token->line <= end_line && 
                    next_token->line == tokens[i].line) {
                    
                    int needs_space = 1;
                    
                    // Tokens después de los cuales NO debe haber espacio
                    if (next_token->type == TOKEN_PUNTO_COMA ||
                        next_token->type == TOKEN_COMA ||
                        next_token->type == TOKEN_PARENT_DER ||
                        next_token->type == TOKEN_LLAVE_DER ||
                        next_token->type == TOKEN_CORCH_DER ||
                        next_token->type == TOKEN_PARENT_IZQ ||
                        next_token->type == TOKEN_LLAVE_IZQ||
                        next_token->type == TOKEN_CORCH_IZQ ||
                        next_token->type == TOKEN_PUNTO ||
                        next_token->type == TOKEN_DIV ||
                        next_token->type == TOKEN_MAYOR ||
                        next_token->type == TOKEN_MAYOR_IGUAL ||
                        next_token->type == TOKEN_MENOR ||
                        next_token->type == TOKEN_MENOR_IGUAL ||
                        next_token->type == TOKEN_PUNTO ||
                        next_token->type == TOKEN_FLECHA) {
                        needs_space = 0;
                    }
                    
                    // Tokens antes de los cuales NO debe haber espacio  
                    if (tokens[i].type == TOKEN_PARENT_IZQ ||
                        tokens[i].type == TOKEN_LLAVE_IZQ ||
                        tokens[i].type == TOKEN_CORCH_IZQ ||
                        tokens[i].type == TOKEN_PUNTO ||
                        tokens[i].type == TOKEN_DIV ||
                        tokens[i].type == TOKEN_MAYOR ||
                        tokens[i].type == TOKEN_MAYOR_IGUAL ||
                        tokens[i].type == TOKEN_MENOR ||
                        tokens[i].type == TOKEN_MENOR_IGUAL ||
                        tokens[i].type == TOKEN_FLECHA) {
                        needs_space = 0;
                    }
                    
                    if (needs_space) {
                        fprintf(beamer, " ");
                    }
                }
            }

            started_current_line = true;
            current_column += strlen(tokens[i].lexeme);
        }

        while (current_line <= end_line) {
            fprintf(beamer, "\\\\\n"); 
            current_line++;
        }

        fprintf(beamer, "\\end{flushleft}\n");
        fprintf(beamer, "\\end{frame}\n\n");
    }

    // Liberar memoria
    for (int i = 0; i < token_count; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
    
}

void generar_histograma(FILE* beamer, const char* token_stats) {
    // Leer estadísticas reales
    int palabras_clave = 0, identificadores = 0, constantes = 0, operadores = 0, puntuacion = 0, total = 0;
    
    FILE* stats_file = fopen(token_stats, "r");
    if (stats_file) {
        char category[50];
        int count;
        while (fscanf(stats_file, "%s %d", category, &count) == 2) {
            if (strcmp(category, "PalabrasClave") == 0) palabras_clave = count;
            else if (strcmp(category, "Identificadores") == 0) identificadores = count;
            else if (strcmp(category, "Constantes") == 0) constantes = count;
            else if (strcmp(category, "Operadores") == 0) operadores = count;
            else if (strcmp(category, "Puntuacion") == 0) puntuacion = count;
            else if (strcmp(category, "Total") == 0) total = count;
        }
        fclose(stats_file);
    }
    
    // Calcular altura máxima automáticamente
    int max_val = palabras_clave;
    if (identificadores > max_val) max_val = identificadores;
    if (constantes > max_val) max_val = constantes;
    if (operadores > max_val) max_val = operadores;
    if (puntuacion > max_val) max_val = puntuacion;
    int ymax = (int)(max_val * 1.2);
    
fprintf(beamer, "\\begin{frame}{Distribución de Tokens - Histograma}\n");
    fprintf(beamer, "    \\vspace{-0.3cm}\n");
    fprintf(beamer, "    \\begin{center}\n");
    fprintf(beamer, "        \\resizebox{0.9\\textwidth}{!}{\n");
    fprintf(beamer, "        \\begin{tikzpicture}\n");
    fprintf(beamer, "            \\begin{axis}[\n");
    fprintf(beamer, "                ybar,\n");
    fprintf(beamer, "                bar width=18pt,\n");
    fprintf(beamer, "                width=0.9\\textwidth,\n");
    fprintf(beamer, "                height=0.6\\textheight,\n");
    fprintf(beamer, "                enlargelimits=0.15,\n");
    fprintf(beamer, "                ylabel={Cantidad de Tokens},\n");
    fprintf(beamer, "                ylabel style={font=\\small},\n");
    fprintf(beamer, "                symbolic x coords={{Palabras\\\\Reservadas}, Identificadores, Constantes, Operadores, Puntuación},\n"); 
    fprintf(beamer, "                xtick=data,\n");
    fprintf(beamer, "                xticklabel style={font=\\small,align=center,text depth=0pt},\n"); // ALINEACIÓN CENTRADA
    fprintf(beamer, "                nodes near coords,\n");
    fprintf(beamer, "                nodes near coords style={font=\\footnotesize, yshift=5pt},\n");
    fprintf(beamer, "                ymajorgrids=true,\n");
    fprintf(beamer, "                grid style={dashed,gray!30},\n");
    fprintf(beamer, "                ymin=0,\n");
    fprintf(beamer, "                ymax=%d,\n", ymax);
    fprintf(beamer, "                axis lines*=left\n");
    fprintf(beamer, "            ]\n");
    fprintf(beamer, "            \\addplot[fill=blue!35, draw=blue!70!black, line width=0.7pt] coordinates {\n");
    fprintf(beamer, "                ({Palabras\\\\Reservadas}, %d)\n", palabras_clave); // NOTA: también aquí van llaves
    fprintf(beamer, "                (Identificadores, %d)\n", identificadores);
    fprintf(beamer, "                (Constantes, %d)\n", constantes);
    fprintf(beamer, "                (Operadores, %d)\n", operadores);
    fprintf(beamer, "                (Puntuación, %d)\n", puntuacion);
    fprintf(beamer, "            };\n");
    fprintf(beamer, "            \\end{axis}\n");
    fprintf(beamer, "        \\end{tikzpicture}\n");
    fprintf(beamer, "        }\n");
    fprintf(beamer, "    \\end{center}\n");
    fprintf(beamer, "    \\vspace{0.1cm}\n");
    fprintf(beamer, "    \\begin{itemize}\n");
    fprintf(beamer, "        \\item \\textbf{Tokens totales:} %d\n", total);
    fprintf(beamer, "    \\end{itemize}\n");
    fprintf(beamer, "\\end{frame}\n\n");
}

void generar_beamer(const char* preprocessed_file, const char* token_stats) {
    
    FILE* beamer = fopen("PresentacionBeamer.tex", "w");
    if (!beamer) {
        fprintf(stderr, "❌ Error: No se puede crear archivo Beamer\n");
        return;
    }
    

    fprintf(beamer, "\\documentclass{beamer}\n");
    fprintf(beamer, "\\usepackage[utf8]{inputenc}\n");
    fprintf(beamer, "\\usepackage[english]{babel}\n");
    fprintf(beamer, "\\usepackage{graphicx}\n");
    fprintf(beamer, "\\usepackage{pgfplots}\n");
    fprintf(beamer, "\\usepackage{xcolor}\n");
    fprintf(beamer, "\\usepackage{tikz}\n");
    fprintf(beamer, "\\usepackage{fancyvrb}\n");
    fprintf(beamer, "\\usepackage{listings}\n");
    fprintf(beamer, "\\usepackage{textcomp} %% Para caracteres especiales\n");
    fprintf(beamer, "\\usetikzlibrary{arrows, positioning}\n");
    fprintf(beamer, "\\usetheme{Madrid}\n");
    fprintf(beamer, "\\usecolortheme{whale}\n");
    
    fprintf(beamer, "\\usepackage{amsmath,amssymb}\n"); 
    fprintf(beamer, "\\pgfplotsset{compat=1.18}\n"); 
    
    fprintf(beamer, "\\usepackage{hyperref}\n");
    fprintf(beamer, "\\hypersetup{pdfpagemode=FullScreen} %% Abrir en modo presentación\n");
    
    fprintf(beamer, "\\setbeamertemplate{navigation symbols}{}\n\n");
    
    fprintf(beamer, "\\DefineVerbatimEnvironment{code}{Verbatim}{fontsize=\\scriptsize, formatcom=\\color{black}}\n\n");
    

    fprintf(beamer, "\\sloppy\n");
    fprintf(beamer, "\\tolerance=1000\n");
    fprintf(beamer, "\\emergencystretch=1.5em\n\n");
    
    fprintf(beamer, "\\title{Analizador Léxico}\n");
    fprintf(beamer, "\\date{Semestre II - 2025}\n");
    fprintf(beamer, "\\institute{TEC}\n\n");
    
    fprintf(beamer, "\\begin{document}\n\n");
    
    generar_portada(beamer);
    generar_flex(beamer);
    generar_codigo(beamer, preprocessed_file);
    generar_histograma(beamer, token_stats);
    
    fprintf(beamer, "\\end{document}\n");
    fclose(beamer);
    
    system("pdflatex -interaction=nonstopmode PresentacionBeamer.tex > /dev/null 2>&1");
    system("evince PresentacionBeamer.pdf");
    
  
}