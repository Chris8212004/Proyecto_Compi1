// test2.c - Solo macros simples SIN parámetros
#define WIDTH 10
#define HEIGHT 5  
#define AREA WIDTH * HEIGHT
#define PROJECT_NAME "Mi Proyecto"
#define VERSION 1.0

int main() {
    int area_calculada = AREA;
    char* nombre = PROJECT_NAME;
    float version = VERSION;
    
    printf("Proyecto: %s\n", nombre);
    printf("Area: %d\n", area_calculada);
    printf("Version: %.1f\n", version);
    
    return area_calculada;
}