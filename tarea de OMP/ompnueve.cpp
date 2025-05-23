/*
9. Simulación de un Juego de la Vida (Conway’s Game
 of Life)
 Objetivo: Paralelizar la simulación del Juego de la Vida de Conway uti
lizando OpenMP.
 • Implementa la simulación del Juego de la Vida de Conway en una
 matriz bidimensional.
 • Paraleliza la actualización de las celdas utilizando OpenMP.
 • Compara el rendimiento entre la versión secuencial y la paralelizada.
 Preguntas:
 • ¿Cómo paralelizar el proceso de actualización de celdas sin crear de
pendencias entre los hilos?
 • ¿Qué tan escalable es la simulación cuando se aumenta el tamaño de
 la matriz?
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

bool muerto(const vector<vector<int>>& matriz) {
    for (const auto& fila : matriz) {
        for (int celda : fila) {
            if (celda == 1)
                return false;
        }
    }
    return true;
}

void imprimirmatris(const vector<vector<int>>& matriz) {
    int n = matriz.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout <<" "<<matriz[i][j]<<" ";
        }
        cout << endl;
    }
}

int cantidad_vecinos(const vector<vector<int>>& matris, int i, int j) {
    int n = matris.size();
    int contador = 0;
    int losx[3] = {i-1, i, i+1};
    int losy[3] = {j-1, j, j+1};

    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            if (losx[k] >= 0 && losx[k] < n && losy[l] >= 0 && losy[l] < n) {
                if (matris[losx[k]][losy[l]] == 1) {
                    contador++;
                }
            }
        }
    }

    if (matris[i][j] == 1) {
        contador--;
    }

    return contador;
}

vector<vector<int>> siguiente_generacion(const vector<vector<int>>& actual) {
    int n = actual.size();
    vector<vector<int>> siguiente(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int vecinos = cantidad_vecinos(actual, i, j);
            if (actual[i][j] == 1) {
                // supervivencia
                if (vecinos == 2 || vecinos == 3)
                    siguiente[i][j] = 1;
                // muerte por soledad o sobrepoblación
            } else {
                // Nacimiento
                if (vecinos == 3)
                    siguiente[i][j] = 1;
            }
        }
    }

    return siguiente;
}

bool muertoOMP(const vector<vector<int>>& matriz) {
	bool encontrado=true;
	int n = matriz.size();
	#pragma omp parallel for collapse(2) reduction(&&:encontrado)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == 1)
                encontrado = false;
        }
    }
    return encontrado;
}
/*
void imprimirmatrisOMP(const vector<vector<int>>& matriz) {
    int n = matriz.size();
    
    for (int i = 0; i < n; i++) {
    	#pragma omp parallel for collapse(1) num_threads(12)
        for (int j = 0; j < n; j++) {
            cout <<" "<<matriz[i][j]<<" ";
        }
        cout << endl;
    }
}//al parecer no se puede usar cout en paralelo por lo que vi
*/
/*int cantidad_vecinosOMP(const vector<vector<int>>& matris, int i, int j) {
    int n = matris.size();
    int contador = 0;
    int losx[3] = {i-1, i, i+1};
    int losy[3] = {j-1, j, j+1};

	#pragma omp parallel for collapse(2) reduction(+:contador)
    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            if (losx[k] >= 0 && losx[k] < n && losy[l] >= 0 && losy[l] < n) {
                if (matris[losx[k]][losy[l]] == 1) {
                    contador++;
                }
            }
        }
    }

    if (matris[i][j] == 1) {
        contador--;
    }

    return contador;
}
*/
vector<vector<int>> siguiente_generacionOMP(const vector<vector<int>>& actual) {
    int n = actual.size();
    vector<vector<int>> siguiente(n, vector<int>(n, 0));

	#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int vecinos = cantidad_vecinos(actual, i, j);
            if (actual[i][j] == 1) {
                // supervivencia
                if (vecinos == 2 || vecinos == 3)
                    siguiente[i][j] = 1;
                // muerte por soledad o sobrepoblación
            } else {
                // Nacimiento
                if (vecinos == 3)
                    siguiente[i][j] = 1;
            }
        }
    }

    return siguiente;
}

void juego_de_vida() {
    cout << "Escoja el tamaño de la matriz: ";
    int n;
    cin >> n;

    cout << "Escoja cantidad inicial de vida aleatoria en la matriz: ";
    int vn;
    cin >> vn;

    vector<vector<int>> matriz(n, vector<int>(n, 0)); //se cambio a esta funcion de vector por errores en la ejecucion
    srand(time(0));

    for (int i=0;i<vn;i++){
        matriz[rand()%n][rand()%n]=1;
    }
    vector<vector<int>> copiaparaomp = matriz;
//secuencial
auto inicio = high_resolution_clock::now();

    int generaciones = 1000;
    for (int paso = 0; paso < generaciones; paso++) {
        cout << "\nGeneración " << paso + 1 << ":\n";
        imprimirmatris(matriz);
        if (muerto(matriz)) {
        cout << "termino";
        break;
    	}
        matriz = siguiente_generacion(matriz);

    }
auto fin = high_resolution_clock::now();
auto duracion = duration_cast<milliseconds>(fin - inicio);
cout << "juego de vida secuencial: " <<endl;
cout << "Tiempo transcurrido: " << duracion.count() << " milisegundos"<<endl;
system("pause");  // pausa para anotar

//paralela
inicio = high_resolution_clock::now();

    generaciones = 1000;
    for (int paso = 0; paso < generaciones; paso++) {
        cout << "\nGeneración " << paso + 1 << ":\n";
        imprimirmatris(copiaparaomp);
        if (muertoOMP(copiaparaomp)) {
        cout << "termino";
        break;
    	}
        copiaparaomp = siguiente_generacionOMP(copiaparaomp);

    }
fin = high_resolution_clock::now();
duracion = duration_cast<milliseconds>(fin - inicio);
cout << "juego de vida paralelo: " <<endl;
cout << "Tiempo transcurrido: " << duracion.count() << " milisegundos"<<endl;
}



/*
void imprimirmatris(int[][] matris, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout<<matris[i][j]<<"\t";
        }
        cout<<endl;
    }
}

int cantidad_vecinos(int[][n] matris,const int n, int i,int j){
	int contador=0;
	int losx[3]={i-1,i,i+1};
	int losy[3]={j-1,j,j+1};
	
    //
    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            if(losx[k]<n and losx[k]>-1 and losy[l]<n and losy[l]>-1){
            	if(matris[losx[k]][losy[l]]==1){
    				contador++;
				}
			}
        }
    }
    if(matris[i][j]==1){
    	contador--;
	}
    return contador;
}

void supervivencia(int[][] matris, int n){}
void muerte_x_soledad(int[][] matris, int n){}
void muerte_x_sobrepoblacion(int[][] matris, int n){}
void nacimiento(int[][] matris, int n){}

void juego_de_vida(){

    cout << "Escoja el tamaño de la matriz: ";
    const int n;
    cin >> n;

    cout << "Escoja cantidad inicial de vida: ";
    int vn;
    cin >> vn;
    
    int matriz[n][n];

    srand(time(0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }
    
    for (int i=0;i<vn;i++){
        matriz[rand()%n][rand()%n]=1;
    }
    
    //secuencial
    int axi=0;
    while(axi<1000){
        imprimirmatris(matriz,n);

        for(int i = 0;i<n;i++){
            for (int j = 0; j < n; j++) {
            int vecinos=cantidad_vecinos(matriz,n,i,j);
            
            }
        }
    }
}
*/