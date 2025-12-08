#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono> // Para medir o tempo
#include <iomanip>

// ==========================================
// 1. CLASSE HEAP PURA (Min-Heap)
// ==========================================
class PureMinHeap {
private:
    std::vector<double> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

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
    void insert(double value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    void remove(double value) {
        int index = -1;
        // Busca linear O(N)
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

    // O(N log N) - Precisa ordenar cópia
    double median() {
        if (heap.empty()) return 0.0;
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());
        int n = temp.size();
        if (n % 2 != 0) return temp[n / 2];
        else return (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    }

    // O(N) - Busca linear completa
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : heap) {
            if (val >= x && val <= y) result.push_back(val);
        }
        return result;
    }
};

// ==========================================
// 2. CLASSE AVL PURA
// ==========================================
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

    // O(N) - Percorre árvore
    double median() {
        std::vector<double> list;
        inorderToList(root, list);
        if (list.empty()) return 0.0;
        int n = list.size();
        if (n % 2 != 0) return list[n / 2];
        return (list[n / 2 - 1] + list[n / 2]) / 2.0;
    }

    // O(N) Otimizado (poda ramos inúteis)
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> res;
        rangeQueryRec(root, x, y, res);
        return res;
    }
};

// ==========================================
// 3. BENCHMARK
// ==========================================

// Função auxiliar para leitura
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
            try {
                dados.push_back(std::stod(linha));
            } catch (...) {}
        }
    }
    return dados;
}

int main() {
    std::vector<double> dataset = lerCSV("temperaturas.csv");
    if (dataset.empty()) {
        std::cout << "Gere o CSV primeiro com o script Python!" << std::endl;
        return 1;
    }

    std::cout << "Dados carregados: " << dataset.size() << " entradas." << std::endl;
    std::cout << std::fixed << std::setprecision(2);

    PureMinHeap heap;
    AVLTree avl;

    using namespace std::chrono;

    // --- TESTE 1: INSERÇÃO (1000 itens) ---
    auto start = high_resolution_clock::now();
    for (double val : dataset) heap.insert(val);
    auto end = high_resolution_clock::now();
    auto timeHeapInsert = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : dataset) avl.insert(val);
    end = high_resolution_clock::now();
    auto timeAvlInsert = duration_cast<microseconds>(end - start).count();

    // --- TESTE 2: MEDIANA ---
    start = high_resolution_clock::now();
    double medHeap = heap.median();
    end = high_resolution_clock::now();
    auto timeHeapMedian = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    double medAvl = avl.median();
    end = high_resolution_clock::now();
    auto timeAvlMedian = duration_cast<microseconds>(end - start).count();

    // --- TESTE 3: RANGE QUERY (20.0 a 30.0) ---
    start = high_resolution_clock::now();
    auto resHeap = heap.rangeQuery(20.0, 30.0);
    end = high_resolution_clock::now();
    auto timeHeapRange = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    auto resAvl = avl.rangeQuery(20.0, 30.0);
    end = high_resolution_clock::now();
    auto timeAvlRange = duration_cast<microseconds>(end - start).count();

    // --- TESTE 4: REMOÇÃO (Primeiros 100 itens do CSV) ---
    // Clonamos os dados para não afetar testes futuros se houvesse, mas aqui é o fim.
    std::vector<double> toRemove;
    for(int i=0; i<100; i++) toRemove.push_back(dataset[i]);

    start = high_resolution_clock::now();
    for (double val : toRemove) heap.remove(val);
    end = high_resolution_clock::now();
    auto timeHeapRemove = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (double val : toRemove) avl.remove(val);
    end = high_resolution_clock::now();
    auto timeAvlRemove = duration_cast<microseconds>(end - start).count();

    // --- RESULTADOS ---
    std::cout << "\n=== RESULTADOS DO BENCHMARK (Microsegundos - us) ===\n";
    std::cout << std::left << std::setw(20) << "Operacao" 
            << std::setw(15) << "HEAP (Pura)" 
            << std::setw(15) << "AVL (Padrao)" 
            << "Vencedor" << std::endl;
    std::cout << "------------------------------------------------------------\n";
    
    std::cout << std::left << std::setw(20) << "Insert (1000x)" 
            << std::setw(15) << timeHeapInsert 
            << std::setw(15) << timeAvlInsert 
            << (timeHeapInsert < timeAvlInsert ? "HEAP" : "AVL") << std::endl;

    std::cout << std::left << std::setw(20) << "Median Calc" 
            << std::setw(15) << timeHeapMedian 
            << std::setw(15) << timeAvlMedian 
            << (timeHeapMedian < timeAvlMedian ? "HEAP" : "AVL") << std::endl;

    std::cout << std::left << std::setw(20) << "Range Query" 
            << std::setw(15) << timeHeapRange 
            << std::setw(15) << timeAvlRange 
            << (timeHeapRange < timeAvlRange ? "HEAP" : "AVL") << std::endl;

    std::cout << std::left << std::setw(20) << "Remove (100x)" 
            << std::setw(15) << timeHeapRemove 
            << std::setw(15) << timeAvlRemove 
            << (timeHeapRemove < timeAvlRemove ? "HEAP" : "AVL") << std::endl;

    std::cout << "\nNota(com base nos conhecimentos sobre as arvores): \n";
    std::cout << "- HEAP vence em insercao (log N simples vs log N com rebalanceamento).\n";
    std::cout << "- AVL vence em remocao e range (log N vs linear).\n";
    std::cout << "- Mediana: AVL O(N) vs Heap O(N log N) (Devido a ordenacao necessaria).\n";

    return 0;

}
