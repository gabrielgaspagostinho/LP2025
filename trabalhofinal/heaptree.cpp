#include <iostream>
#include <vector>
#include <algorithm> // Usado apenas para ordenar cópias temporárias (funções min/max/median)
#include <cmath>

class PureMinHeap {
private:
    std::vector<double> heap;

    // Pa Navegação
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    // Lógica Core da Heap
    
    // Sobe o elemento (Bubble Up)
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // Desce o elemento (Bubble Down)
    void siftDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        int r = rightChild(i);
        int n = heap.size();

        if (l < n && heap[l] < heap[minIndex])
            minIndex = l;

        if (r < n && heap[r] < heap[minIndex])
            minIndex = r;

        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

public:
    // Inserção: O(log N)
    void insert(double value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    // Remoção Arbitrária: O(N)
    // Heaps não são feitas para busca. Precisamos procurar linearmente.
    void remove(double value) {
        int index = -1;
        // 1. Busca Linear (O(N))
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i] == value) {
                index = i;
                break;
            }
        }

        if (index == -1) return; // Valor não encontrado

        // 2. Substituir pelo último elemento
        heap[index] = heap.back();
        heap.pop_back();

        // 3. Restaurar propriedade da Heap
        // O elemento movido pode precisar subir OU descer
        if (index < heap.size()) {
            siftDown(index);
            siftUp(index);
        }
    }

    // Operações "Caras" (Necessitam ordenar uma cópia)

    // O(N log N) - Copia e ordena
    void printSorted() {
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());

        std::cout << "Heap (Ordenada temp): ";
        for (double val : temp) std::cout << val << " ";
        std::cout << std::endl;
    }

    // O(N log N) - Copia, ordena e pega o meio
    double median() {
        if (heap.empty()) return 0.0;

        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());

        int n = temp.size();
        if (n % 2 != 0)
            return temp[n / 2];
        else
            return (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    }

    // O(N log N) - Ordena e pega os primeiros
    // Nota: Min(1) é O(1) na Heap, mas Min(k) exige extração ou ordenação.
    std::vector<double> min(int n) {
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());
        
        std::vector<double> result;
        for(int i=0; i<n && i<temp.size(); i++) {
            result.push_back(temp[i]);
        }
        return result;
    }

    // O(N log N) - Ordena e pega os últimos
    std::vector<double> max(int n) {
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end(), std::greater<double>());
        
        std::vector<double> result;
        for(int i=0; i<n && i<temp.size(); i++) {
            result.push_back(temp[i]);
        }
        return result;
    }

    // Range Query: O(N)
    // Precisamos visitar todos os nós, pois a Heap não ordena horizontalmente
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : heap) {
            if (val >= x && val <= y) {
                result.push_back(val);
            }
        }
        // Opcional: ordenar o resultado da query
        std::sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    PureMinHeap sensor;

    sensor.insert(25.5);
    sensor.insert(30.0);
    sensor.insert(22.1);
    sensor.insert(10.0); // Novo mínimo
    sensor.insert(25.5); // Duplicata
    sensor.insert(40.0);

    sensor.printSorted();

    std::cout << "Mediana: " << sensor.median() << std::endl;

    std::vector<double> range = sensor.rangeQuery(20.0, 30.0);
    std::cout << "Range [20, 30]: ";
    for(double v : range) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "Removendo 22.1..." << std::endl;
    sensor.remove(22.1);
    sensor.printSorted();

    return 0;
}