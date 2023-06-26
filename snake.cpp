#include <conio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include "easyx/graphics.h"
#include <cstring>
#include <ctime>
#include <conio.h>
#include "snake.h"
#include <thread>

// 初始化
void init()
{
    snake.sn.emplace_back(3, 3);
    snake.sn.emplace_back(4, 3);
    snake.sn.emplace_back(5, 3);

    snake.dx = 1;
    snake.dy = 0;
    snake.dir = 'R';

    food.p.x = 5;
    food.p.y = 5;

    grade = 0;

    is_running = false;

    w = 500;
    h = 500;
}

// 绘制蛇
void draw_snake()
{
    setfillcolor(GREEN);
    for(auto &e: snake.sn) fillrectangle(e.x * 25, e.y * 25, (e.x + 1) * 25, (e.y + 1) * 25);

    setfillcolor(BLUE);
    int hx = snake.sn.back().x;
    int hy = snake.sn.back().y;
    fillrectangle(hx * 25, hy * 25, (hx + 1) * 25, (hy + 1) * 25);

    setfillcolor(YELLOW);
    fillrectangle(food.p.x * 25, food.p.y * 25, (food.p.x + 1) * 25, (food.p.y + 1) * 25);
}

// 绘制地图
void draw_map()
{
    for(int i = 0; i < 25; i++) {
        for(int j = 0; j < 25; j++) {
            if((i + j) % 2 == 0) {
                setfillcolor(RGB(170,215,81));
            } else {
                setfillcolor(RGB(162,208,72));
            }
            solidrectangle(i * 25, j * 25, (i + 1)* 25, (j + 1) * 25);
        }
    }

}

// 绘制
void draw()
{
    cleardevice();

    draw_map();
    draw_snake();

    setbkmode(TRANSPARENT); // 设置背景颜色为透明
    if (is_running) {
        std::string s = "grade: " + std::to_string(grade);
        int tw = textwidth((LPTSTR)s.c_str());
        settextstyle(20, 0, _T("times new roman"));
        outtextxy(w - tw, 0, (LPTSTR)s.c_str());
    }
    else {
        std::string s1 = "pleas enter SPACE key to start!";
        int tw2 = textwidth((LPTSTR)s1.c_str());
        settextstyle(20, 0, _T("times new roman"));
        outtextxy(w / 2 - tw2 / 2, h / 2, (LPTSTR)s1.c_str());
    }

    FlushBatchDraw();
}

// 移动
void move()
{
    // 蛇头
    int hx = snake.sn.back().x + snake.dx;
    int hy = snake.sn.back().y + snake.dy;

    // 触碰边界问题处理
    if(hx < 0)  hx = 19;
    if(hx > 19) hx = 0;
    if(hy < 0)  hy = 19;
    if(hy > 19) hy = 0;

    // 判断是否吃到食物
    if(hx == food.p.x && hy == food.p.y) {
        snake.sn.emplace_back(hx, hy);
        food.p.x = rand() % 20;
        food.p.y = rand() % 20;
        grade += 25;
    } else {
        for(int i = 0; i < snake.sn.size() - 1; i++) {
            snake.sn[i].x = snake.sn[i + 1].x;
            snake.sn[i].y = snake.sn[i + 1].y;
        }
        snake.sn.back().x = hx;
        snake.sn.back().y = hy;
    }
}

// 控制器
void controller()
{
    ExMessage msg;
    while(peekmessage(&msg, EM_KEY)) {
        if(msg.message == WM_KEYDOWN) { // 获取键盘按下消息
            switch(msg.vkcode) {
                case 'W':
                case VK_UP:    if(snake.dir != 'D') {snake.dx = 0; snake.dy = -1; snake.dir = 'U';} break;
                case 'S':
                case VK_DOWN:  if(snake.dir != 'U') {snake.dx = 0; snake.dy =  1; snake.dir = 'D';} break;
                case 'A':
                case VK_LEFT:  if(snake.dir != 'R') {snake.dx = -1; snake.dy = 0; snake.dir = 'L';} break;
                case 'D':
                case VK_RIGHT: if(snake.dir != 'L') {snake.dx = 1; snake.dy =  0; snake.dir = 'R';} break;
                case VK_SPACE: is_running = !is_running; break; // 暂停
                case VK_ESCAPE: exit(0); // 退出
            }
        }
    }
    flushmessage();  // 清空缓存消息
}

// 游戏开始
void play()
{
    draw();
    controller();
}

void move_snake()
{
    while (true)
    {
        if(is_running) move();
        Sleep(100);
    }

}

int main()
{
    init();
    initgraph(w, h);
    BeginBatchDraw();

    // 启动一个线程用于移动蛇位置
    std::thread th1(move_snake);
    th1.detach();

    while (true)
    {
        play();
    }

    EndBatchDraw();
    closegraph();
    return 0;
}