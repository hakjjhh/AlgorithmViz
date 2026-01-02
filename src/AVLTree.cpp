#include "../include/AVLTree.h"  

int AVLTree::getHeight(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// 右旋 (LL情况)

Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // 旋转
    x->right = y;
    y->left = T2;

    // 更新高度
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    // 返回新的根
    return x;
}

// 左旋 (RR情况)
Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // 旋转
    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// 插入函数（递归 + 自动平衡）
Node* AVLTree::insertNode(Node* node, int key) {
    // 1. 标准 BST 插入
    if (node == nullptr) return new Node(key);

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node; // 不允许重复键值

    // 2. 更新高度
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // 3. 获取平衡因子
    int balance = getBalance(node);

    // 4. 平衡维护 (四种情况)

    // LL型 -> 右旋
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR型 -> 左旋
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR型 -> 先左后右
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL型 -> 先右后左
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// 公开的插入接口
void AVLTree::insert(int key) {
    root = insertNode(root, key);
}