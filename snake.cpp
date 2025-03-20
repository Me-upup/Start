#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cmath>
#include <windows.h>
using namespace std;

/*** 光标定位 ***/
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;

void locate(int x,int y)
{
    coord.X=y;
    coord.Y=x;
    SetConsoleCursorPosition(hout,coord);
};

/*** 隐藏光标 ***/
void hide()
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(hout, &cursor_info);
}

/*** 生成随机数 ***/
double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

/*** 定义地图的长宽，蛇的坐标，长度，方向，食物的位置 ***/
int m,n;

struct node
{
    int x,y;
}snake[1000];

int snake_length,dir;
node food;
int direct[4][2]={{-1,0},{1,0},{0,-1},{0,1}};

/*** 输出墙 ***/
void print_wall()
{
    wcout << L" ";
    for (int i=1;i<=n;i++)
        wcout << L"-";
    wcout << endl;
    for (int j=0;j<=m-1;j++)
    {
        wcout << L"|";
        for (int i=1;i<=n;i++) wcout << L" ";
        wcout << L"|" << endl;
    }
    wcout << L" ";
    for (int i=1;i<=n;i++)
        wcout << L"-";
}

/*** 首次输出蛇，其中snake[0]代表头 ***/
void print_snake()
{
    locate(snake[0].x,snake[0].y);
    wcout << L"@";
    for (int i=1;i<=snake_length-1;i++)
    {
        locate(snake[i].x,snake[i].y);
        wcout << L"*";
    }
}

/*** 判断是否撞墙或者自撞 ***/
bool is_correct()
{
    if (snake[0].x==0 || snake[0].y==0 || snake[0].x==m+1 || snake[0].y==n+1) return false;
    for (int i=1;i<=snake_length-1;i++)
    {
        if (snake[0].x==snake[i].x && snake[0].y==snake[i].y) return false;
    }
    return true;
}

/*** 随机生成并输出食物位置 ***/
bool print_food()
{
    srand((unsigned)time(0));
    bool e;
    while (1)
    {
        e=true;
        int i=(int) random(0,m)+1,j=(int) random(0,n)+1;
        food.x=i;food.y=j;
        for (int k=0;k<=snake_length-1;k++)
        {
            if (snake[k].x==food.x && snake[k].y==food.y)
            {
                e=false;break;
            }
        }
        if (e) break;
    }
    locate(food.x,food.y);
    wcout << L"$";
    return true;
}

/*** 蛇的前进 ***/
bool go_ahead()
{
    node temp;
    bool e=false;
    temp=snake[snake_length-1];
    for (int i=snake_length-1;i>=1;i--)
        snake[i]=snake[i-1];
    snake[0].x+=direct[dir][0];
    snake[0].y+=direct[dir][1];
    locate(snake[1].x,snake[1].y);
    wcout << L"*";
    /*** 吃到了食物 ***/
    if (snake[0].x==food.x && snake[0].y==food.y)
    {
        snake_length++;
        e=true;
        snake[snake_length-1]=temp;
    }
    /*** 输出此时蛇状态 ***/
    if (!e)
    {
        locate(temp.x,temp.y);
        wcout << L" ";
    }
    else
        print_food();
    locate(snake[0].x,snake[0].y);
    wcout << L"@";
    /*** 如果自撞 ***/
    if (!is_correct())
    {
        system("cls");
        wcout << L"You lose!" << endl << L"Length: " << snake_length << endl;
        return false;
    }
    return true;
}

/*** 主函数 ***/
int main()
{
    system("chcp 65001"); // 设置控制台编码为 UTF-8
    system("mode con cols=80 lines=30"); // 设置cmd窗口大小
    system("color 0"); // 设置字符颜色：0 黑；1 深蓝；2 深绿；3 深青；4 深红；5 深紫；6 深褐 
    wcout << L"--------------------贪吃蛇---------------------" << endl;
    wcout << L"先选择难度.请在1-10中输入1个数,1最简单,10则最难" << endl;
    wcout << L"然后进入游戏画面,以方向键控制方向.祝你游戏愉快!" << endl;
    wcout << L"-----------------------------------------------" << endl;
    wcout << L"作者：Ikun小学生" << endl;
    wcout << L"请输入难度级别：" ; 
    m=25;
    n=40; 
    if (m<10 || n<10 || m>25 || n>40)
    {
        wcout << L"ERROR" << endl;
        system("pause");
        return 0;
    }
    int hard;
    wcin >> hard;
    if (hard<=0 || hard>100)
    {
        wcout << L"ERROR" << endl;
        system("pause");
        return 0;
    }
    /*** 数据全部初始化，包括蛇长，位置，方向 ***/
    snake_length=5;
    clock_t a,b;
    char ch;
    double hard_len;
    for (int i=0;i<=4;i++)
    {
        snake[i].x=1;
        snake[i].y=5-i;
    }
    dir=3;
    /*** 输出初始地图，蛇与食物 ***/
    system("cls");
    hide();
    print_wall();
    print_food();
    print_snake();
    locate(m+2,0);
    wcout << L"现在长度是: ";
    /*** 开始游戏 ***/
    while (1)
    {
        /*** 难度随长度增加而提高 ***/
        hard_len=(double)snake_length/(double) (m*n);
        /*** 调节时间，单位是ms ***/
        a=clock();
        while (1)
        {
            b=clock();
            if (b-a>=(int)(400-30*hard)*(1-sqrt(hard_len))) break;
        }
        /*** 接受键盘输入的上下左右，并以此改变方向 ***/
        if (kbhit())
        {
            ch=getch();
            if (ch==-32)
            {
                ch=getch();
                switch(ch)
                {
                case 72:
                    if (dir==2 || dir==3)
                        dir=0;
                    break;
                case 80:
                    if (dir==2 || dir==3)
                        dir=1;
                    break;
                case 75:
                    if (dir==0 || dir==1)
                        dir=2;
                    break;
                case 77:
                    if (dir==0 || dir==1)
                        dir=3;
                    break;
                }
            }
        }
        /*** 前进 ***/
        if (!go_ahead()) break;
        /*** 在最后输出此时长度 ***/
        locate(m+2,12);
        wcout << snake_length;
    }
    system("pause");
    return 0;
}