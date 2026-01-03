#pragma once
#include <algorithm>
#include <graphics.h>
#include <string>

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int x, y;

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1), x(0), y(0) {}
};

class AVLTree {
private:
    Node* root;
    std::wstring lastLog; // 记录最后一条操作日志

    // 辅助函数
    int getHeight(Node* node);
    int getBalance(Node* node);
    Node* minValueNode(Node* node);

    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* insertNode(Node* node, int key);
    Node* deleteNode(Node* root, int key);
    bool searchNode(Node* node, int key);

    void updateCoordinates(Node* node, int x, int y, int offset);
    void drawLines(Node* node);
    void drawNodes(Node* node);
    Node* getNodeAt(Node* node, int mx, int my);

public:
    AVLTree();
    void insert(int key);
    void remove(int key);
    bool search(int key);
    void inOrderTraversal(std::wstring& result); // 中序遍历接口
    void preOrderTraversal(std::wstring& result);  // 前序
    void postOrderTraversal(std::wstring& result); // 后序
    int getNodeCount();                          // 获取节点数
    int getTreeHeight();                         // 获取树高

    void draw();
    Node* findClickedNode(int x, int y);
    Node* getRoot();

    // 获取日志用于状态栏显示
    const std::wstring& getLog() { return lastLog; }
};