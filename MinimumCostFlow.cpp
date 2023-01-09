#include <ctime>
#include <queue>
#include "function.h"

using namespace std;

int n,m;
int pre[2*maxn];                //存储前驱顶点
int dist[2*maxn];               //存储到源点s的距离

int inq[2*maxn];                //记录每个顶点是否在队列中
int min_c_f;                    //记录增广路径中的残留容量
int vertex;                     //总顶点数(包括货物顶点与仓库顶点+超源点超汇点)
int min_cost;                   //保存最小费用

//求最短路径的SPFA算法
void SPFA(int s)
{
    queue<int> que;
    int u;
    //初始化
    for(int i=0; i<=vertex; i++) { 
        dist[i] = INF;
        pre[i] = -1;
        inq[i] = 0;
    }
    dist[s] = 0;
    que.push(s);
    inq[s] = 1;
    while(!que.empty()) {
        u = que.front();
        que.pop();
        inq[u]=0;
        //更新u的邻接点的dist[], pre[], inq[]
        for(int i=0; i<=vertex; i++) {
            int v=i;
            if(G[u][v].c_f==0) //表示(u,v)没有边
                continue;
            if(G[u][v].v==INF)
                G[u][v].v = -G[v][u].v;
            if(dist[v] > dist[u]+G[u][v].v) { //松弛操作
                dist[v] = dist[u]+G[u][v].v;
                pre[v]=u;
                if(inq[v]==0) {
                    que.push(v);
                    inq[v]=1;
                }  
            }
        }
    }
}

void ford_fulkerson(int s,int t)
{
    SPFA(s);
    while(pre[t]!=-1) { //pre为-1表示没有找到从s到t的增广路径
        min_cost+=dist[t]; //将这一条最短路径的值加进min_cost
        min_c_f = INF;
        int u=pre[t], v=t;
        //计算增广路径上的残留容量
        while(u!=-1) {  
            if(min_c_f > G[u][v].c_f) min_c_f = G[u][v].c_f;
            v=u;
            u=pre[v];
        }
        u=pre[t], v=t;
        while(u!=-1) {
            G[u][v].f+=min_c_f; //修改流
            G[v][u].f=-G[u][v].f;
            G[u][v].c_f=G[u][v].c-G[u][v].f; //修改残留容量
            G[v][u].c_f=G[v][u].c-G[v][u].f;
            v=u;
            u=pre[v];
        }
        SPFA(s);
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
        min_cost = 0;
        vertex = num_g+num_w+1; //加入超源点0与超汇点,构成抽象网络流结构

        for(int u=0; u<=vertex; u++) {
            for(int v=0; v<=vertex; v++) {
                G[u][v].c = G[v][u].c = 0;
                G[u][v].c_f = G[v][u].c_f = 0;  
                G[u][v].f = G[v][u].f = 0;  
                G[u][v].v = G[v][u].v = INF;  
            }
        }

        for(int i=1; i<=num_g; i++) {
            G[0][i].v = 0;  //从超源点到每个货物的费用/权值为0
            G[0][i].c = G[0][i].c_f = 1; //从超源点到每个货物之间的容量取为1
            for(int j=1; j<=num_w; j++) {
                int w = abs(goods[i-1].x-house[j-1].x) + abs(goods[i-1].y-house[j-1].y); //计算边权w
                G[i][num_g+j].v = w; //注意第二个坐标预留了货物结点的数据数量
                G[i][num_g+j].c = G[i][num_g+j].c_f = 1; //容量取1
                G[num_g+j][vertex].v = 0; //从每个仓库到超汇点的费用/权值为0
                G[num_g+j][vertex].c = G[num_g+j][vertex].c_f = 1;//从每个仓库到超汇点的容量取为1
            }
        }
        
        //FF算法计算最大流(使用SPFA计算最小费用最大流)
        ford_fulkerson(0, vertex);
        cout<< min_cost <<endl;

    }
    clock_t stop = clock();
    //输出运行时间
    printf("Time: %d ms\n",(int)(stop-start));

    return 0;
}