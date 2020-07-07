#include "./ITree/itree.h"
#include "./Interval/interval.h"
//#include "./TablaHash/tablahash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
//A = {2, -3, 5, 7, 3}
//A = {x: 2 <= x <= 4}
int main() {
    char buffer[MAX_LINEA], nombreSet[3], nombreInterval;
    //TablaHash *th = tablahash_crear(26, hash);
    Interval *aux = NULL;
    ITree arbol = NULL;
    buffer[0] = '\0';
    printf("Insertar conjunto por extension: 'A = {-2, 5, 7, -9}'\n");
    printf("Insertar conjunto por compresion: 'A = {x: -9 <= x <= 42}'\n");
    printf("Unir conjuntos: 'A = A | A1'\n");
    printf("Intersecar conjuntos: 'A = A & A1'\n");
    printf("Restar conjuntos: 'A = A - A1'\n");
    printf("Complemento de conjuntos: 'A = ~A'\n");
    printf("Imprimir conjuntos: 'imprimir A'\n");
    printf("Cerrar el interprete: 'salir'\n");
    while (strcmp(leer_cadena(buffer), "salir\0")) {
        if (strlen(buffer) >= 7 && isalpha(buffer[0]) != 0 && buffer[1] == ' ' && buffer[2] == '=' && buffer[3] == ' ' && buffer[4] == '{') {
            nombreSet[0] = buffer[0];
            nombreSet[1] = '\n';
            if (strlen(buffer) >= 20 && isalpha(buffer[5]) != 0 && buffer[6] == ':' && buffer[7] == ' ') {
                nombreInterval = buffer[5];
                aux = leer_compresion(buffer, 8, nombreInterval);
                if (aux) {
                    if (interval_valido(aux)) {
                        arbol = itree_insertar(arbol, aux);
                        itree_imprimir(arbol);
                        printf("\n");
                        itree_destruir(arbol);
                        arbol = itree_crear();
                    } else {
                        interval_destruir(aux);
                        aux = NULL;
                    }
                } else
                    printf("Conjunto mal formado\n");
            } else {
                if (leer_extension(buffer, 5, &arbol) == 1) {
                    itree_imprimir(arbol);
                    printf("\n");
                    itree_destruir(arbol);
                    arbol = itree_crear();
                }
            }
        } else if (strlen(buffer) >= 8 && isalpha(buffer[0]) != 0 && isdigit(buffer[1]) != 0 && buffer[2] == ' ' && buffer[3] == '=' && buffer[4] == ' ' && buffer[5] == '{') {
            nombreSet[0] = buffer[0];
            nombreSet[1] = buffer[1];
            nombreSet[2] = '\n';
            if (strlen(buffer) >= 21 && isalpha(buffer[6]) != 0 && buffer[7] == ':' && buffer[8] == ' ') {
                nombreInterval = buffer[6];
                aux = leer_compresion(buffer, 9, nombreInterval);
                if (aux) {
                    if (interval_valido(aux) == 1) {
                        arbol = itree_insertar(arbol, aux);
                        itree_imprimir(arbol);
                        printf("\n");
                        itree_destruir(arbol);
                        arbol = itree_crear();
                    } else {
                        interval_destruir(aux);
                        aux = NULL;
                    }
                } else
                    printf("Conjunto mal formado\n");
            } else {
                if (leer_extension(buffer, 6, &arbol) == 1) {
                    itree_imprimir(arbol);
                    printf("\n");
                    itree_destruir(arbol);
                    arbol = itree_crear();
                }
            }
        } else
            printf("Conjunto mal formado ultimo\n");
    }
    return 0;
}