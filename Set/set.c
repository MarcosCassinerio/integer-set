#include "set.h"

struct _Set {
    int size;
    Interval **intervalArray;
};

/*
  set_insertar_ultimo: Set* Interval*
  Dado un conjunto y un intervalo, inserta dicho intervalo en la siguiente
  posicion del array de intervalos.
*/
void set_insertar_ultimo(Set *set, Interval *interval) {
    if (set && *set) { // Si el conjunto no es nulo
        // Inserta el intervalo en la siguiente posicion del array de intervalos
        (*set)->intervalArray[(*set)->size] = interval;
        (*set)->size++; // Aumenta el tamano de la cantidad de intervalos actuales
    }
}
// MIN:-1, 4:MAX
// 4:5
// 1 2

/*
  buscar_inicio_interseccion: Interval** Interval* int int -> int
  Dado un array de intervalos, un intervalo y sus limites, devuelve la posicion
  del menor intervalo del array de intervalos que interseca con el intervalo dado.
*/
int buscar_inicio_interseccion(Interval **intervalArray, Interval *interval, int inicio, int final) {
    if (inicio == final)
        return inicio;
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

Set set_crear(int size) {
    Set set = malloc(sizeof(struct _Set)); // Pide memoria para el conjunto
    set->size = 0;  // Le asigna 0 al tamano del conjunto
    // Le asigna el tamano al array de intervalos
    set->intervalArray = malloc(sizeof(Interval*) * size);
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
    Set salida = set_crear(set->size); // Inicializa salida
    if (set) { // Si el conjunto es no nulo
        for (; posicion < set->size; posicion ++)
            // Inserta una copia del intervalo dado al final de salida
            set_insertar_ultimo(&salida, interval_copy(set->intervalArray[posicion]));
    }
    return salida; // Retorna salida
}

void set_insertar(Set *set, Interval *interval) {
    int posicion = 0;
    Interval *intervalAux;
    Set salida = NULL;

    if (!set || !(*set)) // Si el conjunto es nulo
        *set = set_crear(1); // Inicializa el conjunto con tamano 1

    if (interval_valido(interval)) { // Si el intervalo es valido
        // Inicializa salida con un tamano uno mayor al conjunto
        salida = set_crear((*set)->size + 1);
        while (posicion < (*set)->size) {
            intervalAux = NULL;
            if (!interval) { // Si el intervalo es nulo
                // Inserta el intervalo del conjunto al final de salida
                set_insertar_ultimo(&salida, (*set)->intervalArray[posicion]);
                posicion ++;
            } else {
                intervalAux = interval_concat(interval, (*set)->intervalArray[posicion]);
                if (intervalAux) { // Si intervalAux es no nulo
                    interval_destruir(&interval); // Destruye interval
                    // Destruye el intervalo actual del conjunto
                    interval_destruir(&((*set)->intervalArray[posicion]));
                    posicion ++;
                    interval = intervalAux;
                // Si el intervalo del conjunto es menor a interval
                } else if (interval_comparar((*set)->intervalArray[posicion], interval) < 0){
                    // Inserta el intervalo del conjunto al final de salida
                    set_insertar_ultimo(&salida, (*set)->intervalArray[posicion]);
                    posicion ++;
                } else {
                    // Inserta el intervalo al final de salida
                    set_insertar_ultimo(&salida, interval);
                    interval = NULL;
                }
            }
        }
        if ((*set)->intervalArray) // Si el array de intervalos no es nulo
            free((*set)->intervalArray); // Libera la memoria del array de intervalos
        free(*set); // Libera la memoria del conjunto
        if (interval) // Si el intervalo es no nulo
            // Inserta el intervalo al final de salida
            set_insertar_ultimo(&salida, interval);
        *set = salida; // El conjunto apunta a salida
    }
}

Set set_unir(Set set1, Set set2) {
    int posicion1 = 0, posicion2 = 0, concatenado, *posicion;
    Interval *intervalInsertar = NULL, *intervalAux;
    Set salida = set_crear(set1->size + set2->size), setAux = NULL;
    // Si el tamano de alguno de los conjuntos es 0
    if (!set1->size || !set2->size) {
        setAux = set1->size ? set1 : set2;
        // Guarda en salida una copia del nodo cuyo tamano no es 0 o uno de ellos
        // si ambos tienen tamano 0
        salida = set_copia(setAux);
        return salida; // Retorna salida
    }
    while (posicion1 < set1->size || posicion2 < set2->size) {
        // Si el tamano de ambos conjuntos no es 0
        if (posicion1 < set1->size && posicion2 < set2->size) {
            if (intervalInsertar) { // Si intervalInsertar no es nulo
                concatenado = 0;
                // Guarda en intervalAux la union de ambos conjuntos si es que
                // resulta en un solo intervalo
                intervalAux = interval_concat(intervalInsertar, set1->intervalArray[posicion1]);
                if (intervalAux) { // Si intervalAux no es nulo
                    // Destruye intervalInsertar
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion1 ++;
                    concatenado = 1;
                }
                // Guarda en intervalAux la union de ambos intervalos si es que
                // resulta en un solo intervalo
                intervalAux = interval_concat(intervalInsertar, set2->intervalArray[posicion2]);
                if (intervalAux) { // Si intervalAux no es nulo
                    // Destruye intervalInsertar
                    interval_destruir(&intervalInsertar);
                    intervalInsertar = intervalAux;
                    posicion2 ++;
                    concatenado = 1;
                }
                if (!concatenado) {
                    // Inserta intervalInsertar al final de salida
                    set_insertar_ultimo(&salida, intervalInsertar);
                    intervalInsertar = NULL;
                }
            } else {
                // Guarda en intervalInsertar la union de ambos intervalos si es que
                // resulta en un solo intervalo
                intervalInsertar = interval_concat(set1->intervalArray[posicion1], set2->intervalArray[posicion2]);
                if (intervalInsertar) { // Si intervalInsertar no es nulo
                    posicion1 ++;
                    posicion2 ++;
                } else {
                    // Guarda en setAux el conjunto con el menor intervalo actual
                    setAux = interval_comparar(set1->intervalArray[posicion1], set2->intervalArray[posicion2]) < 0 ? set1 : set2;
                    // Guarda en posicion la posicion del conjunto con menor intervalo actual
                    posicion = interval_comparar(set1->intervalArray[posicion1], set2->intervalArray[posicion2]) < 0 ? &posicion1 : &posicion2;
                    // Inserta el intervalo actual de setAux al final de salida
                    set_insertar_ultimo(&salida, interval_copy(setAux->intervalArray[*posicion]));
                    (*posicion) ++;
                }
            }
        } else {
            // Guarda en setAux el conjunto que no se haya recorrido entero
            setAux = posicion1 < set1->size ? set1 : set2;
            // Guarda en posicion la posicion del conjunto que no se haya recorrido entero
            posicion = posicion1 < set1->size ? &posicion1 : &posicion2;
            if (intervalInsertar) { // Si intervalInsertar no es nulo
                // Guarda en intervalAux la union de ambos intervalos si es que resulta
                // en un solo intervalo
                intervalAux = interval_concat(intervalInsertar, setAux->intervalArray[*posicion]);
                if (intervalAux) { // Si intervalAux no es nulo
                    interval_destruir(&intervalInsertar); // Destruye intervalInsertar
                    intervalInsertar = intervalAux; // Guarda intervalAux en intervalInsertar
                    (*posicion) ++;
                } else {
                    // Inserta intervalInsertar al final de salida
                    set_insertar_ultimo(&salida, intervalInsertar);
                    intervalInsertar = NULL;
                }
            } else {
                // Inserta el intervalo actual de setAux al final de salida
                set_insertar_ultimo(&salida, interval_copy(setAux->intervalArray[*posicion]));
                (*posicion) ++;
            }
        }
    }
    if (intervalInsertar) // Si intervalInsertar no es nulo
        // Inserta intervalInsertar al final de salida
        set_insertar_ultimo(&salida, intervalInsertar);
    return salida; // Retorna salida
}
// -3:-1 4:5
// MIN:-1 4:MAX

Set set_intersecar(Set set1, Set set2) {
    int posicion1 = 0, posicion2 = 1, terminado;
    Interval *interseccion;
    // Inicializa salida con un tamano uno mayor a la suma del tamano de los conjuntos
    Set salida = set_crear(set1->size + set2->size - 1);
    if (!set1->size || !set2->size) // Si el tamano de alguno de los conjuntos es 0
        return salida;
    for (; posicion1 < set1->size && posicion2 <= set2->size; posicion1 ++) {
        terminado = 0;
        // Halla la posicion del menor intervalo del conjunto 2 que interseca 
        // con el intervalo actual si es que existe
        posicion2 = buscar_inicio_interseccion(set2->intervalArray, set1->intervalArray[posicion1], posicion2, set2->size);
        while (posicion2 <= set2->size && !terminado) {
            // Guarda en interseccion la interseccion entre los intervalos
            interseccion = interval_interseccion(set1->intervalArray[posicion1], set2->intervalArray[posicion2 - 1]);
            if (interseccion) { // Si interseccion no es nulo
                // Inserta la interseccion al final del conjunto de salida
                set_insertar_ultimo(&salida, interseccion);
                posicion2 ++;
            } else
                terminado = 1;
        }
        if (posicion2 != 1) // Si posicion2 es diferente a la posicion inicial
          posicion2 --;
    }
    return salida;
}

Set set_restar(Set set1, Set set2) {
    Set salida2 = NULL;
    // Guarda en salida1 el complemento del conjunto2
    Set salida1 = set_complemento(set2);
    // Guarda en salida2 la interseccion entre el conjunto1 y salida1
    salida2 = set_intersecar(set1, salida1);
    // Destruye salida1 ya que era un conjunto auxiliar
    set_destruir(salida1);
    return salida2; // Retorna salida2
}

Set set_complemento(Set set) {
    int posicion = 0, anterior = INT_MIN;
    // Inicializa salida con un tamano uno mayor al conjunto
    Set salida = set_crear(set->size + 1);
    for (; posicion < set->size; posicion ++) {
        // Si el extremo izquierdo del nodo actual es mayor a anterior
        if (interval_extremo_izq(set->intervalArray[posicion]) > anterior)
            // Inserta el intervalo creado al final de salida
            set_insertar_ultimo(&salida, interval_crear(anterior, interval_extremo_izq(set->intervalArray[posicion]) - 1));
        // Si el extremo derecho del intervalo es diferente a INT_MAX
        if (interval_extremo_der(set->intervalArray[posicion]) != INT_MAX)
            // Le asigna a anterior el extremo derecho del intervalo mas uno
            anterior = interval_extremo_der(set->intervalArray[posicion]) + 1;
        else
            // En caso contrario le asigna el extremo derecho del intervalo
            anterior = interval_extremo_der(set->intervalArray[posicion]);
    }
    if (anterior != INT_MAX) // Si anterior es diferente a INT_MAX
        // Inserta el intervalo creado al final de salida
        set_insertar_ultimo(&salida, interval_crear(anterior, INT_MAX));
    return salida; // Retorna salida
}

void set_imprimir(Set set) {
    int posicion = 0;
    while (posicion < set->size) {
        // Imprime el intervalo actual
        interval_imprimir(set->intervalArray[posicion]);
        // Si el intervalo actual no es el ultimo
        if (posicion < (set->size - 1))
            printf(", ");
        posicion ++;
    }
}
