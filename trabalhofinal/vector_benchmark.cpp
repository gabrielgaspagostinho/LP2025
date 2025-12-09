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

    // Calcula a mediana (faz o sort se precisar)
    double median() {
        if (data.empty()) return 0.0;
        if (!sorted) runInsertionSort();

        int n = data.size();
        if (n % 2 != 0) return data[n / 2];
        else return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }

    // Busca todos os valores no intervalo [x, y]
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : data) {
            if (val >= x && val <= y) {
                result.push_back(val);
            }
        }
        return result;
    }

    // Retorna cópia ordenada do vetor
    std::vector<double> getSortedList() {
        if (!sorted) runInsertionSort();
        return data;
    }

    // Retorna os n menores valores
    std::vector<double> min(int n) {
        if (!sorted) runInsertionSort();
        std::vector<double> res;
        for(int i=0; i<n && i<(int)data.size(); i++) res.push_back(data[i]);
        return res;
    }

    // Retorna os n maiores valores
    std::vector<double> max(int n) {
        if (!sorted) runInsertionSort();
        std::vector<double> res;
        int sz = data.size();
        for(int i=0; i<n && i<sz; i++) res.push_back(data[sz - 1 - i]);
        return res;
    }

    // Verifica se o vetor está vazio
    bool isEmpty() { return data.empty(); }
};

int main() {
    VectorInsertionSort vec;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir " << filename << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "=== BENCHMARK: VECTOR INSERTION SORT (O(N^2)) ===" << std::endl;

    // 1. Inserção dos dados (super rápido)
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        try {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            vec.insert(std::stod(line));
            count++;
        } catch (...) {}
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_insert = end - start;
    std::cout << "[INSERT] " << count << " registros | Tempo: " << t_insert.count() << " ms" << std::endl;
    file.close();

    if (vec.isEmpty()) return 0;

    // 2. Mediana (aqui faz o sort O(N^2))
    start = std::chrono::high_resolution_clock::now();
    double med = vec.median();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_median = end - start;
    std::cout << "[MEDIAN] Resultado: " << med << " | Tempo (inclui Sort): " << t_median.count() << " ms" << std::endl;

    // 3. Menores valores (3 menores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> mins = vec.min(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_min = end - start;
    std::cout << "[MIN(3)] "; 
    for(auto v : mins) std::cout << v << " ";
    std::cout << "| Tempo: " << t_min.count() << " ms" << std::endl;

    // 4. Maiores valores (3 maiores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> maxs = vec.max(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_max = end - start;
    std::cout << "[MAX(3)] "; 
    for(auto v : maxs) std::cout << v << " ";
    std::cout << "| Tempo: " << t_max.count() << " ms" << std::endl;

    // 5. Consulta por intervalo (busca linear)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> range = vec.rangeQuery(20.0, 25.0);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_range = end - start;
    std::cout << "[RANGE(20, 25)] Encontrados: " << range.size() << " itens | Tempo: " << t_range.count() << " ms" << std::endl;

    // 6. Remover valor (exemplo: 22.5)
    start = std::chrono::high_resolution_clock::now();
    vec.remove(22.5);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_remove = end - start;
    std::cout << "[REMOVE(22.5)] Tempo: " << t_remove.count() << " ms" << std::endl;

    // 7. Gera lista ordenada (já está ordenado)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> sortedList = vec.getSortedList();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_sort = end - start;
    std::cout << "[PRINTSORTED] Lista de " << sortedList.size() << " itens | Tempo: " << t_sort.count() << " ms" << std::endl;

    return 0;
}