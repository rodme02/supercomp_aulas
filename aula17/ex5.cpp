#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int valor = rank + 1;  // Cada processo tem um valor exclusivo
    int soma = 0;

    if (rank == 0) {
        soma = valor;
        MPI_Send(&soma, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&soma, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        soma += valor;
        if (rank != size - 1) {
            MPI_Send(&soma, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            printf("Soma total: %d\n", soma);
        }
    }

    MPI_Finalize();
    return 0;
}
