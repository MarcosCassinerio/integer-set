#include "tablahash.h"

struct _Contenedor {
  void *dato;
};

struct _CasillaHash {
  char clave;
  void *dato;
};

struct _TablaHash {
  CasillaHash *tabla;
  unsigned numElems;
  unsigned capacidad;
  unsigned profundidad;
  FuncionHash hash;
};

Contenedor *contenedor_crear(void *dato) {
  Contenedor *contenedor = malloc(sizeof(Contenedor)); // Pedimos memoria
  contenedor->dato = dato; // Le asignamos el dato dado
  return contenedor; // Retornamos el contenedor
}

void *contenedor_obtener_dato(Contenedor *contenedor) {
  // Si el contenedor no es nulo devuelve el dato, en caso contrario devuelve NULL
  return contenedor ? contenedor->dato : NULL;
}

TablaHash *tablahash_crear(FuncionHash hash, unsigned profundidad) {
  // Pedimos memoria para la estructura principal.
  TablaHash *tabla = malloc(sizeof(TablaHash));
  unsigned idx = 0;
  // Asignamos sus valores
  tabla->numElems = 0;
  tabla->profundidad = profundidad;
  tabla->hash = hash;

  // Dependiendo de su profundidad asigna la capacidad a la tabla
  if (profundidad == PROFUNDIDAD_MAXIMA) {
    tabla->capacidad = capacidadTH1;
  } else {
    tabla->capacidad = capacidadTH2;
  }

  // Pedimos memoria para las casillas de la tabla
  tabla->tabla = malloc(sizeof(CasillaHash) * tabla->capacidad);

  // Inicializamos las casillas con datos nulos.
  for (; idx < tabla->capacidad; ++idx) {
    tabla->tabla[idx].clave = ' ';
    tabla->tabla[idx].dato = NULL;
  }

  return tabla; // Retornamos la tabla
}

void tablahash_insertar(TablaHash *tabla, char *clave, void *dato) {
  TablaHash *tablaAux;
  unsigned idx;
  // Si la clave en esa posicion es una letra guarda su mayuscula
  if (isalpha(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]) != 0)
    clave[PROFUNDIDAD_MAXIMA - tabla->profundidad] = toupper(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  // Guarda la posicion donde insertara el dato.
  idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  // Le asigna la clave a la casilla dada
  tabla->tabla[idx].clave = clave[PROFUNDIDAD_MAXIMA - tabla->profundidad];

  if (tabla->profundidad == 0) { // Si la profundidad es 0
    if (tabla->tabla[idx].clave == ' ') // Si la clave es un espacio
      tabla->numElems ++; // Aumenta en uno la cantidad de elementos
    tabla->tabla[idx].dato = dato; // Le asigna el dato dado
  } else {
    if (tabla->tabla[idx].clave == ' ') { // Si la clave es un espacio
      tabla->numElems ++; // Aumenta en uno la cantidad de elementos
      // Crea una tabla y la inserta en el dato de la casilla
      tablaAux = tablahash_crear(tabla->hash, tabla->profundidad - 1);
      tabla->tabla[idx].dato = tablaAux;
    }
    // Inserta el dato en la casilla de la tabla
    tablahash_insertar(tabla->tabla[idx].dato, clave, dato);
  }
}

Contenedor *tablahash_buscar(TablaHash *tabla, char *clave) {
  unsigned idx;
  // Si la clave en esa posicion es una letra guarda su mayuscula
  if (isalpha(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]) != 0)
    clave[PROFUNDIDAD_MAXIMA - tabla->profundidad] = toupper(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  // Guarda la posicion donde buscara el dato.
  idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  if (tabla->profundidad == 0) { // Si la profundidad es 0
    if (tabla->tabla[idx].clave != ' ') // Si la clave no es un espacio
      // Retorna un contenedor con el dato en el
      return contenedor_crear(tabla->tabla[idx].dato);
  } else {
    if (tabla->tabla[idx].clave != ' ') // Si la clave no es un espacio
      // Retorna la llamada a la funcion con el dato de la casilla
      return tablahash_buscar(tabla->tabla[idx].dato, clave);
  }
  return NULL; // Retorna NULL
}

void tablahash_eliminar(TablaHash *tabla, char *clave, FuncionVisitante funcion) {
  TablaHash *tablaAux;
  unsigned idx;
    // Si la clave en esa posicion es una letra guarda su mayuscula
  if (isalpha(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]) != 0)
    clave[PROFUNDIDAD_MAXIMA - tabla->profundidad] = toupper(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  // Guarda la posicion de donde eliminara el dato.
  idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  if (tabla->profundidad == 0) { // Si la profundidad es 0
    if (tabla->tabla[idx].clave != ' ') { // Si la clave no es un espacio
      tabla->numElems --; // Disminuye la cantidad de elementos en uno
      tabla->tabla[idx].clave = ' '; // Le asigna un espacio a la clave
      funcion(tabla->tabla[idx].dato);  // Destruye el dato de la casilla
    }
  } else {
    if (tabla->tabla[idx].clave != ' ') { // Si la clave no es un espacio
      tablaAux = tabla->tabla[idx].dato; 
      // Llama a la funcion en el dato de la casilla de la tabla
      tablahash_eliminar(tablaAux, clave, funcion);
      if (tablaAux->numElems == 0) { // Si el numero de elementos es 0
        tabla->numElems --; // Disminuye la cantidad de elementos en uno
        tabla->tabla[idx].clave = ' '; // Le asigna un espacio a la clave
        tablahash_destruir(tablaAux); // Llama a tablahash_destruir en la tabla
      }
    }
  }
}

void tablahash_destruir(TablaHash* tabla) {
  free(tabla->tabla); // Liberamos la memoria de las casillas de la tabla
  free(tabla); // Liberamos la memoria de la tabla
  tabla = NULL; // Le damos valore NULL
}

void tablahash_destruir_entera(TablaHash *tabla, FuncionVisitante funcion) {
  unsigned idx = 0;

  for (; idx < tabla->capacidad && tabla->numElems; idx ++) {
    // Si la clave no es un espacio
    if (tabla->tabla[idx].clave != ' ') {
      tabla->numElems --; // Disminuye la cantidad de elementos en uno
      if (tabla->profundidad == 0) // Si la profundidad es 0
        // Destruye el dato de la casilla
        funcion(tabla->tabla[idx].dato);
      else
        // Llama a la funcion en el dato de la casilla
        tablahash_destruir_entera(tabla->tabla[idx].dato, funcion);
    }
  }

  // Destruye la tabla
  tablahash_destruir(tabla);
}
