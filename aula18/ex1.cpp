#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;
    std::vector<float> data;

    // Processo raiz inicializa o array
    if (rank == 0) {
        data.resize(array_size);
        for (int i = 0; i < array_size; ++i) {
            data[i] = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    // Define o tamanho do subarray
    int subarray_size = array_size / size;
    std::vector<float> subarray(subarray_size);

    // Distribui o array
    MPI_Scatter(data.data(), subarray_size, MPI_FLOAT, subarray.data(), subarray_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calcula a média local
    float local_sum = std::accumulate(subarray.begin(), subarray.end(), 0.0f);
    float local_mean = local_sum / subarray_size;

    // Coleta as médias locais
    std::vector<float> local_means(size);
    MPI_Gather(&local_mean, 1, MPI_FLOAT, local_means.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calcula a média global
    if (rank == 0) {
        float global_mean = std::accumulate(local_means.begin(), local_means.end(), 0.0f) / size;
        std::cout << "Média global: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
