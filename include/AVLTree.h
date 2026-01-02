#pragma once
#include <algorithm> // 用于 std::max

// 引入 EasyX 图形库 (因为节点里虽然没用图形类型，但设计逻辑相关)
#include <graphics.h>

// AVL 树节点定义
struct Node {
    int key;        // 节点值
    Node* left;
    Node* right;
    int height;     // 高度

    // 可视化坐标
    int x, y;

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1), x(0), y(0) {}
};

// AVL 树类声明
class AVLTree {
private:
    Node* root;

    // --- 内部算法辅助函数 ---
    int getHeight(Node* node);
    int getBalance(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* insertNode(Node* node, int key);

    // --- 内部可视化辅助函数 ---
    void updateCoordinates(Node* node, int x, int y, int offset);
    void drawLines(Node* node);
    void drawNodes(Node* node);

public:
    AVLTree();  // 构造函数

    void insert(int key); // 插入接口
    void draw();          // 绘制接口
};