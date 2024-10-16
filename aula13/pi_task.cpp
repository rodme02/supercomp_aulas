#include <omp.h>
#include <iostream>
#include <iomanip>
#include <atomic>

static long num_steps = 1024l * 1024 * 1024 * 2;
#define MIN_BLK 1024 * 1024 * 256

double sum = 0;
std::atomic<int> task_count(0); // Contador de tarefas

void pi_r(long Nstart, long Nfinish, double step, int max_tasks) {
    long i, iblk;
    if (Nfinish - Nstart < MIN_BLK || task_count >= max_tasks) {
        // Cálculo sequencial
        double local_sum = 0.0;
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        sum += local_sum;
    } else {
        iblk = Nfinish - Nstart;
        // Criar novas tarefas apenas se o limite de tarefas não foi atingido
        #pragma omp task
        {
            task_count++;
            pi_r(Nstart, Nfinish - iblk / 2, step, max_tasks);
        }
        #pragma omp task
        {
            task_count++;
            pi_r(Nfinish - iblk / 2, Nfinish, step, max_tasks);
        }
        #pragma omp taskwait
    }
}

int main() {
    long i;
    double step, pi;
    double init_time, final_time;
    int max_tasks = 64; // Defina o número máximo de tarefas aqui

    step = 1.0 / (double)num_steps;
    init_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        pi_r(0, num_steps, step, max_tasks);
    }
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;

    std::cout << "for " << num_steps << " steps, pi = " << std::setprecision(15) << pi 
              << " in " << final_time << " secs with max tasks = " << max_tasks << "\n";

    return 0;
}