#include "set.h"

struct _Set {
    int size;
    Interval **intervalArray;
};

/*
  set_insertar_ultimo: Set* Interval*
  Dado un conjunto y un intervalo, inserta dicho intervalo al final del array
  de intervalos del conjunto.
*/
void set_insertar_ultimo(Set *set, Interval *interval) {
    if (set && *set) { // Si el conjunto no es nulo
        (*set)->size++; // Aumenta el tamano del array de intervalos
        // Pide mas memoria para el array de intervalos
        (*set)->intervalArray = realloc((*set)->intervalArray, sizeof(Interval*) * (*set)->size);
        // Inserta el intervalo al final del array de intervalos
        (*set)->intervalArray[(*set)->size - 1] = interval;
    }
}

/*
  buscar_inicio_interseccion: Interval** Interval* int int -> int
  Dado un array de intervalos, un intervalo y sus limites, devuelve la posicion
  del menor intervalo del array de intervalos que interseca con el intervalo dado.
*/
int buscar_inicio_interseccion(Interval **intervalArray, Interval *interval, int inicio, int final) {
    if (inicio == final) {
        return inicio;
    }
    int posicion = (inicio + final) / 2;
    Interval *intervalAux = intervalArray[posicion - 1];
    // Si interval es menor a intervalAux
    if (interval_comparar(interval, intervalAux) <= 0)
        // Retorna la llamada a la funcion con nuevos limites
        return buscar_inicio_interseccion(intervalArray, interval, inicio, posicion);
    else {
        Interval *intervalAux = intervalArray[posicion];
        // Si interval es menor a intervalAux
        if (interval_comparar(interval, intervalAux) <= 0)
            return posicion + 1;
        // Retorna la llamada a la funcion con nuevos limites
        return buscar_inicio_interseccion(intervalArray, interval, posicion + 1, final);
    }
}

Set set_crear() {
    Set set = malloc(sizeof(struct _Set)); // Pide memoria para el conjunto
    set->size = 0;  // Le asigna 0 al tamano del conjunto
    set->intervalArray = NULL; // Setea NULL en el array de intervalos
    return set; // Retorna el conjunto
}

void set_destruir(void *dato) {
    Set set = (Set) dato; // Castea el dato a un conjunto
    int posicion = 0;
    if (set) { // Si el conjunto es no nulo
        if (set->intervalArray) { // Si el array de intervalos es no nulo
            for (; posicion < set->size; posicion ++) {
                // Destruye el intervalo en dicha posicion
                interval_destruir(&(set->intervalArray[posicion]));
            }
            free(set->intervalArray); // Libera la memoria del array de intervalos
        }
        free(set); // Libera la memoria del conjunto
    }
}

Set set_copia(Set set) {
    int posicion = 0;
    Set salida = set_crear(); // Inicializa salida
    if (set) { // Si el conjunto es no nulo
        for (; posicion < set->size; posicion ++)
            // Inserta una copia del intervalo dado al final de salida
            set_insertar_ultimo(&salida, interval_copy(set->intervalArray[posicion]));
    }
    return salida; // Retorna salida
}

Set set_insertar(Set set, Interval *interval) {
    int posicion = 0;
    Interval *intervalAux;
    Set salida = NULL;
    if (!interval_valido(interval)) {
        printf("Intervalo invalido\n");
        return set;
    }
    salida = set_crear();
    if (!set) {
        set_insertar_ultimo(&salida, interval);
        return salida;
    }
    while (posicion < set->size) {
        intervalAux = NULL;
        if (!interval) {
            set_insertar_ultimo(&salida, interval_copy(set->intervalArray[posicion]));
            posicion ++;
        } else {
            intervalAux = interval_concat(interval, set->intervalArray[posicion]);
            if (intervalAux) {
                posicion ++;
                interval_destruir(&interval);
                interval = intervalAux;
            } else if (interval_extremo_izq(set->intervalArray[posicion]) < interval_extremo_izq(interval)){
                set_insertar_ultimo(&salida, interval_copy(set->intervalArray[posicion]));
                posicion ++;
            } else {
                set_insertar_ultimo(&salida, interval);
                interval = NULL;
            }
        }
    }
    if (interval)
        set_insertar_ultimo(&salida, interval);
    set_destruir(set);
    return salida;
}
//

Set set_unir(Set set1, Set set2) {
    int posicion1 = 0, posicion2 = 0, concatenado;
    Interval *intervalInsertar = NULL, *intervalAux;
    Set salida = set_crear(), setAux = NULL;
    if (!set1->size || !set2->size) {
        setAux = set1->size ? set1 : set2;
        salida = set_copia(setAux);
        return salida;
    }
    while (posicion1 < set1->size || posicion2 < set2->size) {
        if (posicion1 < set1->size && posicion2 < set2->size) {
            if (intervalInsertar) {
                concatenado = 0;
                intervalAux = interval_concat(intervalInsertar, set1->intervalArray[posicion1]);
                if (intervalAux) {
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion1 ++;
                    concatenado = 1;
                }
                intervalAux = interval_concat(intervalInsertar, set2->intervalArray[posicion2]);
                if (intervalAux) {
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion2 ++;
                    concatenado = 1;
                }
                if (!concatenado) {
                    set_insertar_ultimo(&salida, intervalInsertar);
                    intervalInsertar = NULL;
                }
            } else {
                intervalInsertar = interval_concat(set1->intervalArray[posicion1], set2->intervalArray[posicion2]);
                if (intervalInsertar) {
                    posicion1 ++;
                    posicion2 ++;
                } else if (interval_comparar(set1->intervalArray[posicion1], set2->intervalArray[posicion2]) < 0) {
                    set_insertar_ultimo(&salida, interval_copy(set1->intervalArray[posicion1]));
                    posicion1 ++;
                } else {
                    set_insertar_ultimo(&salida, interval_copy(set2->intervalArray[posicion2]));
                    posicion2 ++;
                }
            }
        } else if (posicion1 < set1->size) {
            if (intervalInsertar) {
                intervalAux = interval_concat(intervalInsertar, set1->intervalArray[posicion1]);
                if (intervalAux) {
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion1 ++;
                } else {
                    set_insertar_ultimo(&salida, intervalInsertar);
                    intervalInsertar = NULL;
                }
            } else {
                set_insertar_ultimo(&salida, interval_copy(set1->intervalArray[posicion1]));
                posicion1 ++;
            }
        } else {
            if (intervalInsertar) {
                intervalAux = interval_concat(intervalInsertar, set2->intervalArray[posicion2]);
                if (intervalAux) {
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion2 ++;
                } else {
                    set_insertar_ultimo(&salida, intervalInsertar);
                    intervalInsertar = NULL;
                }
            } else {
                set_insertar_ultimo(&salida, interval_copy(set2->intervalArray[posicion2]));
                posicion2 ++;
            }
        }
    }
    if (intervalInsertar)
        set_insertar_ultimo(&salida, intervalInsertar);
    return salida;
}

Set set_intersecar(Set set1, Set set2) {
    int posicion1 = 0, posicion2 = 1, terminado;
    Interval *interseccion;
    Set salida = set_crear();
    if (!set1->size || !set2->size)
        return salida;
    for (; posicion1 < set1->size && posicion2 <= set2->size; posicion1 ++) {
        terminado = 0;
        posicion2 = buscar_inicio_interseccion(set2->intervalArray, set1->intervalArray[posicion1], posicion2, set2->size);
        while (posicion2 <= set2->size && !terminado) {
            interseccion = interval_interseccion(set1->intervalArray[posicion1], set2->intervalArray[posicion2 - 1]);
            if (interseccion) {
                set_insertar_ultimo(&salida, interseccion);
                posicion2 ++;
            } else
                terminado = 1;
        }
    }
    return salida;
}

Set set_restar(Set set1, Set set2) {
    Set salida1 = set_complemento(set2);
    Set salida2 = set_intersecar(set1, salida1);
    set_destruir(salida1);
    return salida2;
}

Set set_complemento(Set set) {
    int posicion = 0, anterior = INT_MIN;
    Set salida = set_crear();
    for (; posicion < set->size; posicion ++) {
        if (interval_extremo_izq(set->intervalArray[posicion]) > anterior)
            set_insertar_ultimo(&salida, interval_crear(anterior, interval_extremo_izq(set->intervalArray[posicion]) - 1));
        if (interval_extremo_der(set->intervalArray[posicion]) != INT_MAX)
            anterior = interval_extremo_der(set->intervalArray[posicion]) + 1;
        else
            anterior = interval_extremo_der(set->intervalArray[posicion]);
    }
    if (anterior != INT_MAX)
        set_insertar_ultimo(&salida, interval_crear(anterior, INT_MAX));
    return salida;
}

void set_imprimir(Set set) {
    int posicion = 0;
    while (posicion < set->size) {
        interval_imprimir(set->intervalArray[posicion]);
        if (posicion < (set->size - 1))
            printf(", ");
        posicion ++;
    }
}
