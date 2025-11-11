#include "header.h" 
#include <stdlib.h>
#define BUFFER_SIZE 256
#define INIT_VALUE 42

int main() {
    char* buffer = malloc(BUFFER_SIZE);
    int valor = INIT_VALUE * 2;
    
    printf("Buffer: %p, Valor: %d\n", buffer, valor);
    free(buffer);
    return 0;
}