#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Estrutura de um item
struct Item {
    int peso;
    int valor;
};

// Função para comparar itens pelo valor (mais caro primeiro)
bool compararPorValor(const Item& a, const Item& b) {
    return a.valor > b.valor;  // Ordena de forma decrescente pelo valor
}

// Função para comparar itens pelo peso (mais leve primeiro)
bool compararPorPeso(const Item& a, const Item& b) {
    return a.peso < b.peso;  // Ordena de forma crescente pelo peso
}

// Função da heurística "mais caro primeiro"
int mais_caro(int capacidadeDaMochila, vector<Item>& itens) {
    sort(itens.begin(), itens.end(), compararPorValor);  // Ordena pelo valor
    int valorTotal = 0, pesoAtual = 0;
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidadeDaMochila) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

// Função da heurística "mais leve primeiro"
int mais_leve(int capacidadeDaMochila, vector<Item>& itens) {
    sort(itens.begin(), itens.end(), compararPorPeso);  // Ordena pelo peso
    int valorTotal = 0, pesoAtual = 0;
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidadeDaMochila) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

int main(int argc, char* argv[]) {
    // Verifica se o arquivo foi passado como argumento
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <arquivo de entrada>" << endl;
        return 1;
    }

    // Abre o arquivo de entrada
    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    int N, capacidadeDaMochila;
    arquivoEntrada >> N >> capacidadeDaMochila;

    vector<Item> itens(N);
    for (int i = 0; i < N; ++i) {
        arquivoEntrada >> itens[i].peso >> itens[i].valor;
    }

    // Executa e imprime o resultado das duas heurísticas
    cout << "Mais caro primeiro: " << mais_caro(capacidadeDaMochila, itens) << endl;
    cout << "Mais leve primeiro: " << mais_leve(capacidadeDaMochila, itens) << endl;

    return 0;
}
