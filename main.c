#include "./ITree/itree.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Interval * interval1 = interval_crear(4, 4);
    Interval * interval2 = interval_crear(0, 0);
    Interval * interval3 = interval_crear(6, 6);
    Interval * interval4 = interval_crear(2, 2);
    Interval * interval5 = interval_crear(-2, -2);
    Interval * interval6 = interval_crear(-2, 5);

    ITree arbol1 = itree_crear();
    ITree arbol2 = itree_crear();
    ITree unir = itree_crear();
    ITree intersecar = itree_crear();
    arbol1 = itree_insertar(arbol1, interval1);
    arbol1 = itree_insertar(arbol1, interval2);
    arbol1 = itree_insertar(arbol1, interval3);
    arbol1 = itree_insertar(arbol1, interval4);
    arbol1 = itree_insertar(arbol1, interval5);
    arbol2 = itree_insertar(arbol2, interval6);

    itree_recorrer_dfs(arbol1, interval_imprimir);
    printf("\n");
    itree_recorrer_dfs(arbol2, interval_imprimir);
    printf("\n");
    unir = itree_unir(arbol1, arbol2);
    itree_recorrer_dfs(unir, interval_imprimir);
    printf("\n");
    //intersecar = itree_intersecar(arbol1, arbol2);
    //itree_recorrer_dfs(itree_intersecar(arbol1, arbol2), interval_imprimir);

    return 0;
}