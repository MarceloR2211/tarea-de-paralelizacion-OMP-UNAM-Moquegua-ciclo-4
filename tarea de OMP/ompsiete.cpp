/*
7. Simulación de Monte Carlo para Pi
Objetivo: Estimar el valor de π utilizando el método de Monte Carlo, y
paralelizar el proceso de simulación con OpenMP.
• Implementa el método de Monte Carlo para estimar π utilizando una
simulación de puntos aleatorios.
• Paraleliza el proceso de generación de puntos y el cálculo de la estimación de π utilizando OpenMP.
• Compara el rendimiento y la precisión de la versión secuencial y paralela.
Preguntas:
• ¿Cómo afecta el número de puntos generados a la precisión de la estimación?
• ¿Qué mejoras se logran al paralelizar la simulación de Monte Carlo?
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
void pi_motecarlo(){
//secuencial
auto inicio = high_resolution_clock::now();
	int total_puntos = 1000000;
    int puntos_dentro = 0;

    srand(time(0));

    for (int i = 0; i < total_puntos; i++) {
        double x = (double)rand() / RAND_MAX; //[0, 1]
        double y = (double)rand() / RAND_MAX; // [0, 1]

        if (x * x + y * y <= 1.0) {
            puntos_dentro++;
        }
    }

    double pi_estimada = 4.0 * puntos_dentro / total_puntos;
auto fin = high_resolution_clock::now();
auto duracion = duration_cast<milliseconds>(fin - inicio);
	cout << "secuencial " << endl;
	cout << "Estimación de pi usando " << total_puntos << " puntos: " << pi_estimada << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos\n";
//paralelo
inicio = high_resolution_clock::now();
	total_puntos = 1000000;
    puntos_dentro = 0;

    srand(time(0));
    #pragma omp parallel for reduction(+:puntos_dentro)
    for (int i = 0; i < total_puntos; i++) {
        double x = (double)rand() / RAND_MAX; //[0, 1]
        double y = (double)rand() / RAND_MAX; // [0, 1]

        if (x * x + y * y <= 1.0) {
            puntos_dentro++;
        }
    }

    pi_estimada = 4.0 * puntos_dentro / total_puntos;
fin = high_resolution_clock::now();
duracion = duration_cast<milliseconds>(fin - inicio);
	cout << "paralelo " << endl;
	cout << "Estimación de pi usando " << total_puntos << " puntos: " << pi_estimada << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos\n";
    
}