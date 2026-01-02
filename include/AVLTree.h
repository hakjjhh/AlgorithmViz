#pragma once
#include <algorithm> // 用于 std::max

// AVL 树节点定义
struct Node {
    int key;        // 节点保存的数字
    Node* left;
    Node* right;
    int height;     // 当前高度

    // --- 可视化专用属性 ---
    int x, y;       // 目标绘制位置

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1), x(0), y(0) {}
};

// AVL 树类定义
class AVLTree {
public:
    Node* root; // 根节点

    AVLTree() : root(nullptr) {}

    // 核心操作（今天只写这些）
    void insert(int key);   // 插入数字

    // 辅助函数
    int getHeight(Node* node);
    int getBalance(Node* node);

    // 四大旋转（AVL的灵魂）
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);

    // 内部递归插入函数
    Node* insertNode(Node* node, int key);
};
