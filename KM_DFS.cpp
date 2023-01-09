#include <ctime>
#include "function.h"

using namespace std;

int n,m;
int g[maxn][maxn];              //邻接矩阵
int linked[maxn];               //每个仓库的匹配情况(-1表示无匹配)
int val_g[maxn],val_w[maxn];    //期望值
int slack[maxn];                //松弛值
bool vis_g[maxn],vis_w[maxn];   //访问情况


bool DFS(int x)//hungary匈牙利算法
{
    vis_g[x]=true; //标记本次匹配涉及的货物
    for(int y=0; y<num_w; y++) {
        if(!vis_w[y]) { //保证每次寻找增广路时仓库只访问一次
            int tmp = val_g[x]+val_w[y]-g[x][y];
            if(tmp==0) { //两者都是最优配对选择
                vis_w[y]=true;
                if(linked[y]==-1 || DFS(linked[y])) { //匈牙利算法寻找增广路
                    linked[y] = x;
                    return true;
                }
            }
            else if(slack[y]>tmp) { //存储匹配还需要的最小期望值
                slack[y] = tmp;
            }
        }
    }
    return false;
}

int KM()
{
    memset(linked,-1,sizeof(linked));
    memset(val_w,0,sizeof(val_w)); //仓库期待值初始化为0(有货物就行)

    //找每个货物最近的仓库并赋予货物期待值
    for(int i=0; i<num_g; i++) {
        val_g[i] = -INF;
        for(int j=0; j<num_w; j++) {
            if(g[i][j]>val_g[i]) val_g[i]=g[i][j];
        }
    }
    for(int x=0; x<num_g; x++) {
        memset(slack,INF,sizeof(slack));
        //因为货物需要全部入仓，因此只要没找到匹配就降低期望一直找
        while(true) {
            memset(vis_g,0,sizeof(vis_g));
            memset(vis_w,0,sizeof(vis_w));
            if(DFS(x))
                break; //找到匹配时结束
            int d=INF;
            for(int j=0; j<num_w; j++) { //找到上一趟中没有被匹配，且最小松弛值最小的仓库的最小松弛值
                if(!vis_w[j]&&d>slack[j]) d=slack[j];
            }
            for(int i=0; i<num_g; i++) {
                if(vis_g[i]) val_g[i]-=d; //降低上一趟访问过的货物的期望值
            }
            for(int j=0; j<num_w; j++) {
                if(vis_w[j])
                    val_w[j]+=d; //增加上一趟匹配过的仓库的期望值
                else
                    slack[j]-=d; //减少上一趟没有被匹配的仓库的松弛值(货物期望值已降低)
            }
        }
    }

    int ans=0; //统计匹配边权值和
    for(int i=0;i<num_w;i++) {
        if(linked[i]!=-1) ans+=g[linked[i]][i];
    }
    return -ans; //返回最大的相反数。即要求的最小
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
        memset(g,INF,sizeof(g));
        //计算邻接矩阵
        for(int i=0; i<num_g; i++) {
            for(int j=0; j<num_w; j++) {
                int dist = abs(goods[i].x-house[j].x) + abs(goods[i].y-house[j].y);
                g[i][j] = -dist; //权值取反
            }
        }
        
        //KM算法计算
        cout<< KM() <<endl;

    }
    clock_t stop = clock();
    //输出运行时间
    printf("Time: %d ms\n",(int)(stop-start));

    return 0;
}