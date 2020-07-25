#ifndef __SET_H__
#define __SET_H__

#include <limits.h>
#include "../Interval/interval.h"

typedef struct _Set *Set;

/*
  set_crear: int -> Set
  Dado un numero entero, crea un conjunto y le asigna ese numero al tamano de
  su array de intervalos.
*/
Set set_crear(int size);

/*
  set_destruir: void*
  Destruye el conjunto dado.
*/
void set_destruir(void *dato);

/*
  set_copia: Set -> Set
  Retorna una copia del conjunto dado.
*/
Set set_copia(Set set);

/*
  set_insertar: Set* Interval*
  Inserta el intervalo dado en el conjunto manteniendo el orden.
*/
void set_insertar(Set *set, Interval *interval);

/*
  set_unir: Set Set -> Set
  Dados 2 conjuntos, retorna su union.
*/
Set set_unir(Set set1, Set set2);

/*
  set_intersecar: Set Set -> Set
  Dados 2 conjuntos, retorna su interseccion.
*/
Set set_intersecar(Set set1, Set set2);

/*
  set_restar: Set Set -> Set
  Dados 2 conjuntos, retorna su diferencia.
*/
Set set_restar(Set set1, Set set2);

/*
  set_complemento: Set -> Set
  Dado un conjunto, retorna su complemento.
*/
Set set_complemento(Set set);

/*
  set_imprimir: Set
  Imprime el conjunto dado
*/
void set_imprimir(Set set);

#endif /* __SET_H__ */