#include <cstdio>
#include <mpi.h>

/*
 Programacion Paralela y Distribuida
 Yeumen Silva, Andres Maglione y Facundo Gaviola
 02/05/2024
 Comando para compilar: mpic++ Ejercicio5.cpp -o Ejercicio5
 Comando para ejecutar: mpirun -oversubscribe -np 8 ./Ejercicio5
 */

using namespace std;

void ejercicio5();

int main() {
    ejercicio5();
}

int getGroup(int rank) {
    int result;
    int tamano = 8;
    result = rank % tamano;
    if (result < tamano / 2) {
        return 0;
    } else {
        return 1;
    }
}

int getGroup2(int rank) {
    int result;
    int tamano = 8;
    result = rank % tamano;
    if (result < tamano / 2) {
        if (result % 2 == 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (result % 2 == 0) {
            return 0;
        } else {
            return 1;
        }
    }
}

void ejercicio5() {
    MPI_Init(nullptr, nullptr);
    int my_id, new_id;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_split(MPI_COMM_WORLD, getGroup(my_id),my_id,&newcomm);
    MPI_Comm_rank(newcomm,&new_id);
    printf("Proceso %d en el grupo %d\n", my_id, getGroup(my_id));
    MPI_Finalize();
}