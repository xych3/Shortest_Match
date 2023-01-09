#include <vector>
#include <ctime>
#include "function.h"

using namespace std;

int n,m;
int min_cost;                                       //最小移动距离
int g[maxn][maxn];                                  //邻接矩阵
vector<int> linked(maxn,0);                         //每个货物的匹配情况(0表示无匹配)
vector<int> vis_g(maxn,0),vis_w(maxn,0);            //是否已经匹配

//枚举回溯法
void enumerationMatch(vector<int> tmp_linked, vector<int> tmp_visg, vector<int> tmp_visw)
{
    //完全匹配标志
    bool flag = true;
    //如果未完全匹配则枚举回溯
    for(int i=0; i<num_g; i++) {
        if(tmp_visg[i]==1) //寻找未匹配货物
            continue;
        tmp_visg[i] = 1;
        for(int j=0; j<num_w; j++) {
            if(tmp_visw[j]==1) //寻找未匹配仓库
                continue;
            flag = false; //未完全匹配
            tmp_visw[j] = 1;
            tmp_linked[i] = j;
            enumerationMatch(tmp_linked,tmp_visg,tmp_visw);
            tmp_linked[i] = 0;
            tmp_visw[j] = 0;
        }
        tmp_visg[i] = 0;
    }
    
    //已经全部完成匹配
    if(flag) {
        int ans=0; //统计匹配边权值和
        for(int i=0;i<num_g;i++) {
            ans += g[i][tmp_linked[i]];
        }
        min_cost = min_cost>ans? ans:min_cost;
    }
}



int main()
{
    //检查输入文件
    if(!checkDir()) {
        exit(0);
    }
    //输入输出重定向
    string inputfile = prefix+ninput;
    string outputfile = prefix+noutput;
    freopen(inputfile.c_str(), "r", stdin);
    // freopen(outputfile.c_str(), "w", stdout);

    clock_t start = clock();
    while (cin >> n >> m && (n&&m)) {
        //输入地图
        rdMap(n,m);
        min_cost = INF;
        memset(g,INF,sizeof(g));
        //计算邻接矩阵
        for(int i=0; i<num_g; i++) {
            for(int j=0; j<num_w; j++) {
                int dist = abs(goods[i].x-house[j].x) + abs(goods[i].y-house[j].y);
                g[i][j] = dist;
            }
        }
        //枚举算法计算
        enumerationMatch(linked,vis_g,vis_w);

        //输出最小
        cout<< min_cost <<endl;

    }
    clock_t stop = clock();
    //输出运行时间
    printf("Time: %d ms\n",(int)(stop-start));

    return 0;
}