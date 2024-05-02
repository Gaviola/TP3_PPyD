#include <cstdio>
#include <mpi.h>

/*
 Programacion Paralela y Distribuida
 Yeumen Silva, Andres Maglione y Facundo Gaviola
 02/05/2024
 Comando para compilar: mpic++ Ejercicio3.cpp -o Ejercicio3
 Comando para ejecutar: mpirun -oversubscribe -np 8 ./Ejercicio3
 */

using namespace std;

void ejercicio3();

int main() {
    ejercicio3();
}

void ejercicio3() {
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hola mundo desde el proceso %d\n", rank);
    MPI_Finalize();
}