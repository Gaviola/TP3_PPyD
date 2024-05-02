#include <iostream>
#include <mpi.h>

/*
 Programacion Paralela y Distribuida
 Yeumen Silva, Andres Maglione y Facundo Gaviola
 02/05/2024
 Comando para compilar: mpic++ Ejercicio4.cpp -o Ejercicio4
 Comando para ejecutar: mpirun -oversubscribe -np 8 ./Ejercicio4
 */

using namespace std;

void ejercicio4();

int main() {
    ejercicio4();
}

void ejercicio4() {
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double tiempo_inicio;
    if (rank == 0) {
        string mensaje = "coso";
        tiempo_inicio = MPI_Wtime();
        MPI_Send(mensaje.c_str(),mensaje.size() + 1,MPI_CHAR,1,0,MPI_COMM_WORLD);
    } else {
        char mensaje[256];
        MPI_Recv(&mensaje,sizeof(mensaje),MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(mensaje,sizeof(mensaje),MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
    if (rank == 0) {
        char mensaje[256];
        MPI_Recv(mensaje,sizeof(mensaje),MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        double tiempo = MPI_Wtime() - tiempo_inicio;
        cout << fixed << tiempo << endl;
    }
    MPI_Finalize();
}