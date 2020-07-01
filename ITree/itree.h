#ifndef __ITREE_H__
#define __ITREE_H__

#include "../Interval/interval.h"
#include <stddef.h>
#include <stdio.h>
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

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
void itree_destruir(ITree arbol, FuncionIntervalVoid funcion);

/*
    itree_altura: ITree -> int
    Dado un arbol, devuelve su altura.
*/
int itree_altura(ITree arbol);

/*
    itree_insertar: ITree Interval* -> ITree
    Dado un ITree y un puntero a un interval, lo inserta en al arbol, en
    caso que sea valido y que no se encuentre ya en el arbol.
    Retorna la raiz del arbol.
*/
ITree itree_insertar(ITree arbol, Interval * interval);

/*
    itree_eliminar: ITree Interval* -> ITree
    Dado un ITree y un puntero a un interval, si el mismo se encuentra en
    el arbol, lo elimina del mismo liberando su memoria.
    Retorna la raiz del arbol.
*/
ITree itree_eliminar(ITree arbol, Interval * interval);

/*
    itree_recorrer_dfs: ITree FuncionIntervaloVoid
    Recorre el arbol con un algoritmo DFS de forma inorder, aplicando la 
    funcion dada como parametro a los intervalos almacenados en los nodos.
*/
void itree_recorrer_dfs(ITree arbol, FuncionIntervalVoid funcion);

#endif                          /* __ITREE_H__ */