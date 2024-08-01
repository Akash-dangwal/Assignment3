#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int k, h, sz;
    Node *l, *r;
    Node(int key) : k(key), h(1), sz(1), l(nullptr), r(nullptr) {}
};

class AVL {
    Node *root;
    
    int height(Node *n) {
        return n ? n->h : 0;
    }
    
    int size(Node *n) {
        return n ? n->sz : 0;
    }
    
    int balance(Node *n) {
        return height(n->l) - height(n->r);
    }
    
    Node* rotateLeft(Node *n) {
        Node *r = n->r;
        n->r = r->l;
        r->l = n;
        n->h = max(height(n->l), height(n->r)) + 1;
        r->h = max(height(r->l), height(r->r)) + 1;
        n->sz = size(n->l) + size(n->r) + 1;
        r->sz = size(r->l) + size(r->r) + 1;
        return r;
    }
    
    Node* rotateRight(Node *n) {
        Node *l = n->l;
        n->l = l->r;
        l->r = n;
        n->h = max(height(n->l), height(n->r)) + 1;
        l->h = max(height(l->l), height(l->r)) + 1;
        n->sz = size(n->l) + size(n->r) + 1;
        l->sz = size(l->l) + size(l->r) + 1;
        return l;
    }
    
    Node* balanceNode(Node *n) {
        if (balance(n) > 1) {
            if (balance(n->l) < 0) {
                n->l = rotateLeft(n->l);
            }
            return rotateRight(n);
        }
        if (balance(n) < -1) {
            if (balance(n->r) > 0) {
                n->r = rotateRight(n->r);
            }
            return rotateLeft(n);
        }
        return n;
    }
    
    Node* insertNode(Node *n, int key) {
        if (!n) return new Node(key);
        if (key < n->k) n->l = insertNode(n->l, key);
        else if (key > n->k) n->r = insertNode(n->r, key);
        n->h = max(height(n->l), height(n->r)) + 1;
        n->sz = size(n->l) + size(n->r) + 1;
        return balanceNode(n);
    }
    
    Node* minNode(Node *n) {
        while (n->l) n = n->l;
        return n;
    }
    
    Node* removeNode(Node *n, int key) {
        if (!n) return nullptr;
        if (key < n->k) n->l = removeNode(n->l, key);
        else if (key > n->k) n->r = removeNode(n->r, key);
        else {
            if (!n->l) return n->r;
            if (!n->r) return n->l;
            Node *min = minNode(n->r);
            n->k = min->k;
            n->r = removeNode(n->r, min->k);
        }
        n->h = max(height(n->l), height(n->r)) + 1;
        n->sz = size(n->l) + size(n->r) + 1;
        return balanceNode(n);
    }
    
    bool findNode(Node *n, int key) {
        if (!n) return false;
        if (key < n->k) return findNode(n->l, key);
        if (key > n->k) return findNode(n->r, key);
        return true;
    }
    
    int order(Node *n, int key) {
        if (!n) return 0;
        if (key < n->k) return order(n->l, key);
        return size(n->l) + 1 + order(n->r, key);
    }
    
    int getByOrder(Node *n, int k) {
        if (!n) return -1;
        int lSize = size(n->l);
        if (k <= lSize) return getByOrder(n->l, k);
        if (k == lSize + 1) return n->k;
        return getByOrder(n->r, k - lSize - 1);
    }
    
public:
    AVL() : root(nullptr) {}
    
    bool find(int key) {
        return findNode(root, key);
    }
    
    void insert(int key) {
        root = insertNode(root, key);
    }
    
    void remove(int key) {
        root = removeNode(root, key);
    }
    
    int orderOfKey(int key) {
        return order(root, key);
    }
    
    int getByOrder(int k) {
        return getByOrder(root, k);
    }
};

int main() {
    AVL avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.insert(15);

    cout << "Find 20: " << avl.find(20) << endl;
    cout << "Find 25: " << avl.find(25) << endl;
    cout << "Order of 15: " << avl.orderOfKey(15) << endl;
    cout << "Get by order 2: " << avl.getByOrder(2) << endl;

    avl.remove(20);
    cout << "Find 20 after removal: " << avl.find(20) << endl;

    return 0;
}