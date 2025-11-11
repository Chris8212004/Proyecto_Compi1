#include "header.h"
#define WIDTH 10
#define HEIGHT 5
#define AREA WIDTH * HEIGHT
#define MESSAGE "Área calculada"

// Comentario de una línea
/* Comentario
   de múltiples líneas */
int main() {
    int resultado = AREA;
    printf("%s: %d\n", MESSAGE, resultado);
    return resultado;
}