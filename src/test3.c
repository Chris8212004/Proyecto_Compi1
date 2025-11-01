// test3_intermedio.c - Test intermedio
#include "header.h"

#define PI 3.14159
#define MAX_ARRAY 10
#define CUADRADO(x) ((x) * (x))

int suma_arreglo(int arr[], int size);
void imprimir_arreglo(int arr[], int size);

int main() {
    int numeros[MAX_ARRAY] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    printf("Arreglo original:\n");
    imprimir_arreglo(numeros, MAX_ARRAY);
    
    int total = suma_arreglo(numeros, MAX_ARRAY);
    printf("Suma total: %d\n", total);
    
    printf("Cuadrados:\n");
    for (int i = 0; i < MAX_ARRAY; i++) {
        printf("%d^2 = %d\n", numeros[i], CUADRADO(numeros[i]));
    }
    
    // Estructuras de control
    int opcion = 2;
    switch (opcion) {
        case 1:
            printf("Opcion 1 seleccionada\n");
            break;
        case 2:
            printf("Opcion 2 seleccionada\n");
            break;
        default:
            printf("Opcion no valida\n");
    }
    
    return 0;
}

int suma_arreglo(int arr[], int size) {
    int suma = 0;
    for (int i = 0; i < size; i++) {
        suma += arr[i];
    }
    return suma;
}

void imprimir_arreglo(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}