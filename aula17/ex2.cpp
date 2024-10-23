#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int mensagem = rank;
    int mensagem_recebida;

    MPI_Send(&mensagem, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    MPI_Recv(&mensagem_recebida, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Processo %d recebeu a mensagem %d\n", rank, mensagem_recebida);

    MPI_Finalize();
    return 0;
}
