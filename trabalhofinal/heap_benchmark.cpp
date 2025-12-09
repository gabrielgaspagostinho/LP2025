// Bibliotecas que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

// Heap mínimo simples
class PureMinHeap {
private:
    std::vector<double> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    // Sobe o elemento até o lugar certo
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // Desce o elemento até o lugar certo
    void siftDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        int r = rightChild(i);
        int n = heap.size();

        if (l < n && heap[l] < heap[minIndex]) minIndex = l;
        if (r < n && heap[r] < heap[minIndex]) minIndex = r;

        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

public:
    // Adiciona valor no heap
    void insert(double value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    // Remove valor do heap (busca linear)
    void remove(double value) {
        int index = -1;
        // Busca linear O(N) - não é eficiente
        for (size_t i = 0; i < heap.size(); i++) {
            if (heap[i] == value) {
                index = i;
                break;
            }
        }
        if (index == -1) return;

        heap[index] = heap.back();
        heap.pop_back();

        if (index < (int)heap.size()) {
            siftDown(index);
            siftUp(index);
        }
    }

    // Gera uma cópia ordenada do heap
    std::vector<double> getSortedCopy() {
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());
        return temp;
    }

    // Calcula a mediana
    double median() {
        if (heap.empty()) return 0.0;
        std::vector<double> temp = getSortedCopy(); // Custo alto: Sort
        int n = temp.size();
        if (n % 2 != 0) return temp[n / 2];
        else return (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    }

    // Retorna os n menores valores
    std::vector<double> min(int n) {
        std::vector<double> temp = getSortedCopy();
        std::vector<double> result;
        for(int i=0; i<n && i<(int)temp.size(); i++) result.push_back(temp[i]);
        return result;
    }

    // Retorna os n maiores valores
    std::vector<double> max(int n) {
        std::vector<double> temp = getSortedCopy();
        std::vector<double> result;
        for(int i=0; i<n && i<(int)temp.size(); i++) 
            result.push_back(temp[temp.size() - 1 - i]);
        return result;
    }

    // Busca todos os valores no intervalo [x, y]
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : heap) {
            if (val >= x && val <= y) result.push_back(val);
        }
        std::sort(result.begin(), result.end()); // Só pra ficar bonitinho
        return result;
    }

    // Verifica se o heap está vazio
    bool isEmpty() { return heap.empty(); }
};

int main() {
    PureMinHeap sensor;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir " << filename << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "=== BENCHMARK: MIN HEAP ===" << std::endl;

    // 1. Inserção dos dados
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        try {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            sensor.insert(std::stod(line));
            count++;
        } catch (...) {}
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_insert = end - start;
    std::cout << "[INSERT] " << count << " registros | Tempo: " << t_insert.count() << " ms" << std::endl;
    file.close();

    if (sensor.isEmpty()) return 0;

    // 2. Mediana
    start = std::chrono::high_resolution_clock::now();
    double med = sensor.median();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_median = end - start;
    std::cout << "[MEDIAN] Resultado: " << med << " | Tempo: " << t_median.count() << " ms" << std::endl;

    // 3. Menores valores (3 menores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> mins = sensor.min(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_min = end - start;
    std::cout << "[MIN(3)] "; 
    for(auto v : mins) std::cout << v << " ";
    std::cout << "| Tempo: " << t_min.count() << " ms" << std::endl;

    // 4. Maiores valores (3 maiores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> maxs = sensor.max(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_max = end - start;
    std::cout << "[MAX(3)] "; 
    for(auto v : maxs) std::cout << v << " ";
    std::cout << "| Tempo: " << t_max.count() << " ms" << std::endl;

    // 5. Consulta por intervalo (exemplo: 20.0 até 25.0)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> range = sensor.rangeQuery(20.0, 25.0);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_range = end - start;
    std::cout << "[RANGE(20, 25)] Encontrados: " << range.size() << " itens | Tempo: " << t_range.count() << " ms" << std::endl;

    // 6. Remover valor (exemplo: 22.5)
    start = std::chrono::high_resolution_clock::now();
    sensor.remove(22.5);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_remove = end - start;
    std::cout << "[REMOVE(22.5)] Tempo: " << t_remove.count() << " ms" << std::endl;

    // 7. Gera lista ordenada (simula print)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> sortedList = sensor.getSortedCopy();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_sort = end - start;
    std::cout << "[PRINTSORTED] Gerada lista de " << sortedList.size() << " itens | Tempo: " << t_sort.count() << " ms" << std::endl;

    return 0;
}