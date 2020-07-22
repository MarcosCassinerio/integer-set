#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include <assert.h>
#include <stdlib.h>
#define capacidadTH1 26
#define capacidadTH2 10
#define PROFUNDIDAD_MAXIMA 4

/**
 * Tipo de las funciones hash a ser consideradas por las tablas hash.
 */
typedef int (*FuncionHash) (char clave);

typedef void (*FuncionVisitante) (void *dato);

typedef struct _CasillaHash CasillaHash;

typedef struct _TablaHash TablaHash;

typedef struct _Contenedor Contenedor;

void *contenedor_obtener_dato(Contenedor *contenedor);

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(FuncionHash hash, unsigned profundidad);

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, char *clave, void* dato);

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
Contenedor *tablahash_buscar(TablaHash* tabla, char *clave);

/**
 * Elimina un elemento de la tabla.
 */
void tablahash_eliminar(TablaHash* tabla, char *clave, FuncionVisitante funcion);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla);

void tablahash_destruir_entera(TablaHash* tabla, FuncionVisitante funcion);

#endif /* __TABLAHASH_H__ */
