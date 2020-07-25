#include "interval.h"

struct _Interval {
  int extremoIzq;
  int extremoDer;
};

Interval *interval_crear(int extremoIzq, int extremoDer) {
  Interval *interval = (Interval *) malloc(sizeof(Interval)); // Pedimos memoria
  // Asignamos los valores
  interval->extremoIzq = extremoIzq;
  interval->extremoDer = extremoDer;
  return interval;             // Retornamos el puntero al interval
}

void interval_destruir(Interval **interval) {
  if (*interval) // Si el intervalo no es nulo
    free(*interval);  // Libera su memoria
  *interval = NULL; // Le setea NULL
}

int interval_extremo_izq(Interval *interval) {
  return interval->extremoIzq;
}

int interval_extremo_der(Interval *interval) {
  return interval->extremoDer;
}

Interval *interval_concat(Interval *interval1, Interval *interval2) {
  // Si el extremo izquierdo del segundo intervalo es menor al del segundo
  if (interval2->extremoIzq < interval1->extremoIzq)
    // Llama a la funcion con las variables cambiadas
    return interval_concat(interval2, interval1);
  // Si los intervalos se intersecan o son contiguos devuelve su union
  if (interval1->extremoIzq <= interval2->extremoDer && interval1->extremoDer >= interval2->extremoIzq) {
      int extremo_izq = interval1->extremoIzq < interval2->extremoIzq ? interval1->extremoIzq : interval2->extremoIzq;
      int extremo_der = interval1->extremoDer > interval2->extremoDer ? interval1->extremoDer : interval2->extremoDer;
      return interval_crear(extremo_izq, extremo_der);
  }
  if (interval1->extremoDer + 1 == interval2->extremoIzq)
      return interval_crear(interval1->extremoIzq, interval2->extremoDer);
  // En caso contrario devuelve NULL
  return NULL;
}

void interval_imprimir(Interval *interval) {
  if (interval) { // Si el intervalo no es nulo
    // Si el extremo izquierdo del intervalo es igual al derecho
    if (interval->extremoIzq == interval->extremoDer)
      printf("%d", interval->extremoIzq);
    // En caso contrario
    else
      printf("%d:%d", interval->extremoIzq, interval->extremoDer);
  }
}

int interval_valido(Interval *interval) {
  if (interval) { // Si el intervalo no es nulo
    // Si el extremo izquierdo es mayor al derecho
    if (interval->extremoIzq > interval->extremoDer) {
      interval_destruir(&interval); // Eliminamos el intervalo ya que es invalido
      return 0;
    } else
      return 1;
  }
  return 0;
}

Interval *interval_interseccion(Interval *interval1, Interval *interval2) {
  // Si los intervalos se intersecan
  if (interval1->extremoIzq <= interval2->extremoDer && interval1->extremoDer >= interval2->extremoIzq)
    // Retorna su interseccion
    return interval_crear(MAX(interval1->extremoIzq, interval2->extremoIzq), MIN(interval1->extremoDer, interval2->extremoDer));
  // En caso contrario retorna NULL
  return NULL;                              
}

int interval_comparar(Interval *interval1, Interval *interval2) {
  // Retorna la diferencia de sus intervalos izquierdos
  return interval1->extremoIzq - interval2->extremoIzq;
}

Interval *interval_copy(Interval *interval) {
  if (!interval) // Si el intervalo es nulo
    return NULL;
  // Retorna una copia del intervalo
  return interval_crear(interval->extremoIzq, interval->extremoDer);
}
