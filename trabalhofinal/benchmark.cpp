// Bibliotecas que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

// 1. Heap Minimo simples
class PureMinHeap {
private:
    std::vector<double> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    // Sobe o elemento até ficar no lugar certo
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // Desce o elemento até ficar no lugar certo
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

    // Remove valor do heap (se existir)
    void remove(double value) {
        int index = -1;
        for (size_t i = 0; i < heap.size(); i++) {
            if (heap[i] == value) { index = i; break; }
        }
        if (index == -1) return;
        heap[index] = heap.back();
        heap.pop_back();
        if (index < heap.size()) {
            siftDown(index);
            siftUp(index);
        }
    }

    // Calcula a mediana
    double median() {
        if (heap.empty()) return 0.0;
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());
        int n = temp.size();
        if (n % 2 != 0) return temp[n / 2];
        else return (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    }

    // Busca todos os valores no intervalo
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : heap) {
            if (val >= x && val <= y) result.push_back(val);
        }
        return result;
    }
};

// 2. AVL Tree
struct AVLNode {
    double key;
    AVLNode *left, *right;
    int height;
    AVLNode(double k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* N) { return (N == nullptr) ? 0 : N->height; }
    int getBalance(AVLNode* N) { return (N == nullptr) ? 0 : height(N->left) - height(N->right); }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y; y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x; x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode* insertRec(AVLNode* node, double key) {
        if (node == nullptr) return new AVLNode(key);
        if (key < node->key) node->left = insertRec(node->left, key);
        else node->right = insertRec(node->right, key);

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key >= node->right->key) return leftRotate(node);
        if (balance > 1 && key >= node->left->key) {
            node->left = leftRotate(node->left); return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right); return leftRotate(node);
        }
        return node;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left != nullptr) current = current->left;
        return current;
    }

    AVLNode* removeRec(AVLNode* root, double key) {
        if (root == nullptr) return root;
        if (key < root->key) root->left = removeRec(root->left, key);
        else if (key > root->key) root->right = removeRec(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) { temp = root; root = nullptr; }
                else *root = *temp;
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = removeRec(root->right, temp->key);
            }
        }
        if (root == nullptr) return root;
        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = getBalance(root);
        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) { root->left = leftRotate(root->left); return rightRotate(root); }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) { root->right = rightRotate(root->right); return leftRotate(root); }
        return root;
    }

    void inorderToList(AVLNode* root, std::vector<double>& list) {
        if (root != nullptr) {
            inorderToList(root->left, list);
            list.push_back(root->key);
            inorderToList(root->right, list);
        }
    }

    void rangeQueryRec(AVLNode* node, double x, double y, std::vector<double>& res) {
        if (node == nullptr) return;
        if (x < node->key) rangeQueryRec(node->left, x, y, res);
        if (node->key >= x && node->key <= y) res.push_back(node->key);
        if (y > node->key) rangeQueryRec(node->right, x, y, res);
    }

public:
    AVLTree() : root(nullptr) {}
    void insert(double value) { root = insertRec(root, value); }
    void remove(double value) { root = removeRec(root, value); }

    double median() {
        std::vector<double> list;
        inorderToList(root, list);
        if (list.empty()) return 0.0;
        int n = list.size();
        if (n % 2 != 0) return list[n / 2];
        return (list[n / 2 - 1] + list[n / 2]) / 2.0;
    }

    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> res;
        rangeQueryRec(root, x, y, res);
        return res;
    }
};

// 3. Vector + Insertion Sort
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

    // Mediana (fica lento se não estiver ordenado)
    double median() {
        if (data.empty()) return 0.0;
        if (!sorted) runInsertionSort();

        int n = data.size();
        if (n % 2 != 0) return data[n / 2];
        else return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }

    // Busca todos os valores no intervalo
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : data) {
            if (val >= x && val <= y) result.push_back(val);
        }
        return result;
    }
};

// Função pra ler o CSV
std::vector<double> lerCSV(const std::string& arquivo) {
    std::vector<double> dados;
    std::ifstream file(arquivo);
    std::string linha;
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo!" << std::endl;
        return dados;
    }
    while (std::getline(file, linha)) {
        if (!linha.empty()) {
            try { dados.push_back(std::stod(linha)); } catch (...) {}
        }
    }
    return dados;
}

int main() {
    std::vector<double> dataset = lerCSV("temperaturas.csv");
    if (dataset.empty()) {
        std::cout << "Gere o CSV primeiro!" << std::endl;
        return 1;
    }

    std::cout << "Dados carregados: " << dataset.size() << " entradas." << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    PureMinHeap heap;
    AVLTree avl;
    VectorInsertionSort vec;

    using namespace std::chrono;

    // Teste 1: Inserção
    auto start = high_resolution_clock::now();
    for (double val : dataset) heap.insert(val);
    auto end = high_resolution_clock::now();
    auto tHeapIns = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : dataset) avl.insert(val);
    end = high_resolution_clock::now();
    auto tAvlIns = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : dataset) vec.insert(val);
    end = high_resolution_clock::now();
    auto tVecIns = duration_cast<microseconds>(end - start).count();

    // Teste 2: Mediana
    start = high_resolution_clock::now();
    heap.median();
    end = high_resolution_clock::now();
    auto tHeapMed = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    avl.median();
    end = high_resolution_clock::now();
    auto tAvlMed = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    vec.median();
    end = high_resolution_clock::now();
    auto tVecMed = duration_cast<microseconds>(end - start).count();

    // Teste 3: Range Query
    start = high_resolution_clock::now();
    heap.rangeQuery(20.0, 30.0);
    end = high_resolution_clock::now();
    auto tHeapRng = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    avl.rangeQuery(20.0, 30.0);
    end = high_resolution_clock::now();
    auto tAvlRng = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    vec.rangeQuery(20.0, 30.0);
    end = high_resolution_clock::now();
    auto tVecRng = duration_cast<microseconds>(end - start).count();

    // Teste 4: Remoção (100 itens)
    std::vector<double> toRemove;
    for(int i=0; i<100; i++) toRemove.push_back(dataset[i]);

    start = high_resolution_clock::now();
    for (double val : toRemove) heap.remove(val);
    end = high_resolution_clock::now();
    auto tHeapRem = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : toRemove) avl.remove(val);
    end = high_resolution_clock::now();
    auto tAvlRem = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : toRemove) vec.remove(val);
    end = high_resolution_clock::now();
    auto tVecRem = duration_cast<microseconds>(end - start).count();

    // Resultados
    std::cout << "\n=== RESULTADOS DO BENCHMARK (Microsegundos - us) ===\n";
    std::cout << std::left << std::setw(15) << "Operacao" 
            << std::setw(12) << "HEAP" 
            << std::setw(12) << "AVL" 
            << std::setw(12) << "VEC(Ins)" 
            << "Vencedor" << std::endl;
    std::cout << "------------------------------------------------------------\n";
    
    // Função pra imprimir linha da tabela
    auto printRow = [](std::string op, long tH, long tA, long tV) {
        std::string winner;
        if (tH <= tA && tH <= tV) winner = "HEAP";
        else if (tA <= tH && tA <= tV) winner = "AVL";
        else winner = "VEC";

        std::cout << std::left << std::setw(15) << op 
                << std::setw(12) << tH 
                << std::setw(12) << tA 
                << std::setw(12) << tV 
                << winner << std::endl;
    };

    printRow("Insert (1000x)", tHeapIns, tAvlIns, tVecIns);
    printRow("Median Calc", tHeapMed, tAvlMed, tVecMed);
    printRow("Range Query", tHeapRng, tAvlRng, tVecRng);
    printRow("Remove (100x)", tHeapRem, tAvlRem, tVecRem);

    std::cout << "\nResumo rápido: \n";
    std::cout << "- Vector (Ins) ganha na inserção porque só adiciona no fim (O(1)).\n";
    std::cout << "- Vector é muito ruim pra mediana porque faz Insertion Sort O(N^2).\n";
    std::cout << "- AVL é muito boa pra range e remoção.\n";

    return 0;
}