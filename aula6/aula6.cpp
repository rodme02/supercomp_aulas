#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// Função recursiva para calcular o valor máximo e o peso correspondente que pode ser colocado na mochila
pair<int, int> calcularValorPesoMaximo(int capacidadeRestante, const vector<int>& pesos, const vector<int>& valores, int indiceItem, int& combinacoes) {
    combinacoes++;  // Conta cada chamada recursiva como uma combinação considerada

    // Caso base: sem mais itens ou capacidade restante
    if (indiceItem == 0 || capacidadeRestante == 0) return {0, 0};

    // Se o peso do item atual excede a capacidade restante, pula para o próximo item
    if (pesos[indiceItem - 1] > capacidadeRestante) {
        return calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);
    } else {
        // Caso 1: incluir o item atual na mochila
        pair<int, int> incluirItem = calcularValorPesoMaximo(capacidadeRestante - pesos[indiceItem - 1], pesos, valores, indiceItem - 1, combinacoes);
        incluirItem.first += valores[indiceItem - 1];
        incluirItem.second += pesos[indiceItem - 1];

        // Caso 2: não incluir o item atual na mochila
        pair<int, int> excluirItem = calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);

        // Retorna o par (valor, peso) que maximiza o valor
        return (incluirItem.first > excluirItem.first) ? incluirItem : excluirItem;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <arquivo de entrada> <numero de iteracoes>" << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada) {
        cout << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    int iteracoes = stoi(argv[2]);

    int numeroDeItens, capacidadeDaMochila;
    arquivoEntrada >> numeroDeItens >> capacidadeDaMochila;

    vector<int> pesos(numeroDeItens), valores(numeroDeItens);
    for (int i = 0; i < numeroDeItens; ++i) {
        arquivoEntrada >> pesos[i] >> valores[i];
    }

    long long totalTempoExecucao = 0;
    int totalCombinacoes = 0;
    int valorMaximo = 0;
    int pesoOcupado = 0;

    // Executa o algoritmo 'iteracoes' vezes
    for (int i = 0; i < iteracoes; ++i) {
        int combinacoes = 0;
        auto inicio = high_resolution_clock::now();

        auto resultado = calcularValorPesoMaximo(capacidadeDaMochila, pesos, valores, numeroDeItens, combinacoes);

        auto fim = high_resolution_clock::now();
        totalTempoExecucao += duration_cast<nanoseconds>(fim - inicio).count();
        totalCombinacoes += combinacoes;
        valorMaximo = resultado.first;
        pesoOcupado = resultado.second;
    }

    cout << "Numero de iteracoes: " << iteracoes << endl;
    cout << "Numero medio de combinacoes avaliadas: " << totalCombinacoes / iteracoes << endl;
    cout << "Valor maximo: " << valorMaximo << endl;
    cout << "Peso ocupado: " << pesoOcupado << endl;
    cout << "Tempo medio de execucao: " << totalTempoExecucao / iteracoes << " ns" << endl;

    return 0;
}
