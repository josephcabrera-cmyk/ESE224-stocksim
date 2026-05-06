#include "StockBST.h"
#include <iostream>
#include <algorithm>

// Constructor
StockBST::StockBST() 
    : root(nullptr) {}

// Destructor
StockBST::~StockBST() {
    clear();
}

StockBST::BSTNode* StockBST::insertHelper(BSTNode* node, const string& ticker, double key, int year) {
    if (node == nullptr) {
        return new BSTNode(ticker, key, year);
    }

    if (key < node->key) {
        node->left = insertHelper(node->left, ticker, key, year);
    }
    else {
    node->right = insertHelper(node->right, ticker, key, year);   // duplicates go right per the invariant
    }

return node;
}

// Insert a new entry. Duplicates allowed (go to right subtree).
void StockBST::insert(const string& ticker, double key, int year) {
    root = insertHelper(root, ticker, key, year);
}

StockBST::BSTNode* StockBST::searchHelper(BSTNode* node, double key) const {
    if (node == nullptr) {
        return node;
    }

    if (key < node->key) {
        return searchHelper (node->left, key);
    }
    else if (key > node->key) {
        return searchHelper (node->right, key);
    }

    return node;
}

// Return pointer to a node whose key exactly matches. nullptr if not found.
StockBST::BSTNode* StockBST::search(double key) const {
    return searchHelper(root, key);
}

void StockBST::rangeSearchHelper(BSTNode* node, double low, double high, vector<BSTNode*>& results) const {
    if (node == nullptr) {
        return;
    }

    if (node->key > low) {
        rangeSearchHelper(node->left, low, high, results);
    }

    if (low <= node->key && node->key <= high) {
        results.push_back(node);
    }

    if (node->key < high) {
        rangeSearchHelper(node->right, low, high, results);
    }
}

// Collect all nodes where low <= key <= high into 'results' vector.
void StockBST::rangeSearch(double low, double high, vector<BSTNode*>& results) const {
    rangeSearchHelper(root, low, high, results);
}

void StockBST::inorderHelper(BSTNode* node)  const {
    if (node == nullptr) {
        return;
    }

    inorderHelper (node->left);
    cout << node->ticker << " (" << node->key << ")" << endl;

    inorderHelper (node->right);
}

// Tree traversals — each prints: ticker (key) per line
void StockBST::inorder()   const {
    inorderHelper(root);
}

void StockBST::preorderHelper(BSTNode* node) const {
    if (node == nullptr) {
        return;
    }

    cout << node->ticker << " (" << node->key << ")" << endl;

    preorderHelper (node->left);

    preorderHelper (node->right);
}

void StockBST::preorder()  const {
    preorderHelper(root);
}

void StockBST::postorderHelper(BSTNode* node) const {
    if (node == nullptr) {
        return;
    }

    postorderHelper (node->left);

    postorderHelper (node->right);

    cout << node->ticker << " (" << node->key << ")" << endl;
}

void StockBST::postorder() const {
    postorderHelper(root);
}

int StockBST::heightHelper(BSTNode* node) const {
    if (node == nullptr) {
        return -1;
    }   // -1 if empty

    int leftHeight = heightHelper(node->left);

    int rightHeight = heightHelper(node->right);

    return 1 + max(leftHeight, rightHeight);
}

// Returns the number of edges on the longest root-to-leaf path. -1 if empty.
int StockBST::getHeight() const {
    return heightHelper(root);
}

// Finds and returns the node with the maximum key (rightmost node).
// Returns nullptr if tree is empty.

StockBST::BSTNode* StockBST::findMax() const {
    if (root == nullptr) {
        return nullptr;
    }

    BSTNode* node = root;
    while (node->right != nullptr) {
        node = node->right;
    }

    return node;
}

void StockBST::clearHelper(BSTNode* node) {
    if (node == nullptr) {
        return;
    }

    clearHelper (node->left);

    clearHelper (node->right);

    delete node;
}

void StockBST::clear() {
    clearHelper(root);
    root = nullptr;
}