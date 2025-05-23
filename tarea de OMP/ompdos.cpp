/*
2. Multiplicación de Matrices
Objetivo: Paralelizar el proceso de multiplicación de dos matrices utilizando
OpenMP.
• Crea dos matrices A y B de tamaño n × n con valores aleatorios.
• Implementa la multiplicación de matrices de manera secuencial.
• Paraleliza el algoritmo utilizando OpenMP.
• Mide el tiempo de ejecución para la versión secuencial y paralela.
Preguntas:
• ¿Cómo cambia el tiempo de ejecución al aumentar el tamaño de las
matrices?
• ¿Cuántos hilos son necesarios para optimizar el tiempo de ejecución?

imprimirMatriz(matrisR, n);
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// imprimir
void imprimirMatriz(int** matriz, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << matriz[i][j] << "\t";
        }
        cout << endl;
    }
}

void multiplicar_matrices() {
    
    cout << "Escoja el tamaño n: ";
    int n;
    cin >> n;

    int** matris1 = new int*[n];
    int** matris2 = new int*[n];
    int** matrisR = new int*[n];
    for (int i = 0; i < n; i++) {
        matris1[i] = new int[n];
        matris2[i] = new int[n];
        matrisR[i] = new int[n];
    }

    srand(time(0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            matris1[i][j] = rand() % 100 + 1;
            matris2[i][j] = rand() % 100 + 1;
        }

    // multiplicación paralela
    auto inicio = high_resolution_clock::now();
    #pragma omp parallel for collapse(2) num_threads(12)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrisR[i][j] = 0;
            for (int k = 0; k < n; k++) {
                matrisR[i][j] += matris1[i][k] * matris2[k][j];
            }
        }
    }
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "\nMultiplicación paralela: " <<endl;
    
    cout << "Tiempo transcurrido: " << duracion.count() << " milisegundos"<<endl;

    inicio = high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrisR[i][j] = 0;
            for (int k = 0; k < n; k++) {
                matrisR[i][j] += matris1[i][k] * matris2[k][j];
            }
        }
    }
    fin = high_resolution_clock::now();
    duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "Multiplicación secuencial: " <<endl;
    
    cout << "Tiempo transcurrido: " << duracion.count() << " milisegundos"<<endl;

    for (int i = 0; i < n; i++) {
        delete[] matris1[i];
        delete[] matris2[i];
        delete[] matrisR[i];
    }
    delete[] matris1;
    delete[] matris2;
    delete[] matrisR;
    
}
