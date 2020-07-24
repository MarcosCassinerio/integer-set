#include "interval.h"

struct _Interval {
  int extremoIzq;
  int extremoDer;
};

Interval *interval_crear(int extremoIzq, int extremoDer) {
  // Pedimos memoria
  Interval *interval = (Interval *) malloc(sizeof(Interval));
  interval->extremoIzq = extremoIzq;   // Asignamos los valores
  interval->extremoDer = extremoDer;
  return interval;             // Retornamos el puntero al interval
}

void interval_destruir(Interval **interval) {
  if (*interval)
    free(*interval);
  *interval = NULL;
}

int interval_extremo_izq(Interval *interval) {
  return interval->extremoIzq;
}

int interval_extremo_der(Interval *interval) {
  return interval->extremoDer;
}

Interval *interval_concat(Interval *interval1, Interval *interval2) {
  if (interval_extremo_izq(interval2) < interval_extremo_izq(interval1))
    return interval_concat(interval2, interval1);
  if (interval_extremo_izq(interval1) <= interval_extremo_der(interval2) && interval_extremo_der(interval1) >= interval_extremo_izq(interval2)) {
      int extremo_izq = interval_extremo_izq(interval1) < interval_extremo_izq(interval2) ? interval_extremo_izq(interval1) : interval_extremo_izq(interval2);
      int extremo_der = interval_extremo_der(interval1) > interval_extremo_der(interval2) ? interval_extremo_der(interval1) : interval_extremo_der(interval2);
      return interval_crear(extremo_izq, extremo_der);
  }
  if (interval_extremo_der(interval1) + 1 == interval_extremo_izq(interval2))
      return interval_crear(interval_extremo_izq(interval1), interval_extremo_der(interval2));
  return NULL;
}

void interval_imprimir(Interval *interval) {
  if (interval) {
    if (interval_extremo_izq(interval) == interval_extremo_der(interval))
      printf("%d", interval_extremo_izq(interval));
    else
      printf("%d:%d", interval_extremo_izq(interval), interval_extremo_der(interval));
  }
}

int interval_valido(Interval *interval) {
  // Si no existe el intervalo o  el extremo izquierdo del intervalo es mayor al extremo derecho
  if (!interval) {
    if (interval->extremoIzq > interval->extremoDer)
      interval_destruir(&interval); // Eliminamos el intervalo ya que es invalido
    return 0;
  }
  return 1;
}

Interval *interval_interseccion(Interval *interval1, Interval *interval2) {
  if (interval_extremo_izq(interval1) <= interval_extremo_der(interval2) && interval_extremo_der(interval1) >= interval_extremo_izq(interval2))
    return interval_crear(MAX(interval_extremo_izq(interval1), interval_extremo_izq(interval2)), MIN(interval_extremo_der(interval1), interval_extremo_der(interval2)));
  return NULL;                              
}

int interval_comparar(Interval *interval1, Interval *interval2) {
  return interval_extremo_izq(interval1) - interval_extremo_izq(interval2);
}

Interval *interval_copy(Interval *interval) {
  if (!interval)
    return NULL;
  return interval_crear(interval_extremo_izq(interval), interval_extremo_der(interval));
}
