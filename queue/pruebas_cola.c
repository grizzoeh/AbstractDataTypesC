#include "cola.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include "pila.h"
#define PRUEBA_VOLUMEN 10000

static void prueba_cola_vacia(void)
{
    cola_t *cola = cola_crear();
    print_test("se puede crear una cola vacia", cola_esta_vacia(cola));
    print_test("no se puede desencolar una cola vacia", cola_desencolar(cola) == NULL);
    print_test("no se puede ver tope de una pila vacia", cola_ver_primero(cola) == NULL);
    cola_encolar(cola,NULL);
    print_test("se puede encolar el elemento nulo y la cola no estara vacia", cola_esta_vacia(cola) == false);
    cola_desencolar(cola);
    print_test("la cola esta vacia luego de desencolar todos sus elementos", cola_esta_vacia(cola) == true);


    cola_destruir(cola,NULL);
}

static void prueba_invariante(void) {
    cola_t *cola = cola_crear();

    int *int1 = malloc(sizeof(int));
    int int2 = 5;
    int int3 = 4;
   

    cola_encolar(cola, int1);
    
    cola_encolar(cola, &int2);
    cola_encolar(cola, &int3);
    print_test("encola y muestra el primero de la cola correctamente",cola_ver_primero(cola)==int1);
    print_test("desencola correctamente(1)",cola_desencolar(cola)==int1);
    print_test("desencola correctamente(2)",cola_desencolar(cola)==&int2);
    print_test("El primero se actualizo correctamente",cola_ver_primero(cola)==&int3);

    //notar que hago free de int1, ya que en esta prueba no quiero probar destruir cola con punteros si no con ints
    free(int1);
   
    cola_destruir(cola,NULL);
    print_test("la cola con ints fue destruida", true);
  
   
}


static void prueba_volumen(void) {
    cola_t *cola = cola_crear();
    for (int i = 0; i < PRUEBA_VOLUMEN; i++){

        int *entero = malloc(sizeof(int));
        *entero = i;
       
        bool valor_encolar = cola_encolar(cola,entero);
        if (!valor_encolar){
            print_test("prueba de volumen falla al comparar invariante", true);
            break;
        }

    }
    //FIFO
    for (int i = 0; i < PRUEBA_VOLUMEN; i++)
    {
        int *valor = cola_desencolar(cola);
        if (*valor!=i){
            print_test("prueba de volumen falla al comparar invariante", true);
            break;

        }
        free(valor);
    }
    
    print_test("prueba de volumen completada con exito",cola_esta_vacia(cola));

    print_test("no se puede desencolar una cola que se le encolo y desencolo (su estado actual es vacia)",cola_desencolar(cola) == NULL );
    print_test("no se puede ver primero de una cola que se le encolo y desencolo (su estado actual es vacia)",cola_ver_primero(cola) == NULL );
    cola_destruir(cola,NULL);
    //print_test("la pila fue destruida", true);

}

//necesito el wrapper para pila_destruir
void pila_destruir_wrapper(void* elem){
        pila_destruir(elem);
    }

static void prueba_destruir(void) {
    pila_t *pila1 = pila_crear();
    pila_t *pila2 = pila_crear();
    cola_t *cola = cola_crear();
    cola_encolar(cola, pila1);
    cola_encolar(cola, pila2);
    /*
    No se puede invocar directamente a la primitiva pila_destruir con cola_destruir, 
    porque sus firmas son diferentes. Entonces creo una función wrapper 
    que enmascare el comportamiento:
    */
    cola_destruir(cola,pila_destruir_wrapper);
    print_test("la cola con structs (en este caso pilas) fue destruida", true);


    cola_t *cola2 = cola_crear();

    int *punt1 = malloc(sizeof(int));
    int *punt2 = malloc(sizeof(int));
    int *punt3 = malloc(sizeof(int));

    cola_encolar(cola2,punt1);
    cola_encolar(cola2,punt2);
    cola_encolar(cola2,punt3);
    cola_destruir(cola2,free);
    print_test("la cola con punteros fue destruida", true);


}


void pruebas_cola_estudiante()
{
    prueba_cola_vacia();
    prueba_invariante();
    prueba_volumen();
    prueba_destruir();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void)
{
    pruebas_cola_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif
