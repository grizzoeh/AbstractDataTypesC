#include "lista.h"
#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

#define PRUEBA_VOLUMEN 10000

static void pruebas_generales_lista(void)
{
    lista_t *lista = lista_crear();
    print_test("se puede crear una lista vacia", lista_esta_vacia(lista));
    print_test("no se puede borrar un elemento de una lista vacia", lista_borrar_primero(lista) == NULL);
    print_test("no se puede ver primero de una lista vacia", lista_ver_primero(lista) == NULL);
    print_test("no se puede ver el ultimo de una lista vacia", lista_ver_ultimo(lista) == NULL);
    print_test("el largo de una lista vacia es 0", lista_largo(lista) == 0);
    int *int1 = malloc(sizeof(int));
    int *int2 = malloc(sizeof(int));
    *int1 = 3;
    print_test("la lista permite insertar un elemento al principio", lista_insertar_primero(lista, int1) == true);
    print_test("la lista permite insertar un elemento al principio", lista_insertar_ultimo(lista, int2) == true);
    print_test("la lista borra correctamente el primer elemento", lista_borrar_primero(lista) == int1);
    print_test("se actualiza el primero luego de borrar el primero", lista_ver_primero(lista) == int2);
    lista_borrar_primero(lista);
    print_test("luego de borrar todos los elementos de la lista, esta vuelve a estar vacia", lista_esta_vacia(lista));

    free(int1);
    free(int2);
    lista_destruir(lista, NULL);
    print_test("se destruyó la lista correctamente", true);
}

static void prueba_volumen(void)
{
    lista_t *lista = lista_crear();

    for (int i = 0; i < PRUEBA_VOLUMEN; i++)
    {
        int *entero = malloc(sizeof(int));
        *entero = i;
        if (!lista_insertar_ultimo(lista, entero))
            print_test("Fallo al insertar", false);
    }

    //FIFO
    for (int j = 0; j < PRUEBA_VOLUMEN; j++)
    {
        int *valor = lista_borrar_primero(lista);
        if (*valor != j)
        {
            print_test("prueba de volumen falla al comparar invariante", false);
            break;
        }
        free(valor);
    }

    print_test("prueba de volumen completada con exito", lista_esta_vacia(lista));

    lista_destruir(lista, NULL);

}

/* ******************************************************************
 *                    PRUEBAS DEL ITERADOR INTERNO
 * *****************************************************************/

//
// Sumar elementos de una lista con iterador interno
//
bool sumar_todos(void *dato, void *extra)
{
    // Sabemos que ‘extra’ es un entero, por tanto le podemos hacer un cast.
    *(int *)extra += *(int *)dato;
    return true; // seguir iterando
}

void prueba_suma(void)
{
    lista_t *lista = lista_crear();
    int *int1 = malloc(sizeof(int));
    int *int2 = malloc(sizeof(int));
    int *int3 = malloc(sizeof(int));
    *int1 = 3;
    *int2 = 7;
    *int3 = 5;
    lista_insertar_ultimo(lista,int1);
    lista_insertar_ultimo(lista,int2);
    lista_insertar_ultimo(lista,int3);
    int suma = 0;
    lista_iterar(lista, sumar_todos, &suma);
    print_test("la suma total de los elementos de la lista es la correcta (usando iterador interno)", suma==15);
    free(int1);
    free(int2);
    free(int3);
    lista_destruir(lista,NULL);
}

//
// Sumar elementos de una lista con iterador interno con cond corte
//
bool sumar_todos_con_corte(void *dato, void *extra)
{
    // Sabemos que ‘extra’ es un entero, por tanto le podemos hacer un cast.
    *(int *)extra += *(int *)dato;
    if(*(int *)extra>10){
        return false;
    }
    return true; // seguir iterando
}

void prueba_suma_con_corte(void){

    lista_t *lista = lista_crear();
    int *int1 = malloc(sizeof(int));
    int *int2 = malloc(sizeof(int));
    int *int3 = malloc(sizeof(int));
    *int1 = 3;
    *int2 = 8;
    *int3 = 5;
    lista_insertar_ultimo(lista,int1);
    lista_insertar_ultimo(lista,int2);
    lista_insertar_ultimo(lista,int3);
    int suma = 0;
    lista_iterar(lista, sumar_todos_con_corte, &suma);
    print_test("Iterador interno con condicion de corte funciona correctamente: deja de iterar cuando la suma sobrepasa el numero 10", suma==11);
    free(int1);
    free(int2);
    free(int3);
    lista_destruir(lista,NULL);
}

/* ******************************************************************
 *                    PRUEBAS DEL ITERADOR EXTERNO
 * *****************************************************************/
void prueba_recorrer_iter(void)
{
    lista_t* lista= lista_crear();
    
    int valores[7]={1,2,3,4,5,6,7};
    for (size_t i = 0; i < 7; i++){
        lista_insertar_ultimo(lista, &valores[i]);
    }
    lista_iter_t* iter= lista_iter_crear(lista);
    
    int contador = 1;
    
    while (!lista_iter_al_final(iter)) {
       
        if(*(int*)lista_iter_ver_actual(iter)!=contador){
            print_test("El iterador no recorre la lista correctamente", false);
            break;
        }
        
        lista_iter_avanzar(iter);
        contador++;
    }
     
    lista_iter_destruir(iter);
    lista_destruir(lista,NULL);
    print_test("El iterador recorre la lista correctamente", true);
}

void prueba_iterador_externo_lista_vacia(void){

    lista_t* lista= lista_crear();
    lista_iter_t* iter= lista_iter_crear(lista);
    print_test("Lista está vacía", lista_esta_vacia(lista));
    print_test("el actual en la lista vacía devuelve NULL", !lista_iter_ver_actual(iter));
    print_test("no se puede avanzar la lista vacía", !lista_iter_avanzar(iter));
    print_test("no se puede eliminar un elemento de la lista vacía", !lista_iter_borrar(iter));
    print_test("el iterador esta al final de la lista", lista_iter_al_final(iter));

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    print_test("Se destruyen la lista y el terador", true);
    
}

void pruebas_generales_iterador_externo(void){
    lista_t* lista= lista_crear();
    lista_iter_t* iter= lista_iter_crear(lista);
    int valores[7]={1,3,4,5,6,2,32};

    lista_iter_insertar(iter, &valores[0]);
    print_test("Iterador Inserta valores correctamente en la primera posicion luego de ser creado", lista_ver_primero(lista)==&valores[0]);
    print_test("El iterador borra valores correctamente", lista_iter_borrar(iter) == &valores[0]);
    print_test("Se elimina el insertardo luego de crear la lista, quedando el primero NULL", lista_ver_primero(lista)==NULL);
    lista_iter_insertar(iter, &valores[1]);
    lista_iter_insertar(iter, &valores[2]);
    lista_iter_insertar(iter, &valores[3]);
    //avanzo hasta el final
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("Iterador está al final", lista_iter_al_final(iter));
    print_test("Insertar correctamente al final de la lista", lista_iter_insertar(iter, &valores[5]));
    print_test("Cuando el iterador inserta al final efectivamente modifica al ultimo de la lista", lista_iter_ver_actual(iter)==lista_ver_ultimo(lista));
    void* ultimo_antiguo=lista_iter_borrar(iter);
    print_test("Remover el ultimo elemento con iterador cambia el ultimo de la lista", lista_ver_ultimo(lista)!=ultimo_antiguo);

    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    
    
}


void pruebas_destruccion(void){
    lista_t* lista_vacia1= lista_crear();
    lista_destruir(lista_vacia1, NULL);
    print_test("Se destruyó la lista vacia con NULL", true);
    /////
    lista_t* lista_vacia2= lista_crear();
    lista_destruir(lista_vacia2, free);
    print_test("Se destruyó la lista vacia con una funcion", true);
    /////
    lista_t* lista_no_vacia1 = lista_crear();
    int *int1 = malloc(sizeof(int));
    int *int2 = malloc(sizeof(int));
    lista_insertar_ultimo(lista_no_vacia1,int1);
    lista_insertar_ultimo(lista_no_vacia1,int2);
    lista_destruir(lista_no_vacia1, NULL);
    free(int1);
    free(int2);
    print_test("Se destruyó la lista no vacia con NULL", true);
    /////
    lista_t* lista_no_vacia2 = lista_crear();
    int *punt1 = malloc(sizeof(int));
    int *punt2 = malloc(sizeof(int));
    int *punt3 = malloc(sizeof(int));
    lista_insertar_ultimo(lista_no_vacia2,punt1);
    lista_insertar_ultimo(lista_no_vacia2,punt2);
    lista_insertar_ultimo(lista_no_vacia2,punt3);
    lista_destruir(lista_no_vacia2,free);

    print_test("Se destruyó la lista no vacia con una funcion", true);

}




void pruebas_lista_estudiante()
{
    pruebas_generales_lista();
    prueba_volumen();
    prueba_suma();
    prueba_suma_con_corte();
    prueba_iterador_externo_lista_vacia();
    prueba_recorrer_iter();
    pruebas_generales_iterador_externo();
    pruebas_destruccion();
    
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void)
{
    pruebas_lista_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif
