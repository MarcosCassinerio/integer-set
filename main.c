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

    ITree arbol = itree_crear();
    arbol = itree_insertar(arbol, interval1);
    arbol = itree_insertar(arbol, interval2);
    arbol = itree_insertar(arbol, interval3);
    arbol = itree_insertar(arbol, interval4);
    arbol = itree_insertar(arbol, interval5);

    itree_recorrer_dfs(arbol, interval_imprimir);

    return 0;
}