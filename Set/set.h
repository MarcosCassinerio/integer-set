#ifndef __SET_H__
#define __SET_H__

#include <limits.h>
#include "../Interval/interval.h"

typedef struct _Set *Set;

Set set_crear();

void set_destruir(Set set);

Set set_copia(Set set);

Set set_insertar(Set set, Interval *interval);

Set set_unir(Set set1, Set set2);

Set set_intersecar(Set set1, Set set2);

Set set_restar(Set set1, Set set2);

Set set_complemento(Set set);

void set_imprimir(Set set);

#endif /* __SET_H__ */