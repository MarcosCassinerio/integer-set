#include "./Interval/interval.h"
#include "./Set/set.h"
#include "./TablaHash/tablahash.h"
#include <stdint.h>
#include <math.h>
#define LEN_MAX 100
#define MAX_INTERVALS 50
#define TAMANO_TH 100
#define LEN_SET 5

int hash(char *clave) {
    int hash = 0;
    for (; *clave != '\0'; clave ++)
        hash += *clave;
    return hash;
}

/*
    leer_cadena: char* -> char*
    Toma lo escrito por el teclado, y lo retorna reemplazando el salto de linea
    por un '\0'.
*/
char *leer_cadena(char **string) {
    char c;
    int pos = 0, size = LEN_MAX;
    *string = malloc(sizeof(char) * size); // Pedimos memoria para string
    // Mientras que el caracter leido sea distinto a '\n'
    while ((c = getchar()) != '\n') {
        (*string)[pos++] = c; // Almacenamos el caracter en string
        // Si el string se llena le damos mas memoria
        if (pos == LEN_MAX) {
            size = pos + LEN_MAX;
            *string = realloc(*string, sizeof(char) * size);
        }
    }
    (*string)[pos] = '\0'; // Colocamos un terminador al final
    return *string; // Devolvemos un puntero al comienzo de string
}

/*
  leer_extension: char* int set* -> int
  Lee todos los numeros en el string y los guarda como intervalos en el 
  conjunto dado si es que el string cumplio el formato dado y devuelve si hubo 
  errores en la lectura.
*/
int leer_extension(char *string, int posicion, Set *set) {
    char *resto;
    int numero, correcto = 1, tamanoActual = MAX_INTERVALS, aux = 0, pos = 0;
    string += posicion;
    // Inicializa el array de intervalos con el tamano dado
    Interval **intervalArray = malloc(sizeof(Interval*) * tamanoActual);
    while (correcto == 1 && *string != '}') {
        // Si el caracter actual es un numero o un menos
        if (isdigit(*string) != 0 || *string == '-') {
            // Guarda el numero leido
            numero = strtol(string, &resto, 10);
            // Agrega el intervalo nuevo al array
            intervalArray[pos++] = interval_crear(numero, numero);
            // Si el siguiente string cumple el formato dado
            if (*resto == ',' && (*(resto + 1) == '-' || isdigit(*(resto + 1)) != 0))
                string = resto + 1;
            else
                string = resto;
        } else
            correcto = 0;
    }
    if (*(string + 1) != '\0') // Si el caracter siguiente es el terminador
        correcto = 0;
    if (correcto == 0) { // Si ocurrio algun error leyendo la cadena
        // Destruye los intervalos del array
        for (; aux < pos; ++aux)
            interval_destruir(&(intervalArray[aux]));
    } else {
        // Crea un nuevo conjunto con los datos del array en el
        *set = set_crear(pos);
        for (; aux < pos; ++aux)
            set_insertar(set, intervalArray[aux]);
    }
    // Libera la memoria del array
    free(intervalArray);
    return correcto; // Retorna si hubo errores en la lectura
}


/*
  leer_comprension: char* int char -> Interval*
  Lee el extremo izquierdo y derecho de un intervalo en el string y devuelve un 
  nuevo intervalos con dichos extremos en el si es que no hubo errores en la
  lectura, en caso contrario retorna NULL.
*/
Interval *leer_comprension(char *string, int posicion, char nombreInterval) {
    char *resto, aux1[10];
    int numero1, numero2;
    string += posicion;
    // Si el string cumple el formato dado
    if (isdigit(*string) != 0 || *string == '-')
        // Guarda el numero1
        numero1 = strtol(string, &resto, 10);
    strcpy(aux1, " <= ");
    aux1[4] = nombreInterval;
    aux1[5] = '\0';
    strcat(aux1, " <= ");
    // Si el string cumple el formato dado
    if (strncmp(resto, aux1, 9) == 0 && (isdigit(*(resto + 9)) != 0 || *(resto + 9) == '-')) {
        string = resto + 9;
        // Guarda el numero2
        numero2 = strtol(string, &resto, 10);
    }
    // Si el string
    if (*resto == '}' && *(resto + 1) == '\0')
        // Retorna un intervalo nuevo con los 2 numeros
        return interval_crear(numero1, numero2);
    return NULL; // Retorna NULL
}

/*
  obtenerConjuntoDestino: char* char** -> int
  Dado un string guarda en la variable conjunto el substring leido si cumplio 
  cierto formato la lectura y retorna la posicion del ultimo char que leyo.
*/
int obtenerConjuntoDestino(char *string, char **conjunto) {
    int tamano = LEN_SET, pos = 0;
    *conjunto = malloc(sizeof(char) * tamano); // Pide memoria para conjunto

    for (; *string != '\0' && *string != ' '; string ++, pos ++) {
        (*conjunto)[pos++] = *string; // Guarda en conjunto el char actual
        // Si el string se llena le damos mas memoria
        if (pos == tamano) {
            tamano += LEN_SET;
            *conjunto = realloc(*conjunto, sizeof(char) * tamano);
        }
    }

    // Si string cumple el formato dado
    if (strncmp(string, " = ", 3) == 0) {
        // Guarda en la siguiente posicion de conjunto el terminador
        (*conjunto)[pos] = '\0';
        pos += 3;
    } else {
        // Hace que conjunto sea solo el terminador
        *conjunto = realloc(*conjunto, sizeof(char));
        (*conjunto)[0] = '\0';
    }

    return pos; // Retorna la ultima posicion leida
}

/*
  obtenerPrimerConjunto: char* char** int -> int
  Dado un string guarda en la variable conjunto el substring leido si cumplio 
  cierto formato la lectura y retorna la posicion del ultimo char que leyo.
*/
int obtenerPrimerConjunto(char *string, char **conjunto, int pos) {
    int tamano = LEN_SET, posicion = 0;
    *conjunto = malloc(sizeof(char) * tamano); // Pide memoria para conjunto
    string += pos;

    for (; *string != '\0' && *string != ' '; string ++, pos ++) {
        (*conjunto)[posicion++] = *string; // Guarda en conjunto el char actual
        // Si el string se llena le damos mas memoria
        if (posicion == tamano) {
            tamano += LEN_SET;
            *conjunto = realloc(*conjunto, sizeof(char) * tamano);
        }
    }

    // Si string cumple el formato dado
    if (*string == ' ') {
        // Guarda en la siguiente posicion de conjunto el terminador
        (*conjunto)[posicion] = '\0';
        pos ++;
    } else {
        // Hace que conjunto sea solo el terminador
        *conjunto = realloc(*conjunto, sizeof(char));
        (*conjunto)[0] = '\0';
    }

    return pos; // Retorna la ultima posicion leida
}

/*
  obtenerUltimoConjunto: char* char** int -> void
  Dado un string guarda en la variable conjunto el substring leido si cumplio 
  cierto formato la lectura.
*/
void obtenerUltimoConjunto(char *string, char **conjunto, int pos) {
    int tamano = LEN_SET, posicion = 0;
    *conjunto = malloc(sizeof(char) * tamano); // Pide memoria para conjunto
    string += pos;

    for (; *string != '\0' && *string != ' '; string ++, pos ++) {
        (*conjunto)[posicion++] = *string; // Guarda en conjunto el char actual
        // Si el string se llena le damos mas memoria
        if (posicion == tamano) {
            tamano += LEN_SET;
            *conjunto = realloc(*conjunto, sizeof(char) * tamano);
        }
    }

    // Si string cumple el formato dado
    if (*string == '\0')
        // Guarda en la siguiente posicion de conjunto el terminador
        (*conjunto)[posicion] = '\0';
    else {
        // Hace que conjunto sea solo el terminador
        *conjunto = realloc(*conjunto, sizeof(char));
        (*conjunto)[0] = '\0';
    }
}

int main() {
    char *buffer, *conjuntoDestino, *conjuntoUno, *conjuntoDos, operacion;
    Interval *aux;
    Set setDestino = NULL, setUno = NULL, setDos = NULL, setAux = NULL;
    // Crea la tablahash con la funcionhash y tamano dado
    TablaHash *th = tablahash_crear(hash, TAMANO_TH);
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
    // Mientras que buffer sea distino de "salir"
    while (strcmp(leer_cadena(&buffer), "salir\0")) {
        correcto = 1;
        operacion = ' ';
        setDestino = NULL;
        aux = NULL;
        // Si buffer es igual a "imprimir"
        if (strncmp(buffer, "imprimir ", 9) == 0) {
            // Lee el nombre del conjunto y lo guarda en conjuntoDestino
            obtenerUltimoConjunto(buffer, &conjuntoDestino, 9);
            // Si se leyo correctamente buffer
            if (conjuntoDestino[0] != '\0') {
                // Guarda el dato obtenido en contenedor si es que existe
                contenedor = tablahash_buscar(th, conjuntoDestino);
                // Si contenedor no es nulo
                if (contenedor) {
                    // Imprime el dato de contenedor y lo libera la memoria de contenedor
                    set_imprimir(contenedor_obtener_dato(contenedor));
                    free(contenedor);
                } else
                    // Imprime que no fue encontrado el conjunto
                    printf("No se encontro el conjunto %s", conjuntoDestino);
                printf("\n");
            } else
                correcto = 0;
            // Libera la memoria de conjuntoDestino
            free(conjuntoDestino);
        // checkear inicio hasta "A = "
        } else {
            // Lee el nombre del conjunto y lo guarda en conjuntoDestino
            pos = obtenerConjuntoDestino(buffer, &conjuntoDestino, 0);
            // Si no se leyo correctamente buffer
            if (conjuntoDestino[0] == '\0')
                correcto = 0;
            else {
                // Si el caracter actual de buffer es '{'
                if (buffer[pos] == '{') {
                    // Si buffer cumple cierto formato
                    if (isalpha(buffer[pos + 1]) != 0 && buffer[pos + 2] == ' ' && buffer[pos + 3] == ':' && buffer[pos + 4] == ' ') {
                        // Guarda en aux el intervalo leido de buffer
                        aux = leer_comprension(buffer, pos + 5, buffer[pos + 1]);
                        if (aux) { // Si aux no es nulo
                            if (interval_valido(aux)) { // Si el intervalo es valido
                                // Inserta aux en setDestino y lo guarda en la tablahash
                                set_insertar(&setDestino, aux);
                                tablahash_insertar(th, conjuntoDestino, setDestino, set_destruir);
                            } else
                                printf("Intervalo invalido\n");
                        } else
                            correcto = 0;
                    // en caso contrario debe ser por extension "A = {"
                    } else {
                        // Si buffer cumple cierto formato guarda en setDestino todos los intervalos en el
                        if (leer_extension(buffer, pos + 1, &setDestino)) {
                            // Inserta setDestino en la tablahash
                            tablahash_insertar(th, conjuntoDestino, setDestino, set_destruir);
                        }
                        else
                            correcto = 0;
                    }
                // Si buffer cumple cierto formato
                } else if (strncmp(buffer, "~ ", 2) == 0 ) {
                    // Lee el nombre del conjunto y lo guarda en conjuntoUno
                    obtenerUltimoConjunto(buffer, &conjuntoUno, pos + 2);
                    // Si se leyo correctamente buffer
                    if (conjuntoUno[0] != '\0')
                        // Guarda la posicion actual de buffer en operacion
                        operacion = buffer[pos];
                    else
                        correcto = 0;
                } else {
                    // Lee el nombre del conjunto y lo guarda en conjuntoUno
                    pos = obtenerPrimerConjunto(buffer, &conjuntoUno, pos);
                    // Si se leyo correctamente buffer
                    if (conjuntoUno[0] != '\0') {
                        // Si buffer cumple cierto formato
                        if ((buffer[pos] == '|' || buffer[pos] == '&' || buffer[pos] == '-') && buffer[pos + 1] == ' ') {
                            // Lee el nombre del conjunto y lo guarda en conjuntoDos
                            obtenerUltimoConjunto(buffer, &conjuntoDos, pos + 2);
                            // Si se leyo correctamente buffer
                            if (conjuntoDos[0] != '\0')
                                // Guarda la posicion actual de buffer en operacion
                                operacion = buffer[pos];
                            else {
                                correcto = 0;
                                // En caso contrario libera la memoria de conjuntoUno y conjuntoDos
                                free(conjuntoUno);
                                free(conjuntoDos);
                            }
                        } else {
                            correcto = 0;
                            // En caso contrario libera la memoria de conjuntoUno
                            free(conjuntoUno);
                        }
                    } else
                        correcto = 0;
                }
                // Si la operaciones diferente de un espacio
                if (operacion != ' ') {
                    // Guarda el dato obtenido en contenedor si es que existe
                    contenedor = tablahash_buscar(th, conjuntoUno);
                    if (contenedor) { // Si contenedor no es nulo
                        // Guarda el dato de contenedor en setUno
                        setUno = contenedor_obtener_dato(contenedor);
                        free(contenedor); // Libera la memoria de contenedor
                    } else {
                        operacion = ' ';
                        // Imprime que no fue encontrado el conjunto
                        printf("No se encontro el conjunto %s\n", conjuntoUno);
                    }
                    free(conjuntoUno); // Libera la memoria de conjuntoUno
                    // Si la operacion es '~'
                    if (operacion == '~') {
                        // Guarda en setAux el complemento de setUno y lo inserta en la tablahash
                        setAux = set_complemento(setUno);
                        tablahash_insertar(th, conjuntoDestino, setAux, set_destruir);
                    } else {
                        // Guarda el dato obtenido en contenedor si es que existe
                        contenedor = tablahash_buscar(th, conjuntoDos);
                        if (contenedor) { // Si contenedor no es nulo
                            // Guarda el dato de contenedor en setUno
                            setDos = contenedor_obtener_dato(contenedor);
                            free(contenedor); // Libera la memoria de contenedor
                        } else {
                            operacion = ' ';
                            // Imprime que no fue encontrado el conjunto
                            printf("No se encontro el conjunto %s\n", conjuntoDos);
                        }
                        free(conjuntoDos); // Libera la memoria de conjuntoUno
                        switch (operacion) { // Chequea cual es la operacion
                            case '|':
                                // Union de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_unir(setUno, setDos);
                                else
                                    setDestino = set_copia(setUno);
                                // Inserta en la tablahash la union de los conjuntos
                                tablahash_insertar(th, conjuntoDestino, setDestino, set_destruir);
                                break;
                            case '&':
                                // Interseccion de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_intersecar(setUno, setDos);
                                else
                                    setDestino = set_copia(setUno);
                                // Inserta en la tablahash la interseccion de los conjuntos
                                tablahash_insertar(th, conjuntoDestino, setDestino, set_destruir);
                                break;
                            case '-':
                                // Resta de conjuntos
                                if (setUno != setDos)
                                    setDestino = set_restar(setUno, setDos);
                                else
                                    setDestino = set_crear(0);
                                // Inserta en la tablahash la diferencia de los conjuntos
                                tablahash_insertar(th, conjuntoDestino, setDestino, set_destruir);
                                break;
                        }
                    }
                }
            }
            free(conjuntoDestino); // Libera la memoria de conjuntoDestino
        }
        if (correcto == 0)
            printf("Formato Incorrecto\n");
        free(buffer); // Libera la memoria de buffer
    }
    free(buffer); // Libera la memoria de buffer
    // Destruye la tablahash entera
    tablahash_destruir_entera(th, set_destruir);
    return 0;
}
