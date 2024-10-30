#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int iterations = 100;

    // Broadcast da configuração
    if (rank == 0) {
        iterations = 100;  // Define o número de iterações no processo raiz
    }
    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa o cálculo com a configuração recebida
    int result = 0;
    for (int i = 0; i < iterations; ++i) {
        result += rank * i;
    }

    std::cout << "Processo " << rank << " resultado local: " << result << std::endl;

    MPI_Finalize();
    return 0;
}
