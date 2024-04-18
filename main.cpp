#include <iostream>
#include <cstdio>
#include <mpi.h>

int main() {
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hola mundo desde el proceso %d\n", rank);
    MPI_Finalize();
}
