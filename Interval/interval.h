#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include <stddef.h>
#include <stdio.h>

typedef struct _Interval Interval;

/*
  interval_crear: int int -> Interval*
  Dado dos numeros int, la funcion asigna la memoria de un interval,
  lo almacena con estos valores y retorna el puntero a donde esta almacenado
*/
Interval *interval_crear(int extremoIzq, int extremoDer);

/*
  interval_destruir: Interval*
  Dado un interval libera la memoria que ocupa, borrandolo.
*/
void interval_destruir(Interval * interval);

/*
  interval_extremo_izq: Interval* -> int
  Dado el puntero de un interval, retorna el valor del extremo izquierdo
  del interval almacenado en la direccion a la que apunta.
*/
int interval_extremo_izq(Interval * interval);

/*
  interval_extremo_izq: Interval* -> int
  Dado el puntero interval, retorna el valor del extremo derecho
  del interval almacenado en la direccion a la que apunta.
*/
int interval_extremo_der(Interval * interval);

Interval *interval_concat(Interval * interval1, Interval * interval2);

/*
  intervalo_imprimir: Intervalo*
  Dado un puntero intervalo lo imprime por pantalla.
*/
void interval_imprimir(Interval * interval);

/*
  intervalo_valido: Intervalo* -> int
  Dado un puntero intervalo, retorna 1 si es valido y imprime por pantalla un 
  error, 0 en caso contrario.
*/
int interval_valido(Interval * interval);

#endif                          /* __INTERVAL_H__ */