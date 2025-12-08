#include <iostream>
#include <vector>

enum Color { RED, BLACK };

struct RBNode {
    double key;
    Color color;
    RBNode *left, *right, *parent;

    RBNode(double k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    RBNode* root;
    RBNode* NIL; // Nó sentinela

    // Rotações
    void leftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        if (x->right != NIL) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    // Balanceamento de Inserção
    void insertFixup(RBNode* k) {
        while (k->parent && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                RBNode* u = k->parent->parent->right; // Tio
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            } else {
                RBNode* u = k->parent->parent->left;
                if (u->color == RED) {
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Auxiliares de Remoção
    void transplant(RBNode* u, RBNode* v) {
        if(u->parent == nullptr) root = v;
        else if(u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    RBNode* minValueNode(RBNode* node) {
        RBNode* current = node;
        while (current->left != NIL) current = current->left;
        return current;
    }

    void deleteNodeHelper(RBNode* node, double key) {
        RBNode* z = NIL;
        RBNode* curr = root;
        while(curr != NIL){
            if(curr->key == key){ z = curr; break; }
            if(curr->key < key) curr = curr->right;
            else curr = curr->left;
        }

        if(z == NIL) return; 

        RBNode* y = z;
        RBNode* x;
        
        // Remoção lógica BST
        if(z->left == NIL){
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL){
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minValueNode(z->right);
            x = y->right;
            if(y->parent == z) x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
    }

    // Auxiliares Estatísticos
    void inorderToList(RBNode* node, std::vector<double>& list) {
        if (node != NIL) {
            inorderToList(node->left, list);
            list.push_back(node->key);
            inorderToList(node->right, list);
        }
    }

    void rangeQueryRec(RBNode* node, double x, double y, std::vector<double>& res) {
        if (node == NIL) return;
        if (x < node->key) rangeQueryRec(node->left, x, y, res);
        if (node->key >= x && node->key <= y) res.push_back(node->key);
        if (y > node->key) rangeQueryRec(node->right, x, y, res);
    }

public:
    RBTree() {
        NIL = new RBNode(0);
        NIL->color = BLACK;
        root = NIL;
    }

    void insert(double key) {
        RBNode* z = new RBNode(key);
        RBNode* y = nullptr;
        RBNode* x = root;

        while (x != NIL) {
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == nullptr) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;

        z->left = NIL;
        z->right = NIL;
        z->color = RED;
        insertFixup(z);
    }

    void remove(double key) {
        deleteNodeHelper(root, key);
    }

    void printSorted() {
        std::vector<double> list;
        inorderToList(root, list);
        std::cout << "RB Ordenada: ";
        for(auto v : list) std::cout << v << " ";
        std::cout << std::endl;
    }

    double median() {
        std::vector<double> list;
        inorderToList(root, list);
        if (list.empty()) return 0;
        int n = list.size();
        if (n % 2 != 0) return list[n/2];
        else return (list[n/2 - 1] + list[n/2]) / 2.0;
    }
    
    std::vector<double> min(int n) {
        std::vector<double> list;
        inorderToList(root, list);
        std::vector<double> res;
        for(int i=0; i<n && i<list.size(); i++) res.push_back(list[i]);
        return res;
    }

    std::vector<double> max(int n) {
        std::vector<double> list;
        inorderToList(root, list);
        std::vector<double> res;
        for(int i=0; i<n && i<list.size(); i++) res.push_back(list[list.size() - 1 - i]);
        return res;
    }

    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> res;
        rangeQueryRec(root, x, y, res);
        return res;
    }
};

// Main para Teste
int main() {
    RBTree rb;
    
    // Testes Básicos
    rb.insert(15.0);
    rb.insert(10.0);
    rb.insert(20.0);
    rb.insert(10.0); // Duplicata
    rb.insert(30.0);
    rb.insert(5.0);

    std::cout << "--- TESTE RUBRO-NEGRA ---" << std::endl;
    rb.printSorted();
    
    std::cout << "Mediana: " << rb.median() << std::endl;
    
    std::vector<double> range = rb.rangeQuery(10.0, 25.0);
    std::cout << "Range [10, 25]: ";
    for(double v : range) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "Removendo 15.0..." << std::endl;
    rb.remove(15.0);
    rb.printSorted();

    return 0;
}