#include <iostream>
#include <cstdio>
#include <mpi.h>
#include <math.h>
#include <cstring>

using std::cout, std::endl;
using std::string;
using std::fixed;
using std::strcpy;

void ejercicio3();
void ejercicio4();
void ejercicio7();

int main() {
    //ejercicio3();
    //ejercicio4();
    ejercicio7();
}

void ejercicio3() {
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hola mundo desde el proceso %d\n", rank);
    MPI_Finalize();
}
//Ping Pong: Un proceso envia un mensje a otro proceso y este lo recibe y reenvia devuelta
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

void Bcast(int  source, char *msg, int tam){

    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if( rank == source){
        for(int i = 0; i < size; i++){
            MPI_Send(msg, tam, MPI_CHAR, i, 0, comm);
        }
    }
    else{
        MPI_Recv(msg, tam, MPI_CHAR, source, 0, comm, MPI_STATUS_IGNORE);
    }
}

void ejercicio7(){
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int tam = 50;
    char mensaje[tam];
    if (rank == 0){
        strcpy(mensaje, "Hello World");
    }

    Bcast(0,mensaje,tam);

    cout << "Proceso " << rank << " tiene el mensaje: " << mensaje << endl;

    MPI_Finalize();
}