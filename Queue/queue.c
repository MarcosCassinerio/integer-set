#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct _QNode {
  void *dato;
  struct _QNode *ant;
  struct _QNode *sig;
};

Queue queue_crear() {
  return NULL;                  // Retorna una cola vacia
}

void *queue_pop(Queue * queue) {
  void *dato = NULL;
  if (queue && *queue) {        // Si existe la cola y no esta vacia
    dato = (*queue)->dato;      // Almacena el dato
    if ((*queue)->sig == *queue) {      // Si la cola tiene un solo elemento
      free(*queue);             // Borra el nodo de la queue
      *queue = NULL;            // Lo devuelve al estado de una cola vacia
    } else {                    // Si tiene mas de un elemnto
      QNode *aux = *queue;      // Crea un puntero al nodo auxiliar
      // Elimina la referencia de este nodo en la cola
      aux->ant->sig = aux->sig;
      aux->sig->ant = aux->ant;
      *queue = aux->sig;
      free(aux);                // Libera la memoria del nodo
    }
  }
  return dato;                  // Retorna el dato almacenado en el nodo borrado
}

Queue queue_push(Queue queue, void *dato) {
  QNode *nodo = malloc(sizeof(QNode));  // Pide la memoria de un nodo
  nodo->dato = dato;            //Almacena el dato en el nodo
  if (!queue) {                 // Si la cola esta vacia
    // Hacemos que este nodo en siguiente y en anterior apunte a si mismo
    nodo->ant = nodo;
    nodo->sig = nodo;
    return nodo;
  }
  // Si la cola no es vacia
  // Hacemos que el ultimo nodo de la cola sea el nodo previamente creado
  queue->ant->sig = nodo;
  nodo->ant = queue->ant;
  queue->ant = nodo;
  nodo->sig = queue;
  return queue;
}

void queue_destruir(Queue queue, FuncionDestruir funcion) {
  while (queue) {               // Mientras haya elementos en la cola
    funcion(queue_pop(&queue)); // Eliminamos y liberamos la memoria los nodos y sus datos
  }
}
