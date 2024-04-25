#include <iostream>
#include <cstdio>
#include <mpi.h>
#include <math.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

void ejercicio3();
void ejercicio4();
void ejercicio5();
void ejercicio6();
void ejercicio7();

int main() {
    //ejercicio3();
    //ejercicio4();
    //ejercicio5();
    //ejercicio6();
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


void createDeck(vector<float>* cards) {
    for (int i=0; i < 4; i++) {
        for (int j=1; j <= 7; j++) {
            cards->push_back((float) j);
        }
    }
    for(float i=0; i < 12; i++){
        cards ->push_back(0.5);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(cards->begin(), cards->end(), default_random_engine(seed));
}

int calculatePlayer(int currentPlayer, int nproc) {
    if (currentPlayer == (nproc - 1)) {
        return 1;
    } else {
        return currentPlayer + 1;
    }
}

void ejercicio6() {
    MPI_Init(nullptr, nullptr);
    int rank, nproc;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int tagCard = 0, tagRes = 1;
    if (rank == 0) {
        vector<float> cards;
        createDeck(&cards);
        int currentPlayer = 1;
        vector<bool> stoppedProcesses(nproc, 1);

        // Mandar cartas a cada proceso
        for (int c = 0; c < cards.size(); c++) {
            int supposedPlayer = currentPlayer;
            bool shouldStopFor = false;
            while (!stoppedProcesses[currentPlayer] && !shouldStopFor) {
                currentPlayer = calculatePlayer(currentPlayer, nproc);

                if (supposedPlayer == currentPlayer) {
                    shouldStopFor = true;
                }
            }

            if (shouldStopFor) {
                break;
            }
            MPI_Send(&cards[c], 1, MPI_FLOAT, currentPlayer, 0 ,MPI_COMM_WORLD);
            bool stopped;
            MPI_Recv(&stopped, 1, MPI_C_BOOL, currentPlayer, 1, MPI_COMM_WORLD, nullptr);
            stoppedProcesses[currentPlayer] = stopped;

            currentPlayer = calculatePlayer(currentPlayer, nproc);
        }

        vector<float> points(nproc);
        for (int i = 1; i < nproc; i++) {
            float score;
            MPI_Recv(&score, 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD, nullptr);
            points[i] = score;
            cout << "El jugador " << i << " termina con " << points[i] << " puntos " << endl;
        }
        float mejorPuntuacion = 0;
        vector<int> ganadores;
        for (int i = 1; i < nproc; ++i) {
            if (points[i] > mejorPuntuacion && points[i] <= 7.5) {
                mejorPuntuacion = points[i];
                ganadores = {i};
            } else {
                if (points[i] == mejorPuntuacion) {
                    ganadores.push_back(i);
                }
            }
        }
        for (int i=0; i < ganadores.size();i++) {
            cout << "El ganador es el jugador " << ganadores[i] << " con " << mejorPuntuacion << " puntos." << endl;
        }





    } else {
        // Los workers reciben cartas
        float card;
        float points = 0;
        bool keepAsking = true;
        while (keepAsking)  {
            MPI_Recv(&card, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, nullptr);
            points += card;

            if(points > 7.5) {
                keepAsking = false;
                cout << "Jugador " << rank << " se pasa de 7.5" << endl;
            }
            if (points <= 7.5 && points >= 4) {
                keepAsking = false;
                cout << "Jugador " << rank << " se planta. Puntos finales: " << points << endl;
            }
            MPI_Send(&keepAsking, 1, MPI_C_BOOL, 0, 1, MPI_COMM_WORLD);
        }
        MPI_Send(&points, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);


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