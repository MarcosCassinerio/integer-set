#include "./Set/set.h"
#include "./Interval/interval.h"

int main() {
    Interval *interval1 = interval_crear(1, 1);
    Interval *interval2 = interval_crear(3, 3);
    Interval *interval3 = interval_crear(5, 5);
    Interval *interval4 = interval_crear(7, 7);
    Interval *interval5 = interval_crear(3, 7);
    Interval *interval6 = interval_crear(10, 10);

    Set set1 = set_crear();
    Set set2 = set_crear();
    Set set3 = set_crear();
    Set set4 = set_crear();
    Set set5 = set_crear();
    Set set6 = set_crear();

    set1 = set_insertar(set1, interval5);
    set1 = set_insertar(set1, interval6);

    set2 = set_insertar(set2, interval1);
    set2 = set_insertar(set2, interval2);
    set2 = set_insertar(set2, interval3);
    set2 = set_insertar(set2, interval4);

    set3 = set_unir(set1, set2);

    printf("Uno: ");
    set_imprimir(set1);
    printf("\n");
    printf("Dos: ");
    set_imprimir(set2);
    printf("\n");
    printf("Union: ");
    set_imprimir(set3);
    printf("\n");

    set4 = set_intersecar(set1, set2);

    set5 = set_restar(set1, set2);

    set6 = set_complemento(set1);

    printf("Interseccion: ");
    set_imprimir(set4);
    printf("\n");
    printf("Resta: ");
    set_imprimir(set5);
    printf("\n");
    printf("Complemento Uno: ");
    set_imprimir(set6);

    set_destruir(&set1);
    set_destruir(&set2);
    set_destruir(&set3);
    set_destruir(&set4);
    set_destruir(&set5);
    set_destruir(&set6);

    return 0;
}