/*
4. Reducción con OpenMP
Objetivo: Entender cómo funciona la cláusula de reducción en OpenMP.
• Crea un arreglo de 106 números aleatorios.
• Suma los elementos del arreglo utilizando la cláusula de reducción en
OpenMP.
• Compara el rendimiento con y sin la cláusula de reducción.
Preguntas:
• ¿Qué sucede si no se utiliza la cláusula de reducción?
• ¿Cómo afecta la paralelización a la precisión y al tiempo de ejecución?

*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
void reduccion(){

 const int size = 106;
    int* array = new int[size];
	int sum=0;
	
    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        array[i]= rand()%100+1;
    }
    
//paralela inicio    
    auto inicio = high_resolution_clock::now();
    
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    auto fin = high_resolution_clock::now();
	auto duracion = duration_cast<milliseconds>(fin - inicio);
    
    cout<<"suma secuencial: " << sum << endl;
    cout << "tiempo transcurrido: " << duracion.count() << " milisegundos" << endl;
//paralela fin
//paralela con reduction inicio
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
//paralela con reduction fin
	delete[] array;
    
}