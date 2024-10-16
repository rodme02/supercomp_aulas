#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <cctype>

int main() {
    thrust::host_vector<float> h_stock_prices;

    // Função para remover espaços em branco de ambos os lados de uma string
    auto trim = [](std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };

    // Passo 1: Leitura de dados a partir de std::cin (entrada padrão redirecionada)
    std::string line;
    while (std::getline(std::cin, line)) {
        // Remover o caractere de carriage return (\r) e espaços extras
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        trim(line);

        // Ignorar linhas vazias ou que só contenham espaços
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        float price;

        // Verificar se o formato da linha está correto
        if (!(ss >> price)) {
            std::cerr << "Erro ao ler preço das ações! Linha lida: '" << line << "'" << std::endl;
            return 1;
        }

        h_stock_prices.push_back(price);
    }

    // Verificar se lemos algum dado
    if (h_stock_prices.size() == 0) {
        std::cerr << "Nenhum dado foi lido." << std::endl;
        return 1;
    }

    // Passo 2: Medir o tempo de transferência para a GPU
    auto start = std::chrono::high_resolution_clock::now(); // Iniciar o cronômetro

    thrust::device_vector<float> d_stock_prices = h_stock_prices; // Transferir para a GPU

    auto end = std::chrono::high_resolution_clock::now(); // Parar o cronômetro
    std::chrono::duration<double> duration = end - start; // Calcular a duração

    // Passo 3: Imprimir o tempo gasto na alocação e cópia dos dados para a GPU
    std::cout << "Tempo de transferência dos dados para a GPU: " << duration.count() << " segundos." << std::endl;

    return 0;
}
