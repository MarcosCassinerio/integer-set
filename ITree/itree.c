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
INodo *itree_nuevo_nodo(Interval * interval) {
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
//            6
//       0          
//    -2  2

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

/*
    itree_obtener_menor: ITree -> INodo*
    Dado un aborl, retorna el menor nodo del arbol
*/
INodo *itree_obtener_menor(ITree arbol) {
  if (!arbol || !arbol->izq)
    return arbol;
  return itree_obtener_menor(arbol->izq);
}

ITree itree_crear() {
  return NULL;                  // Retorna un ITree vacio
}

void itree_destruir(ITree arbol, FuncionIntervalVoid funcion) {
  if (arbol) {                  // Si existe el arbol
    // Ejecutamos itree_destruir en los 2 hijos del nodo
    itree_destruir(arbol->izq, funcion);
    itree_destruir(arbol->der, funcion);
    // Liberamos la memoria de su interval utilizando la funcion "funcion"
    funcion(arbol->interval);
    free(arbol);                // Liberamos la memoria del nodo
  }
}

ITree itree_insertar(ITree arbol, Interval * interval) {
  printf("Inserta intervalo ");
  interval_imprimir(interval);
  printf(" en arbol ");
  itree_recorrer_dfs(arbol, interval_imprimir);
  if (arbol) {
    printf(" con raiz ");
    interval_imprimir(arbol->interval);
  }
  printf("\n");
  Interval * intervalConcat = NULL;
  if (!interval_valido(interval))
    return arbol;
  if (arbol) {
    intervalConcat = interval_concat(arbol->interval, interval);
    if (intervalConcat) {
      interval = intervalConcat;
      if (!arbol-> izq && !arbol->der) {
        interval_destruir(arbol->interval);
        arbol->interval = interval;
        return arbol;
      }
      arbol = itree_eliminar(arbol, arbol->interval);
      return itree_insertar(arbol, interval);
    }
    if (interval_extremo_izq(interval) < interval_extremo_izq(arbol->interval))
      arbol->izq = itree_insertar(arbol->izq, interval);
    else
      arbol->der = itree_insertar(arbol->der, interval);
    arbol->altura = itree_calcular_altura(arbol);
    return itree_balancear(arbol);
  }
  return itree_nuevo_nodo(interval);
}

ITree itree_eliminar(ITree arbol, Interval * interval) {
  INodo * aux = itree_crear();
  if (!interval_valido(interval))
    return arbol;
  if (!arbol) {
    printf("  Interval no encontrado\n");
    return arbol;
  }
  if (interval_extremo_izq(interval) == interval_extremo_izq(arbol->interval) && interval_extremo_der(interval) == interval_extremo_der(arbol->interval)) {
    if (!arbol->izq || !arbol->der) {
      interval_destruir(arbol->interval);
      aux = arbol->izq ? arbol->izq : arbol->der;
      if (aux) {
        arbol->interval = interval_crear(interval_extremo_izq(aux->interval), interval_extremo_der(aux->interval));
        arbol->der = aux->der;
        arbol->izq = aux->izq;
        interval_destruir(aux->interval);
        free(aux);
      } else {
        free(arbol);
        arbol = NULL;
      }
    } else {
      interval_destruir(arbol->interval);
      aux = itree_obtener_menor(arbol->der);
      arbol->interval = interval_crear(interval_extremo_izq(aux->interval), interval_extremo_der(aux->interval));
      arbol->der = itree_eliminar(arbol->der, aux->interval);
    }
  } else if (interval_extremo_izq(interval) > interval_extremo_izq(arbol->interval))
    arbol->der = itree_eliminar(arbol->der, interval);
  else {
    arbol->izq = itree_eliminar(arbol->izq, interval);
  }
  if (arbol)
    arbol->altura = itree_calcular_altura(arbol);
  return itree_balancear(arbol);
}

void itree_recorrer_dfs(ITree arbol, FuncionIntervalVoid funcion) {
  if (arbol) {                  // Si el arbol no esta vacio
    // Aplica la funcion itree_recorrer_dfs en el hijo izquierdo
    itree_recorrer_dfs(arbol->izq, funcion);
    // Aplica la funcion en el interval de la raiz
    funcion(arbol->interval);
    // Aplica la funcion itree_recorrer_dfs en el hijo derecho
    itree_recorrer_dfs(arbol->der, funcion);
  }
}

void itree_recorrer_bfs(ITree arbol, FuncionIntervalVoid funcion) {
  if (arbol) {                  // Si el arbol no esta vacio
    Queue queue = queue_crear();        // Crea una Queue vacia
    ITree aux;
    queue = queue_push(queue, arbol);   // Metemos la raiz en la cola
    while (queue) {             // Mientras la cola no este vacia
      aux = (ITree) queue_pop(&queue);  // Sacamos el primer nodo de la cola
      funcion(aux->interval);  // Aplica la funcion en el intervalo de aux
      // Si aux tiene hijos, los mete en la cola
      queue = aux->izq ? queue_push(queue, aux->izq) : queue;
      queue = aux->der ? queue_push(queue, aux->der) : queue;
    }
  }
}

void itree_obtener_queue(ITree arbol, Queue queue) {
  if (arbol) {
    itree_obtener_queue(arbol->izq, queue);

    interval_imprimir(arbol->interval);
    queue = queue_push(queue, interval_crear(interval_extremo_izq(arbol->interval), interval_extremo_der(arbol->interval)));

    itree_obtener_queue(arbol->der, queue);
  }
}

void itree_copiar(ITree arbol, ITree copia) {
  if (arbol) {
    itree_copiar(arbol->izq, copia);

    copia = itree_insertar(copia, arbol->interval);

    itree_copiar(arbol->der, copia);
  }
}

ITree itree_unir(ITree itree1, ITree itree2) {
  printf("unir\n");
  printf("itree1: ");
  itree_recorrer_dfs(itree1, interval_imprimir);
  printf("\n");
  printf("itree2: ");
  itree_recorrer_dfs(itree2, interval_imprimir);
  printf("\n");
  ITree arbolUnion = itree_crear();
  Queue queue = queue_crear();
  Interval * aux = NULL;
  itree_obtener_queue(itree1, queue);
  while (queue)
    arbolUnion = itree_insertar(arbolUnion, (Interval *) queue_pop(&queue));
  itree_recorrer_dfs(arbolUnion, interval_imprimir);
  itree_obtener_queue(itree2, queue);
  while (queue)
    arbolUnion = itree_insertar(arbolUnion, (Interval *) queue_pop(&queue));
  itree_recorrer_dfs(arbolUnion, interval_imprimir);
  return arbolUnion;
}

ITree itree_intersecar(ITree itree1, ITree itree2) {
  ITree arbolInterserccion = itree_crear();
  Queue queue1 = queue_crear();
  Queue queue2 = queue_crear();
  Interval * interval1 = NULL;
  Interval * interval2 = NULL;
  itree_obtener_queue(itree1, queue1);
  itree_obtener_queue(itree2, queue2);
  while (queue1) {
    interval1 = queue_pop(&queue1);
    while (queue2) {
      interval2 = queue_pop(&queue2);
      arbolInterserccion = itree_insertar(arbolInterserccion, interval_interseccion(interval1, interval2));
    }
  }
  return arbolInterserccion;
}
