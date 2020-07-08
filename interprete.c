#include "./ITree/itree.h"
#include "./Interval/interval.h"
//#include "./TablaHash/tablahash.h"
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
//A = {2, -3, 5, 7, 3}
//A = {x: 2 <= x <= 4}
int main() {
    char buffer[MAX_LINEA], conjuntoFinal[3], conjuntoUno[3], conjuntoDos[3], nombreInterval, operacion;
    //TablaHash *th = tablahash_crear(26, hash);
    Interval *aux = NULL;
    ITree arbol = NULL;
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
        if (strlen(buffer) >= 11 && buffer[0] == 'i' && buffer[1] == 'm' && buffer[2] == 'p' && buffer[3] == 'r' && buffer[4] == 'i' && buffer[5] == 'm' && buffer[6] == 'i' && buffer[7] == 'r' && buffer[8] == ' ' && isalpha(buffer[9]) != 0) {
            if (isdigit(buffer[10]) != 0 && buffer[11] == '\0') {
                conjuntoFinal[0] = buffer[9];
                conjuntoFinal[1] = buffer[10];
                conjuntoFinal[2] = '\0';
            } else if(buffer[10] == '\0') {
                conjuntoFinal[0] = buffer[9];
                conjuntoFinal[1] = '\0';
            } else
                correcto = 0;
            if (correcto == 1)
                printf("imprimir conjunto %s\n", conjuntoFinal);
        // checkear inicio hasta "A = "
        } else if (strlen(buffer) >= 7 && isalpha(buffer[0]) != 0) {
            // guarda conjunto final
            if (isdigit(buffer[1]) != 0 && buffer[2] == ' ' && buffer[3] == '=' && buffer[4] == ' ') {
                conjuntoFinal[0] = buffer[0];
                conjuntoFinal[1] = buffer[1];
                conjuntoFinal[2] = '\0';
                pos = 5;
            } else if (buffer[1] == ' ' && buffer[2] == '=' && buffer[3] == ' ') {
                conjuntoFinal[0] = buffer[0];
                conjuntoFinal[1] = '\0';
                pos = 4;
            } else
                correcto = 0;
            // si correcto hasta ahora
            if (correcto == 1) {
                // checkear si es crear conjunto "A = {"
                if (buffer[pos] == '{') {
                    // checkear si es crear por compresion "A = {x: "
                    if (isalpha(buffer[pos + 1]) != 0 && buffer[pos + 2] == ':' && buffer[pos + 3] == ' ') {
                        printf("Creo conjunto por compresion\n");
                        aux = leer_compresion(buffer, pos + 4, buffer[pos + 1]);
                        if (aux) {
                            if (interval_valido(aux)) {
                                arbol = itree_insertar(arbol, aux);
                                itree_imprimir(arbol); // insertar en tablahash
                                printf("\n"); // eventualmente borrar
                                // eventualmente borrar creo
                                itree_destruir(arbol);
                                arbol = itree_crear();
                                // hasta aca
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
                            itree_imprimir(arbol);
                            printf("\n");
                            itree_destruir(arbol);
                            arbol = itree_crear();
                        } else
                            correcto = 0;
                    }
                // checkear si complemento
                } else if (isalpha(buffer[pos]) != 0) {
                    // guardo conjuntoUno
                    if (isdigit(buffer[pos + 1]) != 0 && buffer[pos + 2] == ' ') {
                        conjuntoUno[0] = buffer[pos];
                        conjuntoUno[1] = buffer[pos + 1];
                        conjuntoUno[2] = '\0';
                        pos += 3;
                    } else if (buffer[pos + 1] == ' ') {
                        conjuntoUno[0] = buffer[pos];
                        conjuntoUno[1] = '\0';
                        pos += 2;
                    } else
                        correcto = 0;
                    if (correcto == 1) {
                        // chechear si es una de las operaciones | & -
                        if ((buffer[pos] == '|' || buffer[pos] == '&' || buffer[pos] == '-') && buffer[pos + 1] == ' ') {
                            if (isalpha(buffer[pos + 2]) != 0) {
                                // guardo conjuntoDos y operacion
                                if (isdigit(buffer[pos + 3]) != 0 && buffer[pos + 4] == '\0') {
                                    conjuntoDos[0] = buffer[pos + 2];
                                    conjuntoDos[1] = buffer[pos + 3];
                                    conjuntoDos[2] = '\0';
                                    operacion = buffer[pos];
                                } else if (buffer[pos + 3] == '\0') {
                                    conjuntoDos[0] = buffer[pos + 2];
                                    conjuntoDos[1] = '\0';
                                    operacion = buffer[pos];
                                } else
                                    correcto = 0;
                            } else
                                correcto = 0;
                        } else
                            correcto = 0;
                    }
                } else if (buffer[pos] == '~') {
                    if (isalpha(buffer[pos + 1]) != 0) {
                        // guardo conjuntoUno y operacion
                        if (isdigit(buffer[pos + 2]) != 0 && buffer[pos + 3] == '\0') {
                            conjuntoUno[0] = buffer[pos + 1];
                            conjuntoUno[1] = buffer[pos + 2];
                            conjuntoUno[2] = '\0';
                            operacion = buffer[pos];
                        } else if (buffer[pos + 2] == '\0') {
                            conjuntoUno[0] = buffer[pos + 1];
                            conjuntoUno[1] = '\0';
                            operacion = buffer[pos];
                        } else
                            correcto = 0;
                    } else
                        correcto = 0;
                // checkeo si union, interseccion o resta "A = B "
                } else
                    correcto = 0;
                switch (operacion) {
                    case '|':
                        // union de conjuntos
                        printf("Union %s, %s, %s\n", conjuntoFinal, conjuntoUno, conjuntoDos);
                        break;
                    case '&':
                        // interseccion de conjuntos
                        printf("Interseccion %s, %s, %s\n", conjuntoFinal, conjuntoUno, conjuntoDos);
                        break;
                    case '-':
                        // resta de conjuntos
                        printf("Resta %s, %s, %s\n", conjuntoFinal, conjuntoUno, conjuntoDos);
                        break;
                    default:
                        printf("defecto\n");
                        break;
                }
            }
        }
        if (correcto == 0)
            printf("Sos pelotudo\n");
    }
    return 0;
}