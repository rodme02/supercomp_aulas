#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;
    std::vector<float> data;

    float max_value;
    if (rank == 0) {
        // Processo raiz inicializa o array e calcula o valor máximo
        data.resize(array_size);
        for (int i = 0; i < array_size; ++i) {
            data[i] = static_cast<float>(rand()) / RAND_MAX;
        }
        max_value = *std::max_element(data.begin(), data.end());
    }

    // Broadcast do valor máximo
    MPI_Bcast(&max_value, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Define o tamanho do subarray
    int subarray_size = array_size / size;
    std::vector<float> subarray(subarray_size);

    // Distribui o array
    MPI_Scatter(data.data(), subarray_size, MPI_FLOAT, subarray.data(), subarray_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Normaliza o subarray
    for (int i = 0; i < subarray_size; ++i) {
        subarray[i] /= max_value;
    }

    // Recolhe os subarrays normalizados
    if (rank == 0) {
        data.resize(array_size);
    }
    MPI_Gather(subarray.data(), subarray_size, MPI_FLOAT, data.data(), subarray_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Array normalizado:" << std::endl;
        for (const auto& value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
