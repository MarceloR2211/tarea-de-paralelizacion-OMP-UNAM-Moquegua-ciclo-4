/*
10. Optimización de un Algoritmo de Fibonacci
 Objetivo: Paralelizar el cálculo de los números de Fibonacci utilizando
 OpenMP.
 • Implementa un algoritmo recursivo para calcular el n-ésimo número de
 Fibonacci.
 • Paraleliza la versión recursiva utilizando OpenMP para computar múlti
ples subproblemas de manera simultánea.
 5
• Mide el tiempo de ejecución de la versión secuencial y paralela.
 Preguntas:
 • ¿Cómo afecta la recursividad a la eficiencia cuando se paraleliza con
 OpenMP?
 • ¿Cuántos hilos son necesarios para maximizar el rendimiento de la par
alelización?
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

//secuencial
long long fibonacci_seq(int n) {
    if (n <= 1)
        return n;
    return fibonacci_seq(n - 1) + fibonacci_seq(n - 2);
}

//paralelo
long long fibonacci_par(int n) {
    if (n <= 1)
        return n;

    long long x, y;

    #pragma omp task shared(x)
    x = fibonacci_par(n - 1);

    #pragma omp task shared(y)
    y = fibonacci_par(n - 2);

    #pragma omp taskwait
    return x + y;
}



void fibonachi(){
	int n = 40; //Fibonacci a calcular
    long long resultado;
    auto inicio = high_resolution_clock::now();
    resultado = fibonacci_seq(n);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "fibonacci secuencial (" << n << ") = " << resultado << endl;
    cout << "tiempo secuencial: " << duracion.count() << " ms" << endl;


    inicio = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        resultado = fibonacci_par(n);
    }
    fin = high_resolution_clock::now();
    duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "fibonacci paralelo (" << n << ") = " << resultado << endl;
    cout << "tiempo paralelo: " << duracion.count() << " ms" << endl;

}