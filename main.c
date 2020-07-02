#include "./ITree/itree.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Interval * prueba1 = interval_crear(20, 20);
    Interval * prueba2 = interval_crear(10, 10);
    Interval * prueba3 = interval_crear(30, 30);
    Interval * prueba4 = interval_crear(40, 40);
    Interval * prueba5 = interval_crear(21, 29);

    ITree arbol1 = itree_crear();

    arbol1 = itree_insertar(arbol1, prueba1);
    arbol1 = itree_insertar(arbol1, prueba2);
    arbol1 = itree_insertar(arbol1, prueba3);
    arbol1 = itree_insertar(arbol1, prueba4);

    itree_recorrer_dfs(arbol1, interval_imprimir);
    printf("\n");
    itree_recorrer_bfs(arbol1, interval_imprimir);
    printf("\n");
    
    arbol1 = itree_insertar(arbol1, prueba5);

    itree_recorrer_dfs(arbol1, interval_imprimir);
    printf("\n");
    itree_recorrer_bfs(arbol1, interval_imprimir);
    printf("\n");
    return 0;
}