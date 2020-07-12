#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include <assert.h>
#define capacidadTH1 26
#define capacidadTH2 10

/**
 * Tipo de las funciones hash a ser consideradas por las tablas hash.
 */
typedef int (*FuncionHash)(char clave);

typedef struct _CasillaHash CasillaHash;

typedef struct _TablaHash TablaHash;

typedef struct _Contenedor Contenedor;

/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(FuncionHash hash);

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
void tablahash_insertar(TablaHash* tabla, char clave1, char clave2, void* dato);

/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
void* tablahash_buscar(TablaHash* tabla, char clave1, char clave2);

/**
 * Elimina un elemento de la tabla.
 */
void tablahash_eliminar(TablaHash* tabla, char clave1, char clave2);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash* tabla);

#endif /* __TABLAHASH_H__ */
