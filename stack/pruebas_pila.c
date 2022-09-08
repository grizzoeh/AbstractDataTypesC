#include "pila.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#define PRUEBA_VOLUMEN 10000



static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();
    print_test("se puede crear una pila vacia",pila_esta_vacia(pila) == true );
    print_test("no se puede desapilar un pila vacia",pila_desapilar(pila) == NULL );
    print_test("no se puede ver tope de una pila vacia",pila_ver_tope(pila) == NULL );

    pila_destruir(pila);
}

static void prueba_invariante(void) {
    pila_t *pila = pila_crear();
   
    char str1[] = "hola";
    char str2[] = "chau";
    char str3[] = "ciao";
    pila_apilar(pila, str1);
    print_test("pila apila correctamente",!pila_esta_vacia(pila));
    pila_apilar(pila, str2);
    pila_apilar(pila, str3);
    print_test("el que desapilo es el ultimo que entro (prueba con strs)",pila_desapilar(pila)==str3);
    
    pila_apilar(pila,NULL);
    print_test("permite apilar el elemento nulo",pila_ver_tope(pila)==NULL);
    print_test("pila apila elemento nulo, pero no esta vacia",!pila_esta_vacia(pila));

    //print_test("pila apila NULL y no esta vacia",!pila_esta_vacia(pila));
    //print_test("desapilo null",pila_desapilar(pila)==NULL);

    
    int *numero = malloc(sizeof(int));
    int numero2 = 5;
    
    
    pila_apilar(pila,numero);
    
    print_test("el que desapilo es el ultimo que entro (prueba con int)",pila_desapilar(pila)==numero);
    free(numero);
    
    pila_apilar(pila, &numero2);
    print_test("el que desapilo es el ultimo que entro (segunda prueba con int )",pila_desapilar(pila)==&numero2);
    pila_destruir(pila);
   
    
    
}

static void prueba_volumen(void) {
    pila_t *pila = pila_crear();
    for (int i = 0; i < PRUEBA_VOLUMEN; i++)
    {
        int *entero = malloc(sizeof(int));
        *entero = i;
        pila_apilar(pila,entero);
    }

    for (int i = PRUEBA_VOLUMEN-1; i > -1; i--)
    {
        int *valor = pila_desapilar(pila);
        if (*valor!=i){
            print_test("prueba de volumen falla al comparar invariante", false);
            break;

        }
        free(valor);
    }
    
    print_test("prueba de volumen completada con exito",pila_esta_vacia(pila));

    print_test("no se puede desapilar un pila que se le apilo y desapilo (su estado actual es vacia)",pila_desapilar(pila) == NULL );
    print_test("no se puede ver tope de una pila que se le apilo y desapilo (su estado actual es vacia)",pila_ver_tope(pila) == NULL );
    pila_destruir(pila);
    print_test("la pila fue destruida", true);
    

}



void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_invariante();
    prueba_volumen();
    
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
