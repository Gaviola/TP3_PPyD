#include <iostream>
#include <mpi.h>
#include <cstring>

/*
 Programacion Paralela y Distribuida
 Yeumen Silva, Andres Maglione y Facundo Gaviola
 02/05/2024
 Comando para compilar: mpic++ Ejercicio7.cpp -o Ejercicio7
 Comando para ejecutar: mpirun -oversubscribe -np 8 ./Ejercicio7
 */

using namespace std;

void ejercicio7();

int main() {
    ejercicio7();
}

void Bcast(int  source, char *msg, int tam){

    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if( rank == source){
        for(int i = 0; i < size; i++){
            if(i != source){
                MPI_Send(msg, tam, MPI_CHAR, i, 0, comm);
            }
        }
    }
    else if (rank != source){
        MPI_Recv(msg, tam, MPI_CHAR, source, 0, comm, MPI_STATUS_IGNORE);
    }
}

void ejercicio7(){
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int source = 3;
    const int tam = 50;
    char mensaje[tam];
    if (rank == source){
        strcpy(mensaje, "Hello World");
    }

    Bcast(source,mensaje,tam);

    if(rank != source){
        cout << "Proceso " << rank << " tiene el mensaje: " << mensaje << endl;
    }

    MPI_Finalize();
}