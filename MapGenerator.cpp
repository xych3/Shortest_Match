#include <ctime>
#include <fstream>
#include "function.h"

using namespace std;

int map_num;            //生成地图数量
int m,n;                //地图大小(1<n,m<100)
int num_gw;             //货物与仓库数量
bool vis[maxn][maxn];   //是否被占用判断
string generateMap;     //存储生成的地图

bool inCheck(int x)
{
    return x>1 && x<100;
}

// 生成在[start,end)范围的的随机INT
int randInt(int start, int end)
{
    return (int)(start+(end-start)*rand()/(RAND_MAX+1.0));
}

//写入生成的地图(以0 0为结束)
void mapWrite()
{
    checkmkDir();
    fstream f;
    f.open(prefix+ninput,ios::out);
    f << generateMap;
    f << "0 0";
    f.close();
}

//地图生成函数
void generate()
{
    memset(vis,0,sizeof(vis));
    //初始化地图
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            map[i][j] = '.';
        }
    }
    //生成货物与仓库
    for(int i=0; i<num_gw; i++) {
        bool flag = true;
        int posm, posn;
        //生成G
        while(flag) {
            posm = randInt(0,m);
            posn = randInt(0,n);
            if(!vis[posm][posn]) {
                map[posm][posn] = 'G';
                vis[posm][posn] = true;
                flag = false;
            }
        }
        //生成W
        flag = true;
        while(flag) {
            posm = randInt(0,m);
            posn = randInt(0,n);
            if(!vis[posm][posn]) {
                map[posm][posn] = 'W';
                vis[posm][posn] = true;
                flag = false;
            }
        }
    }
}

int main()
{
    //地图大小输入
    cout << "请输入地图大小m*n,用空格分隔(1<m,n<" << maxn-2 <<"): ";
    cin >> m >> n;
    while(!(inCheck(m)&&inCheck(n))) {
        cout << "输入不合法,请注意范围重新输入(1<m,n<" << maxn-2 <<"): ";
        cin >> m >> n;
    }
    //货物与仓库数量
    cout << "请输入货物G/仓库W数量num(0<num<" << (int)(m*n/2)+1 << "): ";
    cin >> num_gw;
    while(num_gw>(int)(m*n/2)+1) {
        cout << "输入不合法,请注意范围重新输入(0<num<" << (int)(m*n/2)+1 << "): ";
        cin >> num_gw;
    }
    //生成地图数量输入
    cout << "请输入生成地图数量: ";
    cin >> map_num;
    while(map_num<1 || map_num>INF) {
        cout << "输入不合法,请输入合理地图数量: ";
        cin >> map_num;
    }

    generateMap = "";
    //随机生成地图
    for(int i=0; i<map_num; i++) {
        srand((unsigned)time(0)*(i+1));
        //地图生成
        generate();
        //存入地图
        generateMap = generateMap + to_string(m) + ' ' + to_string(n) + '\n';
        for(int j=0; j<m; j++) {
            for(int k=0; k<n; k++) {
                generateMap = generateMap+map[j][k];
            }
            generateMap = generateMap+'\n';
        }
        generateMap = generateMap+'\n';
    }

    //写入
    mapWrite();

    //检查输入文件
    if(!checkDir()) {
        cout << "地图生成失败,请检查原因。" << endl;
    }
    else cout << "地图生成完成！" << endl;

    return 0;
}