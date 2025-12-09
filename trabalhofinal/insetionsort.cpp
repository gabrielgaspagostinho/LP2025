// Bibliotecas que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

// Classe do vetor com Insertion Sort
class VectorInsertionSort {
private:
    std::vector<double> data;
    bool sorted = false;

    // Insertion Sort (bem lento pra muitos dados)
    void runInsertionSort() {
        int n = data.size();
        for (int i = 1; i < n; i++) {
            double key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
        sorted = true;
    }

public:
    // Adiciona valor (super rápido)
    void insert(double value) {
        data.push_back(value);
        sorted = false; 
    }

    // Remove valor (O(N))
    void remove(double value) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == value) {
                data.erase(it);
                return;
            }
        }
    }

    // Imprime os valores em ordem (faz o sort se precisar)
    void printSorted() {
        if (!sorted) runInsertionSort();
        std::cout << "Temperaturas Ordenadas (Vector Insertion Sort): ";
        for (double val : data) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Verifica se o vetor está vazio
    bool isEmpty() { return data.empty(); }
};

int main() {
    VectorInsertionSort vecSort;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << filename << "'." << std::endl;
        return 1;
    }

    std::cout << "Lendo arquivo e jogando no vetor..." << std::endl;

    // Marca o tempo de início
    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(file, line)) {
        try {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            vecSort.insert(std::stod(line));
            count++;
        } catch (...) { continue; }
    }

    // Só ordena e imprime aqui
    if (!vecSort.isEmpty()) {
        vecSort.printSorted();
    } else {
        std::cout << "Nenhuma temperatura valida encontrada." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    
    std::cout << "\n------------------------------------------------" << std::endl;
    std::cout << "Estatísticas:" << std::endl;
    std::cout << "Total de registros: " << count << std::endl;
    std::cout << "Tempo Total (Leitura + Sort O(N^2) + Print): " << duration.count() << " ms" << std::endl;

    return 0;
}