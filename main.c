#include "./ITree/itree.h"
#include "./Interval/interval.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Interval * aux = NULL;
    ITree arbol1 = itree_crear();
    ITree arbol2 = itree_crear();
    ITree itreeUnion = itree_crear();
    ITree itreeInterseccion = itree_crear();
    Interval * prueba1 = interval_crear(20, 20);
    Interval * prueba2 = interval_crear(10, 10);
    Interval * prueba3 = interval_crear(30, 30);
    Interval * prueba4 = interval_crear(40, 40);
    Interval * prueba5 = interval_crear(21, 29);

    arbol1 = itree_insertar(arbol1, prueba1);
    arbol1 = itree_insertar(arbol1, prueba2);
    arbol1 = itree_insertar(arbol1, prueba3);
    arbol1 = itree_insertar(arbol1, prueba4);
    printf("arbol1: ");
    itree_imprimir(arbol1);
    printf("\n");
    
    arbol2 = itree_insertar(arbol2, prueba5);
    printf("arbol2: ");
    itree_imprimir(arbol2);
    printf("\n");

    itree_unir(&itreeUnion, &arbol1);
    itree_unir(&itreeUnion, &arbol2);
    printf("union 1 y 2: ");
    itree_imprimir(itreeUnion);
    printf("\n");

    itree_intersecar(&itreeInterseccion, &arbol1, &itreeUnion);
    printf("interseccion 1 y union: ");
    itree_imprimir(itreeInterseccion);
    printf("\n");
    return 0;
}