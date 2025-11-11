#include "header.h"
#include <stdio.h>
#define PI 3.14159
#define MAX 100

// Este es un comentario
int main() {
    float radio = 5.0;
    float area = PI * radio * radio;
    
    if (area > MAX) {
        printf("Area muy grande: %f\n", area);
    }
    
    return 0;
}