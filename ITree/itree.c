#include "itree.h"
#include <stdio.h>
#include <stdlib.h>

struct _INodo {
  Interval *interval;
  int altura;
  struct _INodo *izq;
  struct _INodo *der;
};
/*
    itree_nuevo_nodo: Interval* -> INodo
    Dado un intervalo, devuelve un nuevo nodo con el intervalo en el.
*/
INodo *itree_nuevo_nodo(Interval *interval) {
  // Pedimos la memoria para un nodo
  INodo *nodo = (INodo *) malloc(sizeof(INodo));
  nodo->interval = interval;  // Almacenamos el interval
  // Al crear un nodo su altura es 0 y no tiene hijos
  nodo->altura = 0;
  nodo->izq = NULL;
  nodo->der = NULL;
  return nodo;
}

/*
    itree_altura: ITree -> int
    Dado un arbol, devuelve su altura.
*/
int itree_altura(ITree arbol) {
  if (!arbol)                   // Si el arbol esta vacio
    return -1;                  // Retornamos -1 para facilitar calculos
  return arbol->altura;         // Retornamos su altura
}

/*
    itree_calcular_altura: ITree -> int
    Dado un ITree, calcula su altura de la raiz, sumandole 1 a la altura del
    hijo con mayor altura.
*/
int itree_calcular_altura(ITree arbol) {
  return (1 + MAX(itree_altura(arbol->izq), itree_altura(arbol->der)));
}

/*
    rotacion_izquierda: ITree -> ITree
    Dado un ITree, lo modificamos de manera tal que su hijo derecho sea la 
    raiz, la raiz original sea el hijo izquierdo de la nueva raiz y que el 
    anterior hijo izquierdo de la nueva raiz sea el hijo derecho de la vieja.
    Retornamos la nueva raiz.
*/
ITree rotacion_izquierda(ITree arbol) {
  if (!arbol)                   // Si el arbol es vacio
    return arbol;
  ITree aux = arbol->der;       // Creamos la nueva raiz
  // Cambiamos las referencias del arbol
  arbol->der = arbol->der->izq;
  aux->izq = arbol;
  // Actualizamos sus alturas
  arbol->altura = itree_calcular_altura(arbol);
  aux->altura = itree_calcular_altura(aux);
  return aux;
}

/*
    rotacion_izquierda: ITree -> ITree
    Dado un ITree, lo modificamos de manera tal que su hijo izquierdo sea la 
    raiz, la raiz original sea el hijo derecho de la nueva raiz y que el 
    anterior hijo derecho de la nueva raiz sea el hijo izquierdo de la vieja.
    Retornamos la nueva raiz.
*/
ITree rotacion_derecha(ITree arbol) {
  if (!arbol)                   // Si el arbol es vacio
    return arbol;
  ITree aux = arbol->izq;       // Creamos la nueva raiz
  // Cambiamos las referencias del arbol
  arbol->izq = arbol->izq->der;
  aux->der = arbol;
  // Actualizamos sus alturas
  arbol->altura = itree_calcular_altura(arbol);
  aux->altura = itree_calcular_altura(aux);
  return aux;
}

/*
    itree_balance: ITree -> int
    Dado un ITree devuelve su balance
*/
int itree_balance(ITree arbol) {
  if (!arbol)                   // Si el arbol esta vacio
    return 0;                   // Su balance es 0
  // Retornamos la altura del subarbol izquierdo menos 
  // la altura del subarbol derecho
  return itree_altura(arbol->izq) - itree_altura(arbol->der);
}

/*
    itree_balancear: ITree -> ITree
    Dado un arbol, en caso de estar desbalanceado, lo balancea realizando las 
    rotaciones necesarias
*/
ITree itree_balancear(ITree arbol) {
  int balance = itree_balance(arbol);   // Calcula el balance del arbol
  if (balance > 1) {            // Si el balance es mayor a uno
    // Si el balance de su hijo izquierdo es menor a 0
    if (itree_balance(arbol->izq) < 0)
      // Hace una rotacion izquieda en el hijo izquierdo
      arbol->izq = rotacion_izquierda(arbol->izq);
    // Hace una rotacion derecha y retorna la nueva raiz
    return rotacion_derecha(arbol);
  }
  if (balance < -1) {           // Si el balance es menor a uno
    // Si el balance de su hijo derecho es mayor a 0
    if (itree_balance(arbol->der) > 0)
      // Hace una rotacion derecha en su hijo derecho
      arbol->der = rotacion_derecha(arbol->der);
    // Hace una rotacion izquierda y retorna la nueva raiz
    return rotacion_izquierda(arbol);
  }
  return arbol;                 // En caso de estar balanceado no cambia nada
}

void itree_interseccion_intervalo(ITree *itree1, ITree itree2, Interval *interval) {
  Interval *aux = NULL;
  if (itree2) {
    itree_interseccion_intervalo(itree1, itree2->izq, interval);
    aux = interval_interseccion(itree2->interval, interval);
    if (aux)
      *itree1 = itree_insertar(*itree1, aux);
    itree_interseccion_intervalo(itree1, itree2->der, interval);
  }
}

Interval *itree_complemento_aux(ITree *complemento, ITree arbol, Interval *interval) {
  Interval *aux = NULL;
  if (arbol) {
    interval = itree_complemento_aux(complemento, arbol->izq, interval);
    aux = interval_crear(interval_extremo_der(interval), interval_extremo_izq(arbol->interval) - 1);
    if (interval_valido(aux))
      *complemento = itree_insertar(*complemento, interval_crear(interval_extremo_izq(aux), interval_extremo_der(aux)));
    interval_destruir(aux);
    interval_destruir(interval);
    interval = interval_crear(interval_extremo_der(arbol->interval) + 1, interval_extremo_der(arbol->interval) + 1);
    interval = itree_complemento_aux(complemento, arbol->der, interval);
  }
  if (!interval)
    interval = interval_crear(INT_MIN, INT_MIN);
  return interval;
}

Interval *itree_recorrer_dfs(ITree arbol, Interval *interval) {
  Interval *aux = NULL;
  if (arbol) {
    interval = itree_recorrer_dfs(arbol->izq, interval);
    if (!interval) {
      interval = interval_crear(interval_extremo_izq(arbol->interval), interval_extremo_der(arbol->interval));
    } else {
      aux = interval_concat(interval, arbol->interval);
      if (aux)
        interval = aux;
      else {
        interval_imprimir(interval);
        interval_destruir(interval);
        interval = interval_crear(interval_extremo_izq(arbol->interval), interval_extremo_der(arbol->interval));
      }
    }
    interval = itree_recorrer_dfs(arbol->der, interval);
  }
  return interval;
}

ITree itree_crear() {
  return NULL;
}

void itree_destruir(ITree arbol) {
  if (arbol) {
    itree_destruir(arbol->izq);
    itree_destruir(arbol->der);
    interval_destruir(arbol->interval);
    free(arbol);
  }
}

ITree itree_insertar(ITree arbol, Interval *interval) {
  Interval *aux = NULL;
  if (!interval_valido(interval))
    return arbol;
  if (arbol) {
    if (interval_extremo_izq(interval) == interval_extremo_izq(arbol->interval)){
      aux = interval_concat(interval, arbol->interval);
      interval_destruir(arbol->interval);
      interval_destruir(interval);
      arbol->interval = aux;
    } else if (interval_extremo_izq(interval) < interval_extremo_izq(arbol->interval))
      arbol->izq = itree_insertar(arbol->izq, interval);
    else
      arbol->der = itree_insertar(arbol->der, interval);
    arbol->altura = itree_calcular_altura(arbol);
    return itree_balancear(arbol);
  }
  return itree_nuevo_nodo(interval);
}

void itree_unir(ITree *itree1, ITree itree2) {
  if (itree2) {
    itree_unir(itree1, itree2->izq);
    *itree1 = itree_insertar(*itree1, interval_crear(interval_extremo_izq(itree2->interval), interval_extremo_der(itree2->interval)));
    itree_unir(itree1, itree2->der);
  }
}

void itree_intersecar(ITree *interseccion, ITree itree1, ITree itree2) {
  ITree aux = NULL;
  if (itree1) {
    itree_intersecar(interseccion, itree1->izq, itree2);
    itree_interseccion_intervalo(&aux, itree2, itree1->interval);
    itree_unir(interseccion, aux);
    itree_intersecar(interseccion, itree1->der, itree2);
  }
}

ITree itree_complemento(ITree arbol) {
  Interval *aux = NULL;
  ITree complemento = itree_crear();
  aux = itree_complemento_aux(&complemento, arbol, aux);
  if (interval_extremo_der(aux) != INT_MAX)
    complemento = itree_insertar(complemento, interval_crear(interval_extremo_der(aux), INT_MAX));
  interval_destruir(aux);
  return complemento;
}

void itree_imprimir(ITree arbol) {
  Interval *aux = NULL;
  aux = itree_recorrer_dfs(arbol, aux);
  interval_imprimir(aux);
}
