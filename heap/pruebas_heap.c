#include "heap.h" 
#include "testing.h"
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <stdio.h>

/*
 *Esta funcion compara dos valores transformandolos a enteros y devolviendo
 * otro entero que serà
 *   menor a 0  si  a < b
 *       0      si  a == b
 *   mayor a 0  si  a > b
 */
int comparacion(const void * a, const void * b){
	if (a <  b){
		return -1;
	}
	if (a >  b){
		return 1;
	}
	return 0;
}



static void pruebas_creado(){
	heap_t * heap = heap_crear(comparacion);
    print_test("Prueba crear heap vacio", heap);
	print_test("Prueba cantidad cero", heap_cantidad(heap) == 0);
	print_test("Prueba es un heap vacio", heap_esta_vacio(heap));

	print_test("No se puede desencolar un heap vacio", !heap_desencolar(heap));
	heap_destruir(heap, NULL);

}

static void prueba_crear_sin_funcion_comparacion(){
	heap_t* heap = heap_crear( NULL);
    print_test("No se puede crear un heap sin funcion de comparacion", !heap);
}


static void prueba_encolado_desencolado_un_elemento(){
	heap_t * heap = heap_crear(comparacion);
   	int *elemento = malloc(sizeof(int*));
	
	print_test("Insertar un elemento", heap_encolar(heap, elemento));
	print_test("Prueba cantidad correcta", heap_cantidad(heap) == 1);
	print_test("Prueba NO es un heap vacio", !heap_esta_vacio(heap));
	void * elemento_desencolado =  heap_desencolar(heap);
	print_test("Desencolado correcto", (elemento == elemento_desencolado));
	print_test("Prueba cantidad cero", heap_cantidad(heap) == 0);
	heap_destruir(heap, NULL);
	free(elemento);
}
/*
 * A diferencia de la anterior, esta funcion prueba que se libere la memoria
 * Cuando se destruye un heap con elementos dentro
 */
static void prueba_encolado_un_elemento(){
	heap_t * heap = heap_crear(comparacion);
   	int *elemento = malloc(sizeof(int*));
	
	print_test("Insertar un elemento", heap_encolar(heap, elemento));
	print_test("Prueba cantidad correcta", heap_cantidad(heap) == 1);
	print_test("Prueba NO es un heap vacio", !heap_esta_vacio(heap));
	heap_destruir(heap, free);
}

static void prueba_reemplazo_mayor(){

	char* primero = "aaa";
	char* segundo = "bbb";

	heap_t * heap = heap_crear(comparacion);
	heap_encolar(heap, primero);
	print_test("Prueba cantidad correcta", heap_cantidad(heap) == 1);
	void * desen = heap_ver_max(heap);
	print_test("Maximo correcto",(strcmp(primero, (char*)desen) == 0));
	print_test("Insertar otro elemento", heap_encolar(heap, segundo));
	desen = heap_ver_max(heap);
	print_test("Maximo correcto",(strcmp(segundo, (char*)desen) == 0));
	heap_destruir(heap, NULL);
}
/*
 * A diferencia de la anterior, esta funcion no reemplaza el elemento mayor tras
 * insertar un nuevo elemento
 */
static void prueba_NO_reemplazo_mayor(){

	char* primero = "aaa";
	char* segundo = "bbb";

	heap_t * heap = heap_crear(comparacion);
	heap_encolar(heap, segundo);
	print_test("Prueba cantidad correcta", heap_cantidad(heap) == 1);
	void * desen = heap_ver_max(heap);
	print_test("Maximo correcto",(strcmp(segundo, (char*)desen) == 0));
	print_test("Insertar otro elemento", heap_encolar(heap, primero));
	desen = heap_ver_max(heap);
	print_test("Maximo correcto",(strcmp(segundo, (char*)desen) == 0));
	heap_destruir(heap, NULL);
}

static void prueba_desencolado_mayor(){

	char* primero = "aaa";
	char* segundo = "bbb";

	heap_t * heap = heap_crear(comparacion);
	heap_encolar(heap, primero);
	heap_encolar(heap, segundo);
	void * desen = heap_ver_max(heap);
	print_test("Maximo tras encolado correcto",(strcmp(segundo, (char*)desen) == 0));
	desen = heap_desencolar(heap);
	print_test("Desencolado correcto",(strcmp(segundo, (char*)desen) == 0));
	desen = heap_ver_max(heap);
	print_test("Maximo tras esencolado correcto",(strcmp(primero, (char*)desen) == 0));
	heap_destruir(heap, NULL);
}


static void prueba_heapsort(){
	const size_t cant = 7;
	bool ok = true;
	char* ordenado[] = {"aaa","aax", "abb","bta", "btw", "ssd", "usb"};
	char* elementos[] = {"aaa", "ssd","bta", "usb", "btw", "aax","abb"};
	heap_sort((void**)elementos, cant, comparacion);
	for (size_t i = 0; i < cant; i++) {
		
		ok = ok && strcmp(ordenado[i], elementos[i]) == 0;
	}
	print_test("Ordenamiento correcto usando Heapsort", ok);
}

static void prueba_volumen_est(){
	int tope = 5000;
	int vector[tope];
	heap_t * heap = heap_crear(comparacion);
	bool ok = true;
	int i = 0;
	while (ok && i < tope){
	ok = ok && (heap_cantidad(heap) == (i));
	ok = ok && heap_encolar(heap, &vector[i]);
	i++;
	}
	print_test("Prueba de volumen insertar muchos elementos", ok);

	for (int j = 0; j <= i; j++){
		heap_desencolar(heap);
	}
	print_test("Prueba de volumen borrar muchos elementos", (heap_cantidad(heap) == 0));

	heap_destruir(heap, NULL);
}

void pruebas_heap_estudiante(){
	pruebas_creado();
	prueba_crear_sin_funcion_comparacion();
	prueba_encolado_desencolado_un_elemento();
	prueba_encolado_un_elemento();
	prueba_reemplazo_mayor();
	prueba_NO_reemplazo_mayor();
	prueba_desencolado_mayor();
	prueba_heapsort();
	prueba_volumen_est();

}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void)
{
    pruebas_heap_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif
