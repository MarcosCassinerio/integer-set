#include "tablahash.h"

struct _Contenedor {
  void *dato;
};

struct _CasillaHash {
  char *clave;
  void *dato;
};

struct _LinkedList {
  CasillaHash casilla;
  struct _LinkedList *ant;
  struct _LinkedList *sig;
};

struct _TablaHash {
  LinkedList **tabla;
  unsigned numElems;
  unsigned capacidad;
  FuncionHash hash;
};

/*
  contenedor_crear: void* -> Contenedor*
  Retorna un contenedor con el dato proporcionado en el.
*/
Contenedor *contenedor_crear(void *dato) {
  Contenedor *contenedor = malloc(sizeof(Contenedor)); // Pedimos memoria
  contenedor->dato = dato; // Le asignamos el dato dado
  return contenedor; // Retornamos el contenedor
}

/*
  linked_list_crear: CasillaHash -> LinkedList*
  Retorna una linkedList que apunta a si misma con la casilla proporcionada en el.
*/
LinkedList *linked_list_crear(CasillaHash casilla) {
  LinkedList *lista = malloc(sizeof(LinkedList)); // Pide memoria para la lista
  lista->casilla = casilla; // Le asigna la casilla a la lista
  // Hace que la lista apunte a si misma
  lista->ant = lista;
  lista->sig = lista;
  return lista; // Retorna la misma lista
}

/*
  linked_list_insertar: LinkedList** CasillaHash
  Inserta la casilla proporcionada en la lista si es que no hay una con su clave,
  en caso contrario cambia su dato
*/
void linked_list_insertar(LinkedList **lista, CasillaHash casilla, FuncionVisitante funcion) {
  // Inicializa aux en la posicion actual de lista
  LinkedList *nodo = NULL, *aux = *lista;
  int insertado = 0;

  if (!lista || !(*lista)) // Si lista no es nulo
    // Inicializa lista con la casilla
    *lista = linked_list_crear(casilla);
  else {
    for(; aux != (*lista)->ant && !insertado; aux = aux->sig) {
      // Si la clave de la casilla actual coincide con la clave dada
      if (strcmp(aux->casilla.clave, casilla.clave) == 0) {
        // Reemplaza el dato de la casilla actual
        funcion(aux->casilla.dato);
        aux->casilla.dato = casilla.dato;
        insertado = 1;
      }
    }
    if (!insertado) { // Si el dato no fue insertado
      // Si la clave de la casilla actual coincide con la clave dada
      if (strcmp(aux->casilla.clave, casilla.clave) == 0) {
        funcion(aux->casilla.dato);
        // Reemplaza el dato de la casilla actual
        aux->casilla.dato = casilla.dato;
      } else {
        // Inserta el nuevo nodo al final de la lista
        nodo = linked_list_crear(casilla);
        nodo->ant = aux;
        nodo->sig = *lista;
        aux->sig = nodo;
        (*lista)->ant = nodo;
      }
    }
  }
}

/*
  linked_lista_buscar: LinkedList* char* -> void*
  Retorna el dato de la lista que tiene la clave proporcionada si es que existe,
  en caso contrario retorna NULL
*/
void *linked_list_buscar(LinkedList *lista, char *clave) {
  LinkedList *salida = NULL, *aux = lista;

  if (!lista)
    return NULL;

  for (; !salida && aux != lista->ant; aux = aux->sig) {
    // Si la clave de aux coincide con la clave proporcionada
    if (strcmp(aux->casilla.clave, clave) == 0)
      salida = aux; // Guarda aux en salida
  }

  // Si salida no es null retorna su dato, en caso contrario chequea si
  // la clave proporcionada coincide con la clave del ultimo retorna su dato
  // sino retorna NULL
  return salida ? salida->casilla.dato : (strcmp(aux->casilla.clave, clave) == 0) ? aux->casilla.dato : NULL;
}

/*
  linked_list_eliminar: LinkedList**
  Destruye la linkedList junto con sus datos.
*/
void linked_list_eliminar(LinkedList **lista, FuncionVisitante funcion) {
  LinkedList *aux;
  while (*lista) {
    aux = NULL;
    if ((*lista) != (*lista)->sig) { // Si la lista no tiene un solo elemento
      // Guarda en aux la posicion siguiente de lista
      aux = (*lista)->sig;
      (*lista)->ant->sig = aux;
      aux->ant = (*lista)->ant;
    }

    // Elimina la posicion actual de la lista junto a su dato y clave
    free((*lista)->casilla.clave);
    funcion((*lista)->casilla.dato);
    free(*lista);
    *lista = NULL;

    if (aux) { // Si aux no es nulo
      // Guardamos aux en lista
      (*lista) = aux;
      aux = NULL;
    }
  }
}

void *contenedor_obtener_dato(Contenedor *contenedor) {
  // Si el contenedor no es nulo devuelve el dato, en caso contrario devuelve NULL
  return contenedor ? contenedor->dato : NULL;
}

TablaHash *tablahash_crear(FuncionHash hash, unsigned capacidad) {
  // Pedimos memoria para la estructura principal.
  TablaHash *tabla = malloc(sizeof(TablaHash));
  unsigned idx = 0;
  // Asignamos sus valores
  tabla->numElems = 0;
  tabla->hash = hash;
  tabla->capacidad = capacidad;

  // Pedimos memoria para las casillas de la tabla
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidad);

  // Inicializamos las casillas con datos nulos.
  for (; idx < tabla->capacidad; ++idx) {
    tabla->tabla[idx] = NULL;
  }

  return tabla; // Retornamos la tabla
}

void tablahash_insertar(TablaHash *tabla, char *clave, void *dato, FuncionVisitante funcion) {
  unsigned idx;
  // Guarda la posicion donde insertara el dato.
  idx = tabla->hash(clave);
  idx = idx % tabla->capacidad;

  if (!tabla->tabla[idx])
     tabla->numElems ++;

  // Crea la casilla con los datos proporcionados
  CasillaHash casilla;
  casilla.clave = malloc(sizeof(char) * (strlen(clave) + 1));
  strcpy(casilla.clave, clave);
  casilla.dato = dato;

  // Inserta la casilla en la posicion dada de la tabla si es que no hay una 
  // con la misma clave, en ese caso, reemplaza su dato
  linked_list_insertar(&(tabla->tabla[idx]), casilla, funcion);
}

Contenedor *tablahash_buscar(TablaHash *tabla, char *clave) {
  // Guarda la posicion donde buscara el dato.
  unsigned idx = tabla->hash(clave);
  idx = idx % tabla->capacidad;

  // Guarda en dato el dato de la lista cuya clave es igual a la clave proporcionada
  void *dato = linked_list_buscar(tabla->tabla[idx], clave);

  // Si el dato no es nulo retorna un contenedor con el dato en el, en caso 
  // contrario retorna NULL
  return dato ? contenedor_crear(dato) : NULL;
}

void tablahash_destruir(TablaHash* tabla) {
  free(tabla->tabla); // Liberamos la memoria de las casillas de la tabla
  free(tabla); // Liberamos la memoria de la tabla
  tabla = NULL; // Le damos valore NULL
}

void tablahash_destruir_entera(TablaHash *tabla, FuncionVisitante funcion) {
  unsigned idx = 0;

  for (; idx < tabla->capacidad && tabla->numElems; idx ++) {
    if (tabla->tabla[idx]) { // Si la tabla en dada posicion no es nula
      // Destruyo la linked list de la tabla en dada posicion
      linked_list_eliminar(&(tabla->tabla[idx]), funcion);
      tabla->numElems --;
    }
  }

  // Destruye la tabla
  tablahash_destruir(tabla);
}
