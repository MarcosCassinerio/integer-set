#include "./ITree/itree.h"
#include "./Interval/interval.h"
#include "./TablaHash/tablahash.h"
#include <ctype.h>
#include <stdint.h>
#define MAX_LINEA 100

unsigned hash(void* clave) {
    int* p = clave;
    return *p;
}

/*
    leer_cadena: char* -> char*
    Toma lo escrito por el teclado, y lo retorna reemplazando el salto de linea
    por un '\0'.
*/
char *leer_cadena(char *string) {
    char c, *aux = string;
    // Mientras que el caracter leido sea distinto a '\n'
    while ((c = getchar()) != '\n') {
        if (c != '\r') {            // Si el caracter leido es distinto a '\r'
            *string = c;              // Almacenamos el caracter en string
            ++string;                 // Movemos la posicion a la que apunta string
        }
    }
    *string = '\0';               // Colocamos un terminador al final
    return aux;                   // Devolvemos un puntero al comienzo de string
}

int leer_extension(char *string, int posicion, ITree *arbol) {
    char *resto;
    int numero, correcto = 1;
    Interval *aux = NULL;
    string += posicion;
    while (correcto == 1 && *string != '}') {
        if (isdigit(*string) != 0 || *string == '-') {
            numero = strtol(string, &resto, 10);
            *arbol = itree_insertar(*arbol, interval_crear(numero, numero));
            if (*resto == ',' && *(resto + 1) == ' ')
                string = resto + 2;
            else
                string = resto;
        } else
            correcto = 0;
    }
    if (*(string + 1) != '\0')
        correcto = 0;
    if (correcto == 0)
        itree_destruir(*arbol);
    return correcto;
}

Interval *leer_compresion(char *string, int posicion, char nombreInterval) {
    char *resto;
    int numero1, numero2;
    string += posicion;
    if (isdigit(*string) != 0 || *string == '-') {
        numero1 = strtol(string, &resto, 10);
    }
    if (*resto == ' ' && *(resto + 1) == '<' && *(resto + 2) == '=' && *(resto + 3) == ' ' && *(resto + 4) == nombreInterval && *(resto + 5) == ' ' && *(resto + 6) == '<' && *(resto + 7) == '=' && *(resto + 8) == ' ' && (isdigit(*(resto + 9)) != 0 || *(resto + 9) == '-')) {
        string = resto + 9;
        numero2 = strtol(string, &resto, 10);
    }
    if (*resto == '}' && *(resto + 1) == '\0') {
        return interval_crear(numero1, numero2);
    }
    return NULL;
}

int obtenerConjuntoDestino(char *string, char *conjunto, int pos) {
    conjunto[0] = '\0';
    if (isalpha(*string) != 0) {
        if (isdigit(*(string + 1)) != 0 && *(string + 2) == ' ' && *(string + 3) == '=' && *(string + 4) == ' ') {
            conjunto[0] = *string;
            conjunto[1] = *(string + 1);
            conjunto[2] = '\0';
            pos = 5;
        } else if (*(string + 1) == ' ' && *(string + 2) == '=' && *(string + 3) == ' ') {
            conjunto[0] = *string;
            conjunto[1] = '\0';
            pos = 4;
        }
    }
    return pos;
}

int obtenerPrimerConjunto(char *string, char *conjunto, int pos) {
    conjunto[0] = '\0';
    if (isalpha(*(string + pos)) != 0) {
        if (isdigit(*(string + pos + 1)) != 0 && *(string + pos + 2) == ' ') {
            conjunto[0] = *(string + pos);
            conjunto[1] = *(string + pos + 1);
            conjunto[2] = '\0';
            pos += 3;
        } else if (*(string + pos + 1) == ' ') {
            conjunto[0] = *(string + pos);
            conjunto[1] = '\0';
            pos += 2;
        }
    }
    return pos;
}

void obtenerUltimoConjunto(char *string, char *conjunto, int pos) {
    conjunto[0] = '\0';
    if (isalpha(*(string + pos)) != 0) {
        if (isdigit(*(string + pos + 1)) != 0 && *(string + pos + 2) == '\0') {
            conjunto[0] = *(string + pos);
            conjunto[1] = *(string + pos + 1);
            conjunto[2] = '\0';
        } else if (*(string + pos + 1) == '\0') {
            conjunto[0] = *(string + pos);
            conjunto[1] = '\0';
        }
    }
}

int main() {
    char buffer[MAX_LINEA], conjuntoDestino[3], conjuntoUno[3], conjuntoDos[3], nombreInterval, operacion;
    //TablaHash *th = tablahash_crear(26, hash);
    Interval *aux = NULL;
    ITree arbol = NULL;
    TablaHash *th = tablahash_crear(260, hash); 
    int pos, correcto;
    buffer[0] = '\0';
    printf("Crear conjunto por extension: 'A = {-2, 5, 7, -9}'\n");
    printf("Crear conjunto por compresion: 'A = {x: -9 <= x <= 42}'\n");
    printf("Unir conjuntos: 'A = A | A1'\n");
    printf("Intersecar conjuntos: 'A = A & A1'\n");
    printf("Restar conjuntos: 'A = A - A1'\n");
    printf("Complemento de conjuntos: 'A = ~A'\n");
    printf("Imprimir conjuntos: 'imprimir A'\n");
    printf("Cerrar el interprete: 'salir'\n");
    while (strcmp(leer_cadena(buffer), "salir\0")) {
        correcto = 1;
        operacion = ' ';
        // checkear si es imprimir
        if (buffer[0] == 'i' && buffer[1] == 'm' && buffer[2] == 'p' && buffer[3] == 'r' && buffer[4] == 'i' && buffer[5] == 'm' && buffer[6] == 'i' && buffer[7] == 'r' && buffer[8] == ' ') {
            obtenerUltimoConjunto(buffer, conjuntoDestino, 9);
            if (conjuntoDestino[0] != '\0') {
                printf("imprimir conjunto %s\n", conjuntoDestino);
                arbol = tablahash_buscar(th, &conjuntoDestino[0]);
                printf("arbol");
                itree_imprimir(arbol);
                printf(";\n");
            } else
                correcto = 0;
        // checkear inicio hasta "A = "
        } else {
            pos = obtenerConjuntoDestino(buffer, conjuntoDestino, pos);
            if (conjuntoDestino[0] == '\0')
                correcto = 0;
            else {
                // checkear si es crear conjunto "A = {"
                if (buffer[pos] == '{') {
                    // checkear si es crear por compresion "A = {x: "
                    if (isalpha(buffer[pos + 1]) != 0 && buffer[pos + 2] == ':' && buffer[pos + 3] == ' ') {
                        printf("Creo conjunto por compresion\n");
                        aux = leer_compresion(buffer, pos + 4, buffer[pos + 1]);
                        if (aux) {
                            if (interval_valido(aux)) {
                                arbol = itree_insertar(arbol, aux);
                                tablahash_insertar(th, &conjuntoDestino[0], arbol);
                            } else {
                                // eventualmente borrar creo
                                interval_destruir(aux);
                                aux = NULL;
                                // hasta aca
                            }
                        } else
                            correcto = 0;
                    // en caso contrario debe ser por extension "A = {"
                    } else {
                        printf("Creo conjunto por extension\n");
                        if (leer_extension(buffer, pos + 1, &arbol) == 1) {
                            tablahash_insertar(th, &conjuntoDestino[0], arbol);
                        } else
                            correcto = 0;
                    }
                // checkear si la operacion es complemento
                } else if (buffer[pos] == '~') {
                    // obtiene el conjunto de la operacion
                    obtenerUltimoConjunto(buffer, conjuntoDos, pos + 1);
                    if (conjuntoDos[0] != '\0')
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
                    if (conjuntoUno[0] != '\0') {
                    } else
                        correcto = 0;
                }
                switch (operacion) {
                    case '|':
                        // union de conjuntos
                        printf("Union %s, %s, %s\n", conjuntoDestino, conjuntoUno, conjuntoDos);
                        break;
                    case '&':
                        // interseccion de conjuntos
                        printf("Interseccion %s, %s, %s\n", conjuntoDestino, conjuntoUno, conjuntoDos);
                        break;
                    case '-':
                        // resta de conjuntos
                        printf("Resta %s, %s, %s\n", conjuntoDestino, conjuntoUno, conjuntoDos);
                        break;
                    case '~':
                        // complemento de conjunto
                        printf("Complemento %s, %s\n", conjuntoDestino, conjuntoDos);
                        break;
                    default:
                        break;
                }
            }
        }
        if (correcto == 0)
            printf("Sos pelotudo\n");
    }
    return 0;
}