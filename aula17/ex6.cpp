#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char mensagem[20];

    if (rank == 0) {
        for (int i = 1; i < 4; i++) {
            sprintf(mensagem, "Olá processo %d", i);
            MPI_Send(mensagem, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Recv(mensagem, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Processo 0 recebeu: %s\n", mensagem);
        }
    } else {
        MPI_Recv(mensagem, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu: %s\n", rank, mensagem);
        sprintf(mensagem, "Resposta do processo %d", rank);
        MPI_Send(mensagem, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
