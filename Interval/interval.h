#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

typedef struct _Interval Interval;

/*
  interval_crear: int int -> Interval*
  Dado dos numeros int, la funcion asigna la memoria de un interval,
  lo almacena con estos valores y retorna el puntero a donde esta almacenado
*/
Interval *interval_crear(int extremoIzq, int extremoDer);

/*
  interval_destruir: Interval**
  Dado un interval libera la memoria que ocupa, borrandolo.
*/
void interval_destruir(Interval **interval);

/*
  interval_extremo_izq: Interval* -> int
  Dado el puntero de un interval, retorna el valor del extremo izquierdo
  del interval almacenado en la direccion a la que apunta.
*/
int interval_extremo_izq(Interval *interval);

/*
  interval_extremo_izq: Interval* -> int
  Dado el puntero interval, retorna el valor del extremo derecho
  del interval almacenado en la direccion a la que apunta.
*/
int interval_extremo_der(Interval *interval);

/*
  interval_concat: Interval* Interval* -> Interval*
  Dados 2 intervalos, si estos se intersecan o son contiguos retorna la union
  de ellos, en caso contrario retorna un intervalo nulo.
*/
Interval *interval_concat(Interval *interval1, Interval *interval2);

/*
  intervalo_imprimir: Intervalo*
  Dado un puntero intervalo lo imprime por pantalla.
*/
void interval_imprimir(Interval *interval);

/*
  intervalo_valido: Intervalo* -> int
  Dado un puntero intervalo, retorna 1 si es valido. En caso contrario retorna 0
  y lo elimina si no es nulo.
*/
int interval_valido(Interval *interval);

/*
  interval_interseccion: Interval* Interval* -> Interval*
  Dados 2 intervalos, retorna su interseccion.
*/
Interval *interval_interseccion(Interval *interval1, Interval *interval2);

/*
  interval_comparar: Interval* Interval* -> int
  Dados 2 intervalos, retorna un numero mayor a 0 si el extremo izquierdo del
  primer intervalo es mayor al del segundo, 0 si son iguales y uno menor a 0
  si es menor en caso contrario.
*/
int interval_comparar(Interval *interval1, Interval *interval2);

/*
  interval_copy: Interval* -> Interval*
  Dado un intervalo, retorna una copia de este.
*/
Interval *interval_copy(Interval *interval);

#endif                          /* __INTERVAL_H__ */