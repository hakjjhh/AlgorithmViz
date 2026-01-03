#include <graphics.h>
#include <vector>
#include <string>
#include <ctime>
#include "../include/AVLTree.h" 

// ================== 高级 UI 配色 ==================
const COLORREF C_SIDEBAR = RGB(240, 240, 240); // 侧边栏灰
const COLORREF C_CANVAS = WHITE;              // 绘图区白
const COLORREF C_STATUS = RGB(50, 50, 50);    // 状态栏深灰
const COLORREF C_THEME = RGB(24, 144, 255);  // 主题蓝

// 通用按钮结构
struct ModernButton {
    int x, y, w, h, id;
    LPCTSTR text;
    bool isHover;

    ModernButton(int _x, int _y, int _w, int _h, LPCTSTR _text, int _id)
        : x(_x), y(_y), w(_w), h(_h), text(_text), id(_id), isHover(false) {
    }

    bool contains(int mx, int my) {
        return (mx >= x && mx <= x + w && my >= y && my <= y + h);
    }
    bool updateHover(int mx, int my) {
        bool now = contains(mx, my);
        if (now != isHover) { isHover = now; return true; }
        return false;
    }
    void draw() {
        setfillcolor(isHover ? C_THEME : WHITE);
        setlinecolor(isHover ? C_THEME : RGB(200, 200, 200));
        fillroundrect(x, y, x + w, y + h, 5, 5);

        setbkmode(TRANSPARENT);
        settextcolor(isHover ? WHITE : BLACK);
        settextstyle(18, 0, _T("微软雅黑"));
        int tx = x + (w - textwidth(text)) / 2;
        int ty = y + (h - textheight(text)) / 2;
        outtextxy(tx, ty, text);
    }
};

// 辅助：清空消息队列
void waitAnyKey() {
    FlushMouseMsgBuffer(); ExMessage msg;
    while (true) {
        msg = getmessage(EM_MOUSE | EM_KEY);
        if (msg.message == WM_LBUTTONUP || msg.message == WM_KEYDOWN) return;
    }
}

// 简单的占位界面
void showDemoPage(LPCTSTR title) {
    setbkcolor(WHITE); cleardevice();
    settextcolor(C_THEME); settextstyle(40, 0, _T("微软雅黑"));
    outtextxy(300, 300, title);
    FlushBatchDraw(); waitAnyKey();
}
void runDijkstra() { showDemoPage(_T("Dijkstra 模块")); }
void runPrim() { showDemoPage(_T("Prim 模块")); }

// ================== AVL 界面逻辑 ==================
void drawAVLInterface(AVLTree& tree, std::vector<ModernButton>& btns) {
    // 1. 侧边栏背景
    setfillcolor(RGB(240, 240, 240));
    solidrectangle(0, 0, 200, 720);
    setlinecolor(RGB(200, 200, 200));
    line(200, 0, 200, 720);

    // 2. 标题
    setbkmode(TRANSPARENT);
    settextcolor(RGB(24, 144, 255));
    settextstyle(24, 0, _T("微软雅黑"));
    outtextxy(25, 30, _T("AVL 算法演示"));

    // 3. 统计信息 (新增功能：显示树的参数)
    settextcolor(RGB(80, 80, 80));
    settextstyle(16, 0, _T("微软雅黑"));
    TCHAR stats[100];
    _stprintf_s(stats, _T("节点总数: %d"), tree.getNodeCount());
    outtextxy(20, 400, stats);
    _stprintf_s(stats, _T("树的高度: %d"), tree.getTreeHeight());
    outtextxy(20, 425, stats);

    // 4. 操作说明
    settextcolor(RGB(150, 150, 150));
    settextstyle(14, 0, _T("宋体"));
    outtextxy(20, 600, _T("提示:"));
    outtextxy(20, 620, _T("点右侧空白: 随机插入"));
    outtextxy(20, 640, _T("点节点: 删除节点"));

    // 5. 绘图区背景
    setfillcolor(WHITE);
    solidrectangle(201, 0, 1024, 720);

    // 6. 状态栏
    setfillcolor(RGB(50, 50, 50));
    solidrectangle(0, 720, 1024, 768);
    settextcolor(WHITE);
    settextstyle(18, 0, _T("宋体"));
    outtextxy(20, 735, tree.getLog().c_str());

    // 7. 绘制按钮和树
    for (auto& btn : btns) btn.draw();
    tree.draw();
}

// AVL主逻辑
void runAVL() {
    setbkcolor(WHITE); cleardevice();
    AVLTree tree;

    // 预设几个数据
    tree.insert(50); tree.insert(20); tree.insert(80);

    // 定义按钮 (调整了布局，增加了新按钮)
    std::vector<ModernButton> btns;
    int btnX = 25, startY = 80, gap = 50;

    btns.emplace_back(btnX, startY, 150, 35, _T("插入指定值"), 1); // 新增
    btns.emplace_back(btnX, startY + gap, 150, 35, _T("随机插入"), 2);
    btns.emplace_back(btnX, startY + gap * 2, 150, 35, _T("演示查找"), 3);
    btns.emplace_back(btnX, startY + gap * 3, 150, 35, _T("数据遍历"), 5); // 新增
    btns.emplace_back(btnX, startY + gap * 4, 150, 35, _T("清空画布"), 4);
    btns.emplace_back(btnX, startY + gap * 5, 150, 35, _T("返回主菜单"), 6);

    bool exitFlag = false;
    ExMessage msg;
    bool needRedraw = true;

    while (!exitFlag) {
        while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
            if (msg.message == WM_MOUSEMOVE) {
                for (auto& btn : btns) if (btn.updateHover(msg.x, msg.y)) needRedraw = true;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                bool btnClicked = false;

                // 1. 按钮点击处理
                for (auto& btn : btns) {
                    if (btn.contains(msg.x, msg.y)) {
                        btnClicked = true;
                        switch (btn.id) {
                        case 1: // === 插入指定值 ===
                        {
                            TCHAR buf[20];
                            InputBox(buf, 20, _T("请输入要插入的整数:"), _T("精准插入"), _T(""), 0, 0, false);
                            int val = _ttoi(buf);
                            if (val != 0 || buf[0] == '0') {
                                tree.insert(val);
                            }
                        }
                        break;

                        case 2: // 随机插入
                            tree.insert(rand() % 99 + 1);
                            break;

                        case 3: // 演示查找
                        {
                            TCHAR buf[20];
                            InputBox(buf, 20, _T("请输入要查找的数字:"), _T("演示查找"), _T(""), 0, 0, false);
                            int val = _ttoi(buf);
                            if (val != 0 || buf[0] == '0') {
                                drawAVLInterface(tree, btns); // 刷新去残影
                                if (!tree.search(val)) {
                                    MessageBox(GetHWnd(), _T("未找到该节点"), _T("提示"), MB_OK | MB_ICONWARNING);
                                }
                            }
                        }
                        break;
                        case 5:  //遍历结果
                        {
                            std::wstring sPre, sIn, sPost;

                            // 获取三种遍历结果
                            tree.preOrderTraversal(sPre);
                            tree.inOrderTraversal(sIn);
                            tree.postOrderTraversal(sPost);

                            // 拼接成一个长消息
                            std::wstring msg = L"=== AVL树结构校验 ===\n\n";
                            msg += L"1. 前序 (根左右): " + sPre + L"\n   [作用: 还原树结构]\n\n";
                            msg += L"2. 中序 (左根右): " + sIn + L"\n   [作用: 验证有序性]\n\n";
                            msg += L"3. 后序 (左右根): " + sPost + L"\n   [作用: 验证子树完整]";

                            // 弹窗显示
                            MessageBox(GetHWnd(), msg.c_str(), _T("完整遍历数据"), MB_OK | MB_ICONINFORMATION);
                        }
                        break;

                        case 4: tree = AVLTree(); break; // 清空
                        case 6: exitFlag = true; break;  // 返回
                        }
                        needRedraw = true;
                    }
                }

                // 2. 空白处点击 (快捷随机插入) / 节点点击 (删除)
                if (!btnClicked && msg.x > 200) {
                    Node* clickedNode = tree.findClickedNode(msg.x, msg.y);
                    if (clickedNode) {
                        // 删除前确认一下（防止手滑）- 可选
                        tree.remove(clickedNode->key);
                    }
                    else {
                        tree.insert(rand() % 99 + 1);
                    }
                    needRedraw = true;
                }
            }
        }

        if (needRedraw) {
            drawAVLInterface(tree, btns);
            FlushBatchDraw();
            needRedraw = false;
        }
        Sleep(10);
    }
}

// ================== 主菜单 (入口) ==================
int main() {
    srand((unsigned)time(0));
    initgraph(1024, 768);
    BeginBatchDraw();

    // 主菜单按钮
    std::vector<ModernButton> mainBtns;
    mainBtns.emplace_back(332, 280, 360, 60, _T("1. 迪杰斯特拉 (Dijkstra)"), 1);
    mainBtns.emplace_back(332, 360, 360, 60, _T("2. 最小生成树 (Prim)"), 2);
    mainBtns.emplace_back(332, 440, 360, 60, _T("3. 平衡二叉树 (AVL Tree)"), 3);

    bool running = true;
    bool needRedraw = true;
    ExMessage msg;

    while (running) {
        while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
            if (msg.message == WM_MOUSEMOVE) {
                for (auto& b : mainBtns) if (b.updateHover(msg.x, msg.y)) needRedraw = true;
            }
            else if (msg.message == WM_LBUTTONDOWN) {
                for (auto& b : mainBtns) {
                    if (b.contains(msg.x, msg.y)) {
                        switch (b.id) {
                        case 1: runDijkstra(); break;
                        case 2: runPrim(); break;
                        case 3: runAVL(); break; // 进入高级AVL界面
                        }
                        needRedraw = true;
                        // 重置主菜单按钮状态
                        for (auto& btn : mainBtns) btn.isHover = false;
                    }
                }
            }
            else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) running = false;
        }

        if (needRedraw) {
            setbkcolor(RGB(240, 242, 245)); cleardevice();
            setfillcolor(C_THEME); solidrectangle(0, 0, 1024, 120);
            setbkmode(TRANSPARENT); settextcolor(WHITE); settextstyle(40, 0, _T("微软雅黑"));
            outtextxy(60, 40, _T("数据结构算法可视化系统"));
            setfillcolor(WHITE); setlinecolor(WHITE); solidroundrect(262, 160, 762, 610, 20, 20);
            settextcolor(RGB(38, 38, 38)); settextstyle(26, 0, _T("微软雅黑"));
            outtextxy(410, 200, _T("请选择演示模块"));
            for (auto& b : mainBtns) b.draw();
            FlushBatchDraw();
            needRedraw = false;
        }
        Sleep(10);
    }
    EndBatchDraw();
    closegraph();
    return 0;
}