#include <graphics.h>
#include <vector>
#include <string>

// ================== 配置与配色 ==================
const COLORREF C_BG = RGB(240, 242, 245);
const COLORREF C_THEME = RGB(24, 144, 255);
const COLORREF C_SHADOW = RGB(220, 220, 220);
const COLORREF C_TEXT_MAIN = RGB(38, 38, 38);

// 辅助：清空消息队列并等待任意输入
void waitAnyKey() {
    FlushMouseMsgBuffer();
    ExMessage msg;
    while (true) {
        msg = getmessage(EM_MOUSE | EM_KEY);
        if (msg.message == WM_LBUTTONUP || msg.message == WM_KEYDOWN)
            return;
    }
}

// ================== 模拟算法模块接口 ==================
void showDemoPage(LPCTSTR title) {
    setbkcolor(WHITE);
    cleardevice();

    settextcolor(C_THEME);
    settextstyle(40, 0, _T("微软雅黑"));
    int w = textwidth(title);
    outtextxy((1024 - w) / 2, 300, title);

    settextcolor(RGB(150, 150, 150));
    settextstyle(20, 0, _T("微软雅黑"));
    LPCTSTR tip = _T("点击鼠标或按任意键返回...");
    w = textwidth(tip);
    outtextxy((1024 - w) / 2, 360, tip);

    FlushBatchDraw();
    waitAnyKey();
}

void runDijkstra() { showDemoPage(_T("Dijkstra 最短路径演示")); }
void runPrim() { showDemoPage(_T("Prim 最小生成树演示")); }
void runAVL() { showDemoPage(_T("AVL 平衡二叉树演示")); }

// ================== UI 组件 ==================
struct ModernButton {
    int x, y, w, h, id;
    LPCTSTR text;
    bool isHover;

    ModernButton(int _x, int _y, int _w, int _h, LPCTSTR _text, int _id)
        : x(_x), y(_y), w(_w), h(_h), text(_text), id(_id), isHover(false) {
    }

    bool checkHover(int mx, int my) {
        bool nowHover = (mx >= x && mx <= x + w && my >= y && my <= y + h);
        if (nowHover != isHover) {
            isHover = nowHover;
            return true;
        }
        return false;
    }

    void draw() {
        if (!isHover) {
            setfillcolor(C_SHADOW);
            solidroundrect(x + 4, y + 4, x + w + 4, y + h + 4, 10, 10);
        }
        setlinecolor(isHover ? C_THEME : RGB(230, 230, 230));
        setfillcolor(isHover ? C_THEME : WHITE);
        fillroundrect(x, y, x + w, y + h, 10, 10);

        setbkmode(TRANSPARENT);
        settextcolor(isHover ? WHITE : C_TEXT_MAIN);
        settextstyle(22, 0, _T("微软雅黑"));
        int tx = x + (w - textwidth(text)) / 2;
        int ty = y + (h - textheight(text)) / 2;
        outtextxy(tx, ty, text);
    }
};

void drawMainInterface(std::vector<ModernButton>& btns) {
    setbkcolor(C_BG);
    cleardevice();

    setfillcolor(C_THEME);
    solidrectangle(0, 0, 1024, 120);

    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(40, 0, _T("微软雅黑"));
    outtextxy(60, 40, _T("数据结构算法可视化系统"));

    setfillcolor(WHITE);
    setlinecolor(WHITE);
    solidroundrect(262, 160, 762, 610, 20, 20);

    settextcolor(C_TEXT_MAIN);
    settextstyle(26, 0, _T("微软雅黑"));
    outtextxy(410, 200, _T("请选择演示模块"));

    for (auto& btn : btns) btn.draw();
}

int main() {
    initgraph(1024, 768);

    std::vector<ModernButton> btns;
    btns.emplace_back(332, 280, 360, 60, _T("1. 迪杰斯特拉 (Dijkstra)"), 1);
    btns.emplace_back(332, 360, 360, 60, _T("2. 最小生成树 (Prim)"), 2);
    btns.emplace_back(332, 440, 360, 60, _T("3. 平衡二叉树 (AVL Tree)"), 3);

    BeginBatchDraw();

    bool isRunning = true;
    bool needRedraw = true;
    ExMessage msg;

    while (isRunning) {
        while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
            if (msg.message == WM_MOUSEMOVE) {
                for (auto& btn : btns) {
                    if (btn.checkHover(msg.x, msg.y)) needRedraw = true;
                }
            }
            else if (msg.message == WM_LBUTTONUP) {
                for (auto& btn : btns) {
                    if (btn.checkHover(msg.x, msg.y)) {
                        switch (btn.id) {
                        case 1: runDijkstra(); break;
                        case 2: runPrim(); break;
                        case 3: runAVL(); break;
                        }
                        needRedraw = true;
                        for (auto& b : btns) b.isHover = false;
                    }
                }
            }
            else if (msg.message == WM_KEYDOWN) {
                if (msg.vkcode == VK_ESCAPE) isRunning = false;
                if (msg.vkcode == '1') { runDijkstra(); needRedraw = true; }
                if (msg.vkcode == '2') { runPrim(); needRedraw = true; }
                if (msg.vkcode == '3') { runAVL(); needRedraw = true; }
            }
        }

        if (needRedraw) {
            drawMainInterface(btns);
            FlushBatchDraw();
            needRedraw = false;
        }
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
    return 0;
}