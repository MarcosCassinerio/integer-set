#include "./ITree/itree.h"
#include "./Interval/interval.h"
//#include "./TablaHash/tablahash.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_LINEA 100

unsigned hash(void* clave) {
    int* p = clave;
    return *p;
}

/*
    leer_cadena: char* -> char*
    Toma lo escrito por el teclado, y lo retorna reemplazando el salto de linea
    por un '\0'.
*/
char *leer_cadena(char *string) {
    char c, *aux = string;
    // Mientras que el caracter leido sea distinto a '\n'
    while ((c = getchar()) != '\n') {
        if (c != '\r') {            // Si el caracter leido es distinto a '\r'
            *string = c;              // Almacenamos el caracter en string
            ++string;                 // Movemos la posicion a la que apunta string
        }
    }
    *string = '\0';               // Colocamos un terminador al final
    return aux;                   // Devolvemos un puntero al comienzo de string
}

int main() {
    /*
    char buffer[MAX_LINEA];
    TablaHash *th = tablahash_crear(10, hash);
    buffer[0] = '\0';
    while (strcmp(leer_cadena(buffer), "salir\0")) {
    }
    */
    ITree arbol = itree_crear();
    ITree arbol2 = itree_crear();
    Interval *interval1 = interval_crear(20, 20);
    Interval *interval2 = interval_crear(10, 10);
    Interval *interval3 = interval_crear(30, 30);
    Interval *interval4 = interval_crear(40, 40);
    arbol = itree_insertar(arbol, interval1);
    arbol = itree_insertar(arbol, interval2);
    arbol = itree_insertar(arbol, interval3);
    arbol = itree_insertar(arbol, interval4);
    printf("imprimo arbol\n");
    itree_imprimir(arbol);
    printf("\n");
    arbol2 = itree_complemento(arbol);
    printf("imprimo arbol2\n");
    itree_imprimir(arbol2);
    printf("\n");
    return 0;
}