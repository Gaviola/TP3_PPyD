#include <iostream>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

/*
 Programacion Paralela y Distribuida
 Yeumen Silva, Andres Maglione y Facundo Gaviola
 02/05/2024
 Comando para compilar: mpic++ Ejercicio6.cpp -o Ejercicio6
 Comando para ejecutar: mpirun -oversubscribe -np 8 ./Ejercicio6
 */

using namespace std;

void ejercicio6();

int main() {
    ejercicio6();
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