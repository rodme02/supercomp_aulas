#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>

int main() {
    const int N = 100000000; // Número de pontos
    int count = 0; // Contagem de pontos dentro do círculo

    // Semente para o gerador de números aleatórios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Inicia a contagem de tempo
    double start_time = omp_get_wtime();

    // Paralelizando o loop usando OpenMP
    #pragma omp parallel for reduction(+:count)
    for (int i = 0; i < N; ++i) {
        // Cada thread gera suas próprias coordenadas x e y
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;

        // Verifica se o ponto está dentro do círculo
        if (x * x + y * y <= 1) {
            ++count;
        }
    }

    // Estimativa de Pi
    double pi_estimate = 4.0 * static_cast<double>(count) / N;

    // Finaliza a contagem de tempo
    double end_time = omp_get_wtime();
    double duration = end_time - start_time;

    // Exibe o valor estimado de Pi e o tempo de execução
    std::cout << "Valor estimado de Pi (paralelo): " << pi_estimate << std::endl;
    std::cout << "Tempo de execução (paralelo): " << duration << " segundos" << std::endl;

    return 0;
}
