/*
5. Implementación de un Algoritmo de Búsqueda Binaria Paralela
Objetivo: Paralelizar un algoritmo de búsqueda binaria utilizando OpenMP.
• Implementa un algoritmo de búsqueda binaria de manera secuencial.
• Paraleliza el algoritmo de búsqueda binaria utilizando OpenMP.
• Aplica la paralelización al buscar elementos en un arreglo ordenado.
Preguntas:
• ¿Cómo se puede dividir el arreglo para optimizar la paralelización?
• ¿Cuánto mejora el rendimiento al usar múltiples hilos?

*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
#include <algorithm>//paraordenar 
using namespace std;
using namespace std::chrono;

void busqueda_binaria(){

const int size = 1000000;
    int* array = new int[size];
	int sum=0;
	
    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        array[i]= rand()%1000000+1;
    }
sort(array,array + size);
//deben ser punteros y no numeros enteros por eso el array+n

cout << "\nescoje el numero a buscar " <<endl;
int bus;
cin>>bus;
//secuencial
	auto inicio = high_resolution_clock::now();
    int resultado = -1;
	int izq=0;
	int der=size-1;
	
	while(izq<=der){
		int med= izq+(der-izq)/2;
		if (array[med] == bus) {
            #pragma omp critical
            resultado = med;
                break;
        }
        if (array[med] < bus) {
            izq = med + 1;
        } else {
            der = med - 1;
        }
	}
    
    auto fin = high_resolution_clock::now();
	auto duracion = duration_cast<milliseconds>(fin - inicio);
	cout << "secuencial " << endl;
	if (resultado != -1){
	
        cout << "Elemento encontrado en la posición " << resultado << endl;
    }else{
        cout << "Elemento no encontrado." << endl;
}
	cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
//paralelisacion
	resultado = -1;
    int num_hilos = 4;
    int bloque = size / num_hilos;

    inicio = high_resolution_clock::now();

    #pragma omp parallel num_threads(num_hilos)
    {
        int id = omp_get_thread_num();
        int inicio_bloque = id * bloque;
        int fin_bloque = (id == num_hilos - 1) ? size - 1 : (inicio_bloque + bloque - 1);

        int izq_local = inicio_bloque, der_local = fin_bloque;

        while (izq_local <= der_local && resultado == -1) {
            int med = izq_local + (der_local - izq_local) / 2;
            if (array[med] == bus) {
                #pragma omp critical
                resultado = med;
                break;
            }
            if (array[med] < bus) {
                izq_local = med + 1;
            } else {
                der_local = med - 1;
            }
        }
    }
    
    fin = high_resolution_clock::now();
	duracion = duration_cast<milliseconds>(fin - inicio);
	cout << "paralelizacion " << endl;
	if (resultado != -1)
        cout << "Elemento encontrado en la posición " << resultado << endl;
    else
        cout << "Elemento no encontrado." << endl;
        
	cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
}
