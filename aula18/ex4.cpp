#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

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

    // Calcula média e variância locais
    float local_sum = std::accumulate(subarray.begin(), subarray.end(), 0.0f);
    float local_mean = local_sum / subarray_size;

    float local_variance = 0.0f;
    for (const auto& value : subarray) {
        local_variance += (value - local_mean) * (value - local_mean);
    }
    local_variance /= subarray_size;

    // Coleta médias e variâncias locais
    std::vector<float> local_means(size), local_variances(size);
    MPI_Gather(&local_mean, 1, MPI_FLOAT, local_means.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_variance, 1, MPI_FLOAT, local_variances.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cálculo do desvio padrão global
    if (rank == 0) {
        float mean_of_means = std::accumulate(local_means.begin(), local_means.end(), 0.0f) / size;
        float mean_variance = std::accumulate(local_variances.begin(), local_variances.end(), 0.0f) / size;

        float variance_of_means = 0.0f;
        for (const auto& mean : local_means) {
            variance_of_means += (mean - mean_of_means) * (mean - mean_of_means);
        }
        variance_of_means /= size;

        float global_variance = mean_variance + variance_of_means;
        float global_std_dev = std::sqrt(global_variance);

        std::cout << "Desvio padrão global: " << global_std_dev << std::endl;
    }

    MPI_Finalize();
    return 0;
}
