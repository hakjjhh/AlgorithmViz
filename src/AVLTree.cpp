#include "../include/AVLTree.h" 
#include <tchar.h> 
#include <string>

AVLTree::AVLTree() : root(nullptr), lastLog(L"状态：就绪") {}

int AVLTree::getHeight(Node* node) { return node ? node->height : 0; }
int AVLTree::getBalance(Node* node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }

Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left) current = current->left;
    return current;
}

// === 旋转时增加日志 ===
Node* AVLTree::rightRotate(Node* y) {
    lastLog = L"执行右旋 (LL情况)..."; // 记录日志
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

Node* AVLTree::leftRotate(Node* x) {
    lastLog = L"执行左旋 (RR情况)..."; // 记录日志
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = (std::max)(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = (std::max)(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

Node* AVLTree::insertNode(Node* node, int key) {
    if (!node) return new Node(key);
    if (key < node->key) node->left = insertNode(node->left, key);
    else if (key > node->key) node->right = insertNode(node->right, key);
    else return node;

    node->height = 1 + (std::max)(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key) return rightRotate(node);
    if (balance < -1 && key > node->right->key) return leftRotate(node);
    if (balance > 1 && key > node->left->key) { node->left = leftRotate(node->left); return rightRotate(node); }
    if (balance < -1 && key < node->right->key) { node->right = rightRotate(node->right); return leftRotate(node); }
    return node;
}

Node* AVLTree::deleteNode(Node* root, int key) {
    if (!root) return root;
    if (key < root->key) root->left = deleteNode(root->left, key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) { temp = root; root = nullptr; }
            else *root = *temp;
            delete temp;
        }
        else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (!root) return root;
    root->height = 1 + (std::max)(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) { root->left = leftRotate(root->left); return rightRotate(root); }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) { root->right = rightRotate(root->right); return leftRotate(root); }
    return root;
}

bool AVLTree::searchNode(Node* node, int key) {
    if (!node) return false;

    // 1. 准备数字字符串 (后面要反复用)
    TCHAR str[16];
    _stprintf_s(str, _T("%d"), node->key);
    int w = textwidth(str);

    // --- 步骤 A: 节点变绿 (表示正在经过) ---
    setfillcolor(GREEN);
    setlinecolor(RGB(24, 144, 255));
    fillcircle(node->x, node->y, 20); // 画绿圆

    // 关键修复：必须把字重新写上去！
    setbkmode(TRANSPARENT);
    settextcolor(WHITE); // 绿底白字比较清晰
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(node->x - w / 2, node->y - 10, str);

    FlushBatchDraw(); // 立即刷新显示
    Sleep(400);       // 

    // --- 步骤 B: 判断是否找到 ---
    if (key == node->key) {
        // 找到了！变红
        setfillcolor(RED);
        fillcircle(node->x, node->y, 25); // 稍微变大一点表示强调

        settextcolor(WHITE); // 红底白字
        outtextxy(node->x - w / 2, node->y - 10, str);

        FlushBatchDraw();
        Sleep(800); // 找到了多停一会儿
        return true;
    }

    // --- 步骤 C: 递归查找 ---
    bool found;
    if (key < node->key) found = searchNode(node->left, key);
    else found = searchNode(node->right, key);

   
    setfillcolor(RGB(255, 250, 200)); // 恢复淡黄
    fillcircle(node->x, node->y, 20);
    settextcolor(BLACK); // 恢复黑字
    outtextxy(node->x - w / 2, node->y - 10, str);

    int bf = getBalance(node);
    TCHAR strBF[10]; _stprintf_s(strBF, _T("%d"), bf);
    settextstyle(12, 0, _T("Arial")); settextcolor(RGB(150, 150, 150));
    outtextxy(node->x + 12, node->y - 20, strBF);

    return found;
}

void AVLTree::updateCoordinates(Node* node, int x, int y, int offset) {
    if (!node) return;
    node->x = x; node->y = y;
    if (node->left) updateCoordinates(node->left, x - offset, y + 80, offset / 2);
    if (node->right) updateCoordinates(node->right, x + offset, y + 80, offset / 2);
}

void AVLTree::drawLines(Node* node) {
    if (!node) return;
    setlinecolor(RGB(200, 200, 200)); setlinestyle(PS_SOLID, 2);
    if (node->left) { line(node->x, node->y, node->left->x, node->left->y); drawLines(node->left); }
    if (node->right) { line(node->x, node->y, node->right->x, node->right->y); drawLines(node->right); }
}

void AVLTree::drawNodes(Node* node) {
    if (!node) return;
    setfillcolor(RGB(255, 250, 200)); setlinecolor(RGB(24, 144, 255));
    fillcircle(node->x, node->y, 20);
    setbkmode(TRANSPARENT); settextcolor(BLACK); settextstyle(20, 0, _T("微软雅黑"));
    TCHAR str[16]; _stprintf_s(str, _T("%d"), node->key);
    int w = textwidth(str);
    outtextxy(node->x - w / 2, node->y - 10, str);

    // 显示 BF
    int bf = getBalance(node);
    TCHAR strBF[10]; _stprintf_s(strBF, _T("%d"), bf);
    settextstyle(12, 0, _T("Arial")); settextcolor(RGB(150, 150, 150));
    outtextxy(node->x + 12, node->y - 20, strBF);

    drawNodes(node->left); drawNodes(node->right);
}

Node* AVLTree::getNodeAt(Node* node, int mx, int my) {
    if (!node) return nullptr;
    int dx = node->x - mx; int dy = node->y - my;
    if (dx * dx + dy * dy < 20 * 20) return node;
    Node* l = getNodeAt(node->left, mx, my);
    if (l) return l;
    return getNodeAt(node->right, mx, my);
}

void AVLTree::insert(int key) {
    root = insertNode(root, key);
    updateCoordinates(root, 612, 100, 200);
    wchar_t buf[100]; swprintf_s(buf, L"成功插入节点: %d", key);
    lastLog = buf;
}

void AVLTree::remove(int key) {
    root = deleteNode(root, key);
    updateCoordinates(root, 612, 100, 200);
    wchar_t buf[100]; swprintf_s(buf, L"成功删除节点: %d", key);
    lastLog = buf;
}

bool AVLTree::search(int key) {
    lastLog = L"开始查找演示...";
    bool found = searchNode(root, key);
    if (root) draw();

    return found; // 把“找没找到”的结果告诉 main.cpp
}
void AVLTree::draw() { if (root) { drawLines(root); drawNodes(root); } }
Node* AVLTree::findClickedNode(int x, int y) { return getNodeAt(root, x, y); }
Node* AVLTree::getRoot() { return root; }

// 辅助递归：中序遍历 (左 -> 根 -> 右)
void inOrderHelper(Node* node, std::wstring& str) {
    if (!node) return;
    inOrderHelper(node->left, str);

    // 拼接到字符串里，用箭头隔开
    wchar_t buf[20];
    swprintf_s(buf, L"%d -> ", node->key);
    str += buf;

    inOrderHelper(node->right, str);
}

void AVLTree::inOrderTraversal(std::wstring& result) {
    result = L""; // 清空
    inOrderHelper(root, result);
    // 去掉最后多余的 " -> "
    if (result.length() > 4) {
        result = result.substr(0, result.length() - 4);
    }
    else {
        result = L"空树";
    }
}

// 辅助递归：统计节点数
int countNodes(Node* node) {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int AVLTree::getNodeCount() {
    return countNodes(root);
}

int AVLTree::getTreeHeight() {
    return getHeight(root);
}
void preOrderHelper(Node* node, std::wstring& str) {
    if (!node) return;

    // 先记录根
    wchar_t buf[20];
    swprintf_s(buf, L"%d -> ", node->key);
    str += buf;

    preOrderHelper(node->left, str);  // 再左
    preOrderHelper(node->right, str); // 后右
}

// 2. 后序遍历辅助 (左 -> 右 -> 根)
void postOrderHelper(Node* node, std::wstring& str) {
    if (!node) return;

    postOrderHelper(node->left, str);  // 先左
    postOrderHelper(node->right, str); // 再右

    // 最后记录根
    wchar_t buf[20];
    swprintf_s(buf, L"%d -> ", node->key);
    str += buf;
}

// === 公开接口实现 ===

void AVLTree::preOrderTraversal(std::wstring& result) {
    result = L"";
    preOrderHelper(root, result);
    if (result.length() > 4) result = result.substr(0, result.length() - 4);
    else result = L"空树";
}

void AVLTree::postOrderTraversal(std::wstring& result) {
    result = L"";
    postOrderHelper(root, result);
    if (result.length() > 4) result = result.substr(0, result.length() - 4);
    else result = L"空树";
}