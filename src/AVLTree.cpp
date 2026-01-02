#include "../include/AVLTree.h" 
#include <tchar.h> // 用于 _stprintf_s
#include <string>

// 构造函数
AVLTree::AVLTree() : root(nullptr) {}

// 获取高度
int AVLTree::getHeight(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

// 获取平衡因子
int AVLTree::getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// 右旋 (LL)
Node* AVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // 旋转
    x->right = y;
    y->left = T2;

    // 更新高度（注意：这里加了括号 (std::max) 以解决冲突）
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// 左旋 (RR)
Node* AVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // 旋转
    y->left = x;
    x->right = T2;

    // 更新高度（注意：这里加了括号 (std::max) 以解决冲突）
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// 递归插入实现
Node* AVLTree::insertNode(Node* node, int key) {
    // 1. 标准 BST 插入
    if (node == nullptr) return new Node(key);

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node; // 不允许重复

    // 2. 更新高度（注意：这里加了括号 (std::max) 以解决冲突）
    node->height = 1 + (std::max)(getHeight(node->left), getHeight(node->right));

    // 3. 获取平衡因子
    int balance = getBalance(node);

    // 4. 平衡旋转
    // LL
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// === 可视化部分 ===

// 更新坐标
void AVLTree::updateCoordinates(Node* node, int x, int y, int offset) {
    if (node == nullptr) return;

    node->x = x;
    node->y = y;

    // 递归计算子节点坐标
    if (node->left)  updateCoordinates(node->left, x - offset, y + 80, offset / 2);
    if (node->right) updateCoordinates(node->right, x + offset, y + 80, offset / 2);
}

// 画线
void AVLTree::drawLines(Node* node) {
    if (node == nullptr) return;

    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 2);

    if (node->left) {
        line(node->x, node->y, node->left->x, node->left->y);
        drawLines(node->left);
    }
    if (node->right) {
        line(node->x, node->y, node->right->x, node->right->y);
        drawLines(node->right);
    }
}

// 画节点
void AVLTree::drawNodes(Node* node) {
    if (node == nullptr) return;

    // 画圆
    setfillcolor(RGB(255, 250, 200));
    setlinecolor(RGB(24, 144, 255));
    fillcircle(node->x, node->y, 20);

    // 画文字
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, _T("微软雅黑"));

    TCHAR str[16];
    _stprintf_s(str, _T("%d"), node->key);

    int w = textwidth(str);
    int h = textheight(str);
    outtextxy(node->x - w / 2, node->y - h / 2, str);

    drawNodes(node->left);
    drawNodes(node->right);
}

// === 公开接口 ===

void AVLTree::insert(int key) {
    root = insertNode(root, key);
    // 每次插入后，从屏幕中心顶部开始重新计算所有坐标
    updateCoordinates(root, 512, 100, 240);
}

void AVLTree::draw() {
    if (root == nullptr) return;
    drawLines(root); // 先画线
    drawNodes(root); // 再画圆覆盖线头
}