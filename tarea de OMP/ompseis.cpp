/*
6. Algoritmo de Ordenamiento Parallel Merge Sort
Objetivo: Paralelizar el algoritmo de ordenamiento Merge Sort utilizando
OpenMP.
• Implementa el algoritmo Merge Sort de manera secuencial.
• Paraleliza la división y combinación de subarreglos utilizando OpenMP.
3
• Compara los tiempos de ejecución entre la versión secuencial y la paralelizada.
Preguntas:
• ¿Cómo se paraleliza la fase de dividir y combinar los subarreglos?
• ¿Qué secciones del algoritmo son más costosas y cómo pueden optimizarse con OpenMP?

*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

void merge(int* array, int izquierda, int medio, int derecha) {
    int n1 = medio - izquierda + 1;
    int n2 = derecha - medio;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = array[izquierda + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[medio + 1 + j];

    int i = 0, j = 0, k = izquierda;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            array[k++] = L[i++];
        else
            array[k++] = R[j++];
    }

    while (i < n1) array[k++] = L[i++];
    while (j < n2) array[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergesort(int* array, int izquierda, int derecha) {
    if (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        mergesort(array, izquierda, medio);
        mergesort(array, medio + 1, derecha);
        merge(array, izquierda, medio, derecha);
    }
}

void mergesortparallel(int* array, int izquierda, int derecha, int profundidad) {
    if (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (profundidad <= 0) {
            mergesort(array, izquierda, medio);
            mergesort(array, medio + 1, derecha);
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergesortparallel(array, izquierda, medio, profundidad - 1);

                #pragma omp section
                mergesortparallel(array, medio + 1, derecha, profundidad - 1);
            }
        }
        merge(array, izquierda, medio, derecha);
    }
}

void mergepare(){
    const int size = 1000000;
    int* array = new int[size];

    srand(time(0));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100 + 1;
    }

    // secuencial
    auto inicio = high_resolution_clock::now();
    mergesort(array, 0, size - 1);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "Secuencial Merge Sor)\n";
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos\n";

    // desordenar
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100 + 1;
    }

    // paralelización
    inicio = high_resolution_clock::now();
    mergesortparallel(array, 0, size - 1, 4);
    fin = high_resolution_clock::now();
    duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "Paralelización Merge Sort\n";
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos\n";

    delete[] array;
}
