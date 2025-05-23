/*
8. Integración Numérica con el Método de Trapecios
Objetivo: Implementar la integración numérica del área bajo una curva
utilizando el método de trapecios, y paralelizar el cálculo con OpenMP.
• Calcula la integral de una función f(x) sobre un intervalo utilizando el
método de trapecios.
• Paraleliza el cálculo de las áreas de los trapecios utilizando OpenMP.
4
• Mide y compara el rendimiento entre las versiones secuencial y paralela.
Preguntas:
• ¿Qué tan efectivo es OpenMP para paralelizar el cálculo de la suma de
las áreas de los trapecios?
• ¿Cómo mejora el tiempo de ejecución a medida que se aumenta el
número de trapecios?

Para n subintervalos (Método del Trapecio Compuesto) esta formula es mas precisa:
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
#include <cmath>
using namespace std;
using namespace std::chrono;

// f(X)
double f(double x) {
	double resultado=(5+x)/x;
    return resultado;  // aqui la funcion f(x) no logre conseguir entrar la funcion por cin 
}

double integral_trapecio_secuencial(double a, double b, int n) {
    double h = (b - a) / n;
    double suma = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; ++i) {
        double xi = a + i * h;
        suma += f(xi);
    }

    return h * suma;
}

double integral_trapecio_paralela(double a, double b, int n) {
    double h = (b - a) / n;
    double suma = 0.0;

    #pragma omp parallel for reduction(+:suma)
    for (int i = 1; i < n; ++i) {
        double xi = a + i * h;
        suma += f(xi);
    }

    suma += (f(a) + f(b)) / 2.0;

    return h * suma;
}

void integracion_numerica_con_trapecios(){
	cout << "ingrese los limites: "<<endl;
	cout << "el limite a:\n";
	double a;
	cin>>a;
	cout << "el limite b:\n";
    double b;
    cin>>b;
    int n = 100000000;  // mas precicion

    //secuencial
    auto inicio = high_resolution_clock::now();
    double resultado_sec = integral_trapecio_secuencial(a, b, n);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "resultado secuencial: " << resultado_sec << endl;
    cout << "Tiempo Secuencial: " << duracion_sec.count() << " ms" << endl;

    //paralela
    inicio = high_resolution_clock::now();
    double resultado_par = integral_trapecio_paralela(a, b, n);
    fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    cout << "\nresultado Paralelo: " << resultado_par << endl;
    cout << "Tiempo Paralelo: " << duracion_par.count() << " ms" << endl;   
}

