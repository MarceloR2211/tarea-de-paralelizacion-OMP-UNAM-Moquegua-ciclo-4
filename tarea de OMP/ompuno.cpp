
/*
 1. Suma de Elementos de un Arreglo (Secuencial vs
 Paralelo)
 Objetivo: Comparar la eficiencia de la suma de elementos de un arreglo
 utilizando una implementación secuencial y paralela con OpenMP.
 • Crea un arreglo de un millón de enteros aleatorios.
 • Implementa una versión secuencial que sume todos los elementos del
 arreglo.
 • Paraleliza la versión secuencial utilizando OpenMP y mide el rendimiento.
 Preguntas:
 • ¿Cuánto tiempo tarda la ejecución secuencial y cuánto tiempo tarda la
 ejecución paralela?
 • ¿Qué ocurre al variar el número de hilos?
 • ¿Hay mejoras en el rendimiento al usar más hilos
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
int sumarArray() {    //se podria poner void???
    const int size = 1000000;
    int* array = new int[size];
    int sum = 0;

    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        array[i]= rand()%100+1;
    }
//secuencial inicio    
    auto inicio = high_resolution_clock::now();
    
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    
    auto fin = high_resolution_clock::now();
	auto duracion = duration_cast<milliseconds>(fin - inicio);
    
    cout<<"suma secuencial: " << sum << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
//secuencial fin
//paralela inicio
sum = 0;
	inicio = high_resolution_clock::now();
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    
    fin = high_resolution_clock::now();
	duracion = duration_cast<milliseconds>(fin - inicio);
    
    cout<<"suma paralela: " << sum << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
//paralela fin
	delete[] array;
    return 0;
}