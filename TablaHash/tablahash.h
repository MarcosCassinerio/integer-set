#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef int (*FuncionHash) (char* clave);

typedef void (*FuncionVisitante) (void *dato);

typedef struct _CasillaHash CasillaHash;

typedef struct _LinkedList LinkedList;

typedef struct _TablaHash TablaHash;

/*
  tablahash_crear: FuncionHash unsigned -> TablaHash*
  Crea una nueva tabla Hash vacía, con la capacidad dada.
*/
TablaHash* tablahash_crear(FuncionHash hash, unsigned capacidad);

/*
  tablahash_insertar: TablaHash* char* void*
  Inserta el dato en la tabla, asociado a la clave dada.
*/
void tablahash_insertar(TablaHash* tabla, char *clave, void* dato, FuncionVisitante funcion);

/*
  tablahash_buscar: TablaHash* char* -> void*
  Busca un elemento dado en la tabla, y retorna el dato.
*/
void *tablahash_buscar(TablaHash* tabla, char *clave);

/*
  tablahash_destruir_entera: TablaHash* FuncionVisitante
  Destruye todos los elimentos de la tabla con la funcion dada.
*/
void tablahash_destruir_entera(TablaHash* tabla, FuncionVisitante funcion);

#endif /* __TABLAHASH_H__ */
