#ifndef SNAKE_H
#define SNAKE_H
#include <vector>

struct Point
{
    int x, y;
    Point(){}
    Point(int xx, int yy):x(xx), y(yy){}
};
struct Snake
{
    std::vector<Point> sn;  // 蛇身
    int dx, dy;             // 方向增量
    char dir;               // 方向
};
struct Food
{
    Point p;           // 食物坐标
    int time;          // 食物时间
};

Snake snake;
Food food;
int grade;             // 得分
bool is_running;       // 是否启动游戏
int w, h;              // 窗口宽、高

void init();
void draw_snake();
void draw();
void move();
void play();
void controller();
void draw_map();
void move_snake();

#endif