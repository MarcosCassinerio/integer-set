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
  FuncionHash hash;
};

struct _Contenedor {
  void *dato;
};

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash *tablahash_crear(FuncionHash hash) {
  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash *tabla = malloc(sizeof(TablaHash));
  tabla->hash = hash;
  tabla->capacidad = capacidadTH1;
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidadTH1);
  tabla->numElems = 0;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidadTH1; ++idx) {
    tabla->tabla[idx].clave = ' ';
    tabla->tabla[idx].dato = NULL;
  }

  return tabla;
}

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, char clave1, char clave2, void* dato) {
  TablaHash *tabla2 = NULL;
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(clave1);
  idx = idx % tabla->capacidad;
    
  unsigned idy = tabla->hash(clave2);
  idy = idy % tabla->capacidad;

  // Si el lugar estaba vacío, incrementamos el número de elementos.
  if (tabla->tabla[idx].clave == ' ') {
    tabla->numElems++;

    tabla2 = tablahash_crear(tabla->hash);

    tabla2->tabla[idy].clave = clave2;
    tabla2->tabla[idy].dato = dato;

    tabla->tabla[idx].clave = clave1;
    tabla->tabla[idx].dato = tabla2;
  } else {
    tabla2 = tabla->tabla[idx].dato;

    if (tabla2->tabla[idy].clave == ' ')
      tabla2->numElems++;
    
    tabla2->tabla[idy].clave = clave2;
    tabla2->tabla[idy].dato = dato;
  }
}

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
Contenedor *tablahash_buscar(TablaHash* tabla, char clave1, char clave2) {
  TablaHash *tabla2 = NULL;
  Contenedor *contenedor = NULL;
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(clave1);
  idx = idx % tabla->capacidad;
    
  unsigned idy = tabla->hash(clave2);
  idy = idy % tabla->capacidad;

  // Si el lugar esta vacío, retornamos un puntero nulo.
  if (tabla->tabla[idx].clave != clave1)
    return NULL;

  tabla2 = tabla->tabla[idx].dato;

  if (tabla2->tabla[idy].clave != clave2)
    return NULL;

  contenedor = malloc(sizeof(Contenedor));
  contenedor->dato = tabla2->tabla[idy].dato;
  
  return contenedor;
}

/**
 * Elimina un elemento de la tabla.
 */
void tablahash_eliminar(TablaHash* tabla, char clave1, char clave2) {
  TablaHash *tabla2 = NULL;
  // Calculamos la posición de la clave dada, de acuerdo a la función hash.
  unsigned idx = tabla->hash(clave1);
  idx = idx % tabla->capacidad;
    
  unsigned idy = tabla->hash(clave2);
  idy = idy % tabla->capacidad;

  // Si el lugar estaba ocupado, decrementamos el número de elementos.
  if (tabla->tabla[idx].clave != ' ') {
    tabla2 = tabla->tabla[idx].dato;
    if (tabla2->tabla[idy].clave != ' ') {
      tabla2->numElems--;
      if (tabla2->numElems == 0) {
        tablahash_destruir(tabla2);
        
        tabla->numElems--;

        tabla->tabla[idx].clave = ' ';
        tabla->tabla[idx].dato = NULL;
      } else {
        tabla2->tabla[idy].clave = ' ';
        tabla2->tabla[idy].dato = NULL;
      }
    }
  }
}

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla) {
  free(tabla->tabla);
  free(tabla);
}
