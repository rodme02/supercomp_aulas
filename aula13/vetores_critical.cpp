#include <vector>
#include <iostream>
#include <omp.h> // Inclui a biblioteca do OpenMP para medir o tempo

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000; 
    std::vector<double> vec;

    double start_time = omp_get_wtime(); // Captura o tempo de início

    // Preenchendo o vetor em uma região crítica
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        {
            vec.push_back(conta_complexa(i));
        }
    }

    // Exibindo os índices
    for (int i = 0; i < N; i++) {
        std::cout << i << " ";
    }

    double end_time = omp_get_wtime(); // Captura o tempo de fim
    double exec_time = end_time - start_time; // Calcula o tempo de execução

    // Exibe o tempo de execução
    std::cout << "\nTempo de execução: " << exec_time << " segundos." << std::endl;

    return 0;
}