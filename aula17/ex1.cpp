#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        char mensagem[] = "Olá";
        MPI_Send(mensagem, 4, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(mensagem, 4, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo 0 recebeu: %s\n", mensagem);
    } else if (rank == 1) {
        char mensagem[4];
        MPI_Recv(mensagem, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo 1 recebeu: %s\n", mensagem);
        char resposta[] = "Oi";
        MPI_Send(resposta, 3, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
