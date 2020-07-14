#ifndef __ITREE_H__
#define __ITREE_H__

#include "../Interval/interval.h"
#include <limits.h>

typedef struct _INodo INodo;

typedef INodo *ITree;

typedef void (*FuncionIntervalVoid) (Interval * interval);

/*
    itree_crear: -> ITree
    Retorna un ITree vacio.
*/
ITree itree_crear();

/*
    itree_destruir: ITree FuncionIntervalVoid
    Dado un arbol y una funcion para destruir intervals, borra el arbol
    liberando toda la memoria.
*/
void itree_destruir(ITree *arbol);

/*
    itree_insertar: ITree Interval* -> ITree
    Dado un ITree y un puntero a un interval, lo inserta en al arbol, en
    caso que sea valido y que no se encuentre ya en el arbol.
    Retorna la raiz del arbol.
*/
ITree itree_insertar(ITree arbol, Interval * interval);

void itree_unir(ITree *itree1, ITree itree2);

void itree_intersecar(ITree *interseccion, ITree itree1, ITree itree2);

ITree itree_restar(ITree arbol1, ITree arbol2);

ITree itree_complemento(ITree arbol);

/*
    itree_imprimir: ITree
    Recorre el arbol con un algoritmo DFS de forma inorder imprimiendo sus intervalos.
*/
void itree_imprimir(ITree arbol);

#endif                          /* __ITREE_H__ */