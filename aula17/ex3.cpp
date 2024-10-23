#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtenha o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtenha o tamanho total de processos

    int mensagem;
    if (rank == 0) {
        mensagem = 100;  // Mensagem inicial
        MPI_Send(&mensagem, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  // Envia para o próximo processo
    } else {
        MPI_Recv(&mensagem, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Recebe do processo anterior
        printf("Processo %d recebeu a mensagem %d\n", rank, mensagem);

        if (rank != size - 1) {  // Se não for o último processo, envia para o próximo
            MPI_Send(&mensagem, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
