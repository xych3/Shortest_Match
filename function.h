#include <io.h>
#include <direct.h>
#include <string.h>
#include <iostream>
using namespace std;

//常量
const int maxn=102;                     //最大顶点数,不大于100,左右留空
const int INF=0x3f3f3f3f;
const string prefix = "data/";
const string ninput = "input_test.txt";
const string noutput = "output.txt";

//变量
char map[maxn][maxn];                   //地图(左右留空)
int num_g;                              //货物数
int num_w;                              //仓库数

//存放仓库的坐标
struct House
{
    int x,y; 
}house[maxn];

//存放货物的坐标
struct Goods
{
    int x,y;
}goods[maxn];

//存放货物/仓库结点与其他结点的信息
struct element
{
    int c;          //容量
    int f;          //流
    int c_f;        //残余容量
    int v;          //价值/费用
}G[2*maxn][2*maxn];

/*
读取地图并返回货物/仓库数
*/
void rdMap(int n, int m)
{
    int h1=-1, g1=-1;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=m; j++) {
            cin >> map[i][j];
            if(map[i][j]=='W') {
                ++h1;
                house[h1].x=i;
                house[h1].y=j;
            }
            else if(map[i][j]=='G') {
                ++g1;
                goods[g1].x=i;
                goods[g1].y=j;
            }
    }
    num_g = g1+1;
    num_w = h1+1;
}

/*
检查文件夹与数据文件
*/
bool checkDir()
{
    if(_access((prefix).c_str(),0) == -1) {
        cout << "Dir isn't exist." << endl;
        return false;
    }
    if(_access((prefix+ninput).c_str(),0) == -1) {
        cout << "Input isn't exist." << endl;
        return false;
    }
    return true;
}

/*
检查与创建文件夹
*/
void checkmkDir()
{
    if(_access(prefix.c_str(),0) == -1)
        _mkdir(prefix.c_str());
}