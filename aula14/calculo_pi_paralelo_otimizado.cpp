#include <iostream>
#include <random>   // Para geradores de números aleatórios modernos
#include <ctime>
#include <cmath>
#include <omp.h>

int main() {
    const int N = 100000000; // Número de pontos
    int count = 0; // Contagem de pontos dentro do círculo

    // Inicia a contagem de tempo
    double start_time = omp_get_wtime();

    // Paralelizando o loop usando OpenMP
    #pragma omp parallel
    {
        // Cada thread possui seu próprio gerador de números aleatórios
        std::mt19937 rng;
        rng.seed(static_cast<unsigned int>(std::time(0)) + omp_get_thread_num());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        int local_count = 0;

        #pragma omp for
        for (int i = 0; i < N; ++i) {
            // Gera coordenadas x e y usando o gerador local da thread
            double x = dist(rng);
            double y = dist(rng);

            // Verifica se o ponto está dentro do círculo
            if (x * x + y * y <= 1) {
                ++local_count;
            }
        }

        // Redução para somar os valores locais de cada thread no total global
        #pragma omp atomic
        count += local_count;
    }

    // Estimativa de Pi
    double pi_estimate = 4.0 * static_cast<double>(count) / N;

    // Finaliza a contagem de tempo
    double end_time = omp_get_wtime();
    double duration = end_time - start_time;

    // Exibe o valor estimado de Pi e o tempo de execução
    std::cout << "Valor estimado de Pi: " << pi_estimate << std::endl;
    std::cout << "Tempo de execução: " << duration << " segundos" << std::endl;

    return 0;
}
