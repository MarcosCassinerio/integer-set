#include "./Interval/interval.h"
#include "./Set/set.h"
#include "./TablaHash/tablahash.h"
#include <stdint.h>
#include <math.h>
#define LEN_MAX 100
#define MAX_INTERVALS 50

int hash(char clave) {
    return clave;
}

/*
    leer_cadena: char* -> char*
    Toma lo escrito por el teclado, y lo retorna reemplazando el salto de linea
    por un '\0'.
*/
char *leer_cadena(char **string) {
    char c;
    int pos = 0, size = LEN_MAX;
    *string = malloc(sizeof(char) * size);
    // Mientras que el caracter leido sea distinto a '\n'
    while ((c = getchar()) != '\n') {
        (*string)[pos++] = c; // Almacenamos el caracter en string
        if (pos == LEN_MAX) {
            size = pos + LEN_MAX;
            *string = realloc(*string, sizeof(char) * size);
        }
    }
    (*string)[pos] = '\0'; // Colocamos un terminador al final
    return *string; // Devolvemos un puntero al comienzo de string
}

int leer_extension(char *string, int posicion, Set *set) {
    char *resto;
    int numero, correcto = 1, tamanoActual = MAX_INTERVALS, aux = 0, pos = 0;
    string += posicion;
    Interval **intervalArray = malloc(sizeof(Interval*) * tamanoActual);
    while (correcto == 1 && *string != '}') {
        if (isdigit(*string) != 0 || *string == '-') {
            numero = strtol(string, &resto, 10);
            intervalArray[pos++] = interval_crear(numero, numero);
            if (*resto == ',' && *(resto + 1) == ' ' && (*(resto + 2) == '-' || isdigit(*(resto + 2)) != 0))
                string = resto + 2;
            else
                string = resto;
        } else
            correcto = 0;
    }
    if (*(string + 1) != '\0')
        correcto = 0;
    if (correcto == 0) {
        for (; aux < pos; ++aux)
            interval_destruir(&(intervalArray[aux]));
    } else {
        *set = set_crear(pos);
        for (; aux < pos; ++aux)
            set_insertar(set, intervalArray[aux]);
    }
    free(intervalArray);
    return correcto;
}

Interval *leer_comprension(char *string, int posicion, char nombreInterval) {
    char *resto, aux1[10];
    int numero1, numero2;
    string += posicion;
    if (isdigit(*string) != 0 || *string == '-') {
        numero1 = strtol(string, &resto, 10);
    }
    strcpy(aux1, " <= ");
    aux1[4] = nombreInterval;
    aux1[5] = '\0';
    strcat(aux1, " <= ");
    if (strncmp(resto, aux1, 9) == 0 && (isdigit(*(resto + 9)) != 0 || *(resto + 9) == '-')) {
        string = resto + 9;
        numero2 = strtol(string, &resto, 10);
    }
    if (*resto == '}' && *(resto + 1) == '\0') {
        return interval_crear(numero1, numero2);
    }
    return NULL;
}

int obtenerConjuntoDestino(char *string, char *conjunto, int pos) {
    int numero = 0, aux = 1, aux1, numeroAux, loga;
    char *resto;
    conjunto[0] = '\0';
    if (isalpha(*string) != 0) {
        conjunto[0] = *string;
        pos ++;
        string ++;
        for (; *string == '0'; string ++, pos ++) {}
        numero = strtol(string, &resto, 10);
        string = resto;
        if (0 <= numero && numero < pow(10, PROFUNDIDAD_MAXIMA - 1)) {
            if (!numero) {
                for (; aux < PROFUNDIDAD_MAXIMA; aux ++)
                    conjunto[aux] = '0';
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            } else {
                loga = log10(numero);
                loga ++;
                pos += loga;
                for (; aux <= (PROFUNDIDAD_MAXIMA - loga - 1); aux ++)
                    conjunto[aux] = '0';
                numeroAux = numero;
                for (aux = 0; aux < loga; aux ++) {
                    aux1 = (int) (pow(10, loga - aux - 1) + 0.5);
                    conjunto[PROFUNDIDAD_MAXIMA - loga + aux] = (numeroAux / aux1) + '0';
                    numeroAux %= aux1;
                }
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            }
            if (strncmp(string, " = ", 3) != 0)
                conjunto[0] = '\0';
            pos += 3;
        } else
            conjunto[0] = '\0';
    }

    return pos;
}

int obtenerPrimerConjunto(char *string, char *conjunto, int pos) {
    int numero = 0, aux = 1, aux1, numeroAux, loga;
    char *resto;
    conjunto[0] = '\0';
    string += pos;

    if (isalpha(*string) != 0) {
        conjunto[0] = *string;
        pos ++;
        string ++;
        for (; *string == '0'; string ++, pos ++) {}
        numero = strtol(string, &resto, 10);
        string = resto;
        if (0 <= numero && numero < pow(10, PROFUNDIDAD_MAXIMA - 1)) {
            if (!numero) {
                for (; aux < PROFUNDIDAD_MAXIMA; aux ++)
                    conjunto[aux] = '0';
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            } else {
                loga = log10(numero);
                loga ++;
                pos += loga;
                for (; aux < (PROFUNDIDAD_MAXIMA - loga); aux ++)
                    conjunto[aux] = '0';
                numeroAux = numero;
                for (aux = 0; aux < loga; aux ++) {
                    aux1 = (int) (pow(10, loga - aux - 1) + 0.5);
                    conjunto[PROFUNDIDAD_MAXIMA - loga + aux] = (numeroAux / aux1) + '0';
                    numeroAux %= aux1;
                }
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            }
            if (*string != ' ')
                conjunto[0] = '\0';
            pos ++;
        } else
            conjunto[0] = '\0';
    }

    return pos;
}

void obtenerUltimoConjunto(char *string, char *conjunto, int pos) {
    int numero = 0, aux = 1, aux1, numeroAux, loga;
    char *resto;
    conjunto[0] = '\0';
    string += pos;

    if (isalpha(*string) != 0) {
        conjunto[0] = *string;
        string ++;
        for (; *string == '0'; string ++) {}
        numero = strtol(string, &resto, 10);
        string = resto;
        if (0 <= numero && numero < pow(10, PROFUNDIDAD_MAXIMA - 1)) {
            if (!numero) {
                for (; aux < PROFUNDIDAD_MAXIMA; aux ++)
                    conjunto[aux] = '0';
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            } else {
                loga = log10(numero);
                loga ++;
                for (; aux <= (PROFUNDIDAD_MAXIMA - loga - 1); aux ++)
                    conjunto[aux] = '0';
                numeroAux = numero;
                for (aux = 0; aux < loga; aux ++) {
                    aux1 = (int) (pow(10, loga - aux - 1) + 0.5);
                    conjunto[PROFUNDIDAD_MAXIMA - loga + aux] = (numeroAux / aux1) + '0';
                    numeroAux %= aux1;
                }
                conjunto[PROFUNDIDAD_MAXIMA] = '\0';
            }
            if (*string != '\0')
                conjunto[0] = '\0';
        } else
            conjunto[0] = '\0';
    }
}

int main() {
    char *buffer, conjuntoDestino[PROFUNDIDAD_MAXIMA + 1], conjuntoUno[PROFUNDIDAD_MAXIMA + 1], conjuntoDos[PROFUNDIDAD_MAXIMA + 1], operacion;
    Interval *aux;
    Set setDestino = NULL, setUno = NULL, setDos = NULL, setAux = NULL;
    TablaHash *th = tablahash_crear(hash, PROFUNDIDAD_MAXIMA);
    Contenedor *contenedor = NULL;
    int pos, correcto;
    printf("Crear conjunto por extension: 'A = {-2, 5, 7, -9}'\n");
    printf("Crear conjunto por comprension: 'A = {x: -9 <= x <= 42}'\n");
    printf("Unir conjuntos: 'A = A | A1'\n");
    printf("Intersecar conjuntos: 'A = A & A1'\n");
    printf("Restar conjuntos: 'A = A - A1'\n");
    printf("Complemento de conjuntos: 'A = ~A'\n");
    printf("Imprimir conjuntos: 'imprimir A'\n");
    printf("Cerrar el interprete: 'salir'\n");
    while (strcmp(leer_cadena(&buffer), "salir\0")) {
        correcto = 1;
        operacion = ' ';
        // checkear si es imprimir
        if (strncmp(buffer, "imprimir ", 9) == 0) {
            obtenerUltimoConjunto(buffer, conjuntoDestino, 9);
            if (conjuntoDestino[0] != '\0') {
                contenedor = tablahash_buscar(th, conjuntoDestino);
                if (contenedor) {
                    set_imprimir(contenedor_obtener_dato(contenedor));
                    free(contenedor);
                } else
                    printf("No se encontro el conjunto %s", conjuntoDestino);
                printf("\n");
            } else
                correcto = 0;
        // checkear inicio hasta "A = "
        } else {
            pos = obtenerConjuntoDestino(buffer, conjuntoDestino, 0);
            if (conjuntoDestino[0] == '\0')
                correcto = 0;
            else {
                // checkear si es crear conjunto "A = {"
                if (buffer[pos] == '{') {
                    // checkear si es crear por comprension "A = {x: "
                    if (isalpha(buffer[pos + 1]) != 0 && buffer[pos + 2] == ':' && buffer[pos + 3] == ' ') {
                        aux = leer_comprension(buffer, pos + 4, buffer[pos + 1]);
                        if (aux) {
                            if (interval_valido(aux)) {
                                tablahash_eliminar(th, conjuntoDestino, set_destruir);
                                set_insertar(&setDestino, aux);
                                tablahash_insertar(th, conjuntoDestino, setDestino);
                                setDestino = NULL;
                            } else {
                                printf("Intervalo invalido\n");
                            }
                        } else
                            correcto = 0;
                    // en caso contrario debe ser por extension "A = {"
                    } else {
                        if (leer_extension(buffer, pos + 1, &setDestino)) {
                            tablahash_eliminar(th, conjuntoDestino, set_destruir);
                            tablahash_insertar(th, conjuntoDestino, setDestino);
                        }
                        else
                            correcto = 0;
                        setDestino = NULL;
                    }
                // checkear si la operacion es complemento
                } else if (buffer[pos] == '~') {
                    // obtiene el conjunto de la operacion
                    obtenerUltimoConjunto(buffer, conjuntoUno, pos + 1);
                    if (conjuntoUno[0] != '\0')
                        operacion = buffer[pos];
                    else
                        correcto = 0;
                } else {
                    // obtiene el primer conjunto de la operacion
                    pos = obtenerPrimerConjunto(buffer, conjuntoUno, pos);
                    if (conjuntoUno[0] == '\0')
                        correcto = 0;
                    if (correcto == 1) {
                        // chechea si es una operacion
                        if ((buffer[pos] == '|' || buffer[pos] == '&' || buffer[pos] == '-') && buffer[pos + 1] == ' ') {
                            // obtiene el segundo conjunto de la operacion
                            obtenerUltimoConjunto(buffer, conjuntoDos, pos + 2);
                            if (conjuntoDos[0] != '\0')
                                operacion = buffer[pos];
                            else
                                correcto = 0;
                        } else
                            correcto = 0;
                    }
                }
                if (operacion != ' ') {
                    contenedor = tablahash_buscar(th, conjuntoUno);
                    if (contenedor) {
                        setUno = contenedor_obtener_dato(contenedor);
                        free(contenedor);
                    } else
                        printf("No se encontro el conjunto %s\n", conjuntoUno);
                    if (operacion == '~') {
                        if (contenedor) {
                            setAux = set_complemento(setUno);
                            tablahash_eliminar(th, conjuntoDestino, set_destruir);
                            tablahash_insertar(th, conjuntoDestino, setAux);
                        }
                    } else {
                        contenedor = tablahash_buscar(th, conjuntoDos);
                        if (contenedor) {
                            setDos = contenedor_obtener_dato(contenedor);
                            free(contenedor);
                        } else {
                            operacion = ' ';
                            printf("No se encontro el conjunto %s\n", conjuntoDos);
                        }
                        switch (operacion) {
                            case '|':
                                // union de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_unir(setUno, setDos);
                                else
                                    setDestino = set_copia(setUno);
                                tablahash_eliminar(th, conjuntoDestino, set_destruir);
                                tablahash_insertar(th, conjuntoDestino, setDestino);
                                break;
                            case '&':
                                // interseccion de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_intersecar(setUno, setDos);
                                else
                                    setDestino = set_copia(setUno);
                                tablahash_eliminar(th, conjuntoDestino, set_destruir);
                                tablahash_insertar(th, conjuntoDestino, setDestino);
                                break;
                            case '-':
                                // resta de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_restar(setUno, setDos);
                                else
                                    setDestino = set_crear(0);
                                tablahash_eliminar(th, conjuntoDestino, set_destruir);
                                tablahash_insertar(th, conjuntoDestino, setDestino);
                                break;
                        }
                    }
                }
            }
        }
        if (correcto == 0)
            printf("Formato Incorrecto\n");
        free(buffer);
    }
    free(buffer);
    tablahash_destruir_entera(th, set_destruir);
    return 0;
}
