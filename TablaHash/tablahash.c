#include "tablahash.h"

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
struct _CasillaHash {
  char clave;
  void *dato;
};

/**
 * Estructura principal que representa la tabla hash.
 */
struct _TablaHash {
  CasillaHash *tabla;
  unsigned numElems;
  unsigned capacidad;
  unsigned profundidad;
  FuncionHash hash;
};

struct _Contenedor {
  void *dato;
};

Contenedor *contenedor_crear(void *dato) {
  Contenedor *contenedor = malloc(sizeof(Contenedor));
  contenedor->dato = dato;
  return contenedor;
}

void *contenedor_obtener_dato(Contenedor *contenedor) {
  return contenedor ? contenedor->dato : NULL;
}

TablaHash *tablahash_crear(FuncionHash hash, unsigned profundidad) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash *tabla = malloc(sizeof(TablaHash));
  unsigned idx = 0;
  tabla->numElems = 0;
  tabla->profundidad = profundidad;
  tabla->hash = hash;

  if (profundidad == PROFUNDIDAD_MAXIMA) {
    tabla->tabla = malloc(sizeof(CasillaHash) * capacidadTH1);
    tabla->capacidad = capacidadTH1;
  } else {
    tabla->tabla = malloc(sizeof(CasillaHash) * capacidadTH2);
    tabla->capacidad = capacidadTH2;
  }

  // Inicializamos las casillas con datos nulos.
  for (; idx < tabla->capacidad; ++idx) {
    tabla->tabla[idx].clave = ' ';
    tabla->tabla[idx].dato = NULL;
  }

  return tabla;
}

void tablahash_insertar(TablaHash *tabla, char *clave, void *dato) {
  TablaHash *tablaAux;
  unsigned idx;
  if (isalpha(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]) != 0)
    clave[PROFUNDIDAD_MAXIMA - tabla->profundidad] = toupper(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  if (tabla->profundidad == 0) {
    if (tabla->tabla[idx].clave == ' ') {
      tabla->numElems ++;
      tabla->tabla[idx].clave = clave[PROFUNDIDAD_MAXIMA - tabla->profundidad];
    }
    tabla->tabla[idx].dato = dato;
  } else {
    if (tabla->tabla[idx].clave == ' ') {
      tabla->numElems ++;
      tablaAux = tablahash_crear(tabla->hash, tabla->profundidad - 1);
      tabla->tabla[idx].clave = clave[PROFUNDIDAD_MAXIMA - tabla->profundidad];
      tabla->tabla[idx].dato = tablaAux;
    }
    tablahash_insertar(tabla->tabla[idx].dato, clave, dato);
  }
}

Contenedor *tablahash_buscar(TablaHash *tabla, char *clave) {
  unsigned idx;
  if (isalpha(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]) != 0)
    clave[PROFUNDIDAD_MAXIMA - tabla->profundidad] = toupper(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  if (tabla->profundidad == 0) {
    if (tabla->tabla[idx].clave != ' ')
      return contenedor_crear(tabla->tabla[idx].dato);
  } else {
    if (tabla->tabla[idx].clave != ' ')
      return tablahash_buscar(tabla->tabla[idx].dato, clave);
  }
  return NULL;
}

void tablahash_eliminar(TablaHash *tabla, char *clave, FuncionVisitante funcion) {
  TablaHash *tablaAux;
  unsigned idx = tabla->hash(clave[PROFUNDIDAD_MAXIMA - tabla->profundidad]);
  idx = idx % tabla->capacidad;

  if (tabla->profundidad == 0) {
    if (tabla->tabla[idx].clave != ' ') {
      tabla->numElems --;
      tabla->tabla[idx].clave = ' ';
      funcion(tabla->tabla[idx].dato);
    }
  } else {
    if (tabla->tabla[idx].clave != ' ') {
      tablaAux = tabla->tabla[idx].dato;
      tablahash_eliminar(tablaAux, clave, funcion);
      if (tablaAux->numElems == 0) {
        tabla->numElems --;
        tabla->tabla[idx].clave = ' ';
        tablahash_destruir(tablaAux);
      }
    }
  }
}

void tablahash_destruir(TablaHash* tabla) {
  free(tabla->tabla);
  free(tabla);
  tabla = NULL;
}

void tablahash_destruir_entera(TablaHash *tabla, FuncionVisitante funcion) {
  unsigned idx = 0;

  for (; idx < tabla->capacidad && tabla->numElems; idx ++) {
    if (tabla->tabla[idx].clave != ' ') {
      tabla->numElems --;
      if (tabla->profundidad == 0)
        funcion(tabla->tabla[idx].dato);
      else
        tablahash_destruir_entera(tabla->tabla[idx].dato, funcion);
    }
  }

  tablahash_destruir(tabla);
}
