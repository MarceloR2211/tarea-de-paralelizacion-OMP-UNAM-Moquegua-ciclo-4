/*
3. Búsqueda de Máximo en un Arreglo
 Objetivo: Utilizar OpenMP para encontrar el valor máximo en un arreglo
 de tamaño grande.
 • Crea un arreglo de 106 enteros aleatorios.
 • Implementa un algoritmo secuencial que encuentre el valor máximo.
 • Paraleliza la búsqueda utilizando OpenMP.
 Preguntas:
 • ¿Cuál es la diferencia en el tiempo de ejecución entre la versión secuen
cial y la paralela?
 • ¿Cómo se gestionan las variables compartidas y privadas en este tipo
 de algoritmo?
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
void buscar_max_en_arreglo(){
    const int size = 106;
    int* array = new int[size];

    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        array[i]= rand()%100+1;
    }
//mayor por secuencial
auto inicio = high_resolution_clock::now();
int mayor = array[0];
for (int i = 1; i<size;i++){
	if(mayor<array[i]){
		mayor=array[i];
	}
}
auto fin = high_resolution_clock::now();
auto duracion = duration_cast<milliseconds>(fin - inicio);
	for (int i = 0; i < size; i++)
    {
	cout<<" " << array[i];
	}
    cout<<"mayor, secuencial: " << mayor << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
    
//mayor por paralela
mayor = array[0];

inicio = high_resolution_clock::now();
//inicio de regin donde entran varios hilos entre {}
#pragma omp parallel
{
    int mayor_local = mayor;
//divide for en hilos
    #pragma omp for
    for (int i = 1; i < size; i++) {
        if (array[i] > mayor_local) {
            mayor_local = array[i];
        }
    }
//el critical ase que solo un hilo a la ves ejecute parte del codigo en {}
    #pragma omp critical
    {
        if (mayor_local > mayor) {
            mayor = mayor_local;
        }
    }
}
fin = high_resolution_clock::now();
duracion = duration_cast<milliseconds>(fin - inicio);
	for (int i = 0; i < size; i++)
    {
	cout<<" " << array[i];
	}
    cout<<"mayor, paralela: " << mayor << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
    
}
