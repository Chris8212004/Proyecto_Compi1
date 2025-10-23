// test.c
#include "header.h"
#define PI 3.14159
#define MAX 100

int main() {
    float radius = 5.0;
    float area = PI * radius * radius;
    
    if (area > MAX) {
        printf("Area muy grande: %f\n", area);
    }
    
    return 0;
}