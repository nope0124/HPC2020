//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#include "Answer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <set>
#include <functional>
#include <random>
#include <chrono>
#include <cstring>

using namespace std;

const int MAX_V=2500;
struct edge{int to,cost;};
#define rep(i, n) for(int i = int(0); i < int(n); i++)
#define ALL(a)  (a).begin(),(a).end()
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
int INF = 1e9;

//------------------------------------------------------------------------------
namespace hpc {



int H = 50, W = 50;
int n = H * W;
vector<vector<int> > v(H, vector<int>(W, 0));

vector<edge> graph[MAX_V];
int dist[MAX_V];
int pre[MAX_V];

void dijkstra(int s){
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > que;
    fill(dist, dist + n, INF);
    fill(pre, pre + n, -1);
    dist[s] = 0;
    que.push(pair<int, int>(0, s));
    while(!que.empty()){
        auto p = que.top();
        que.pop();
        int v = p.second;
        if (dist[v] < p.first) continue;
        for (auto e: graph[v]) {
            if (dist[e.to] > dist[v] + e.cost) {
                dist[e.to] = dist[v] + e.cost;
                pre[e.to] = v;
                que.push(pair<int, int>(dist[e.to], e.to));
            }
        }
    }
}



int getX(int ret) {
    return ret % W;
}

int getY(int ret) {
    return ret / W;
}

vector<int> get_new_path(vector<int> vec) {
    vector<int> ret;
    int x = getX(vec[0]);
    int y = getY(vec[0]);
    int dir = -1; //0ならy、1ならx
    rep (i, vec.size()) {
        int warn = vec.size();
        if (i == warn - 1) {
            if (dir == 0) {
                if (x != getX(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 1;
                }
            } else if (dir == 1) {
                if (y != getY(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 0;
                }
            }
            ret.push_back(vec[vec.size() - 1]);
            return ret;
        }
        if (i == 0) {
            x = getX(vec[i]);
            y = getY(vec[i]);
            continue;
        }
        if (i == 1) {
            if (getX(vec[i]) == x && getY(vec[i]) != y) {
                dir = 0;
                x = getX(vec[i]);
                y = getY(vec[i]);
            } else {
                dir = 1;
                x = getX(vec[i]);
                y = getY(vec[i]);
            }
            continue;
        }
        if (dir == 0) {
            if (x != getX(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 1;
            }
        } else if (dir == 1) {
            if (y != getY(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 0;
            }
        }
        x = getX(vec[i]);
        y = getY(vec[i]);
    }
    return ret;
}


vector<int> get_path(int t) {
    vector<int> path;
    for (int cur = t; cur != -1; cur = pre[cur]) {
        path.push_back(cur);
    }
    reverse(ALL(path));
    vector<int> ret = get_new_path(path);
    return ret;
}



bool onBoard(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return false;
    else return true;
}


void init(const Stage& aStage) {
//    printf("\n");
    rep (i, MAX_V) graph[i].clear();
    
    rep (i, H) {
        rep (j, W) {
            Vector2 tmp;
            tmp.x = j;
            tmp.y = i;
            if (aStage.terrain(tmp) == Terrain(0)) v[i][j] = 100;
            else if (aStage.terrain(tmp) == Terrain(1)) v[i][j] = 166;
            else if (aStage.terrain(tmp) == Terrain(2)) v[i][j] = 333;
            else if (aStage.terrain(tmp) == Terrain(3)) v[i][j] = 1000;
        }
    }
    rep (i, H) {
        rep (j, W) {
            rep (k, 4) {
                int nx = j + dx[k];
                int ny = i + dy[k];
                if (!onBoard(nx, ny)) continue;
                edge tmp = {ny * W + nx, v[i][j]};
                graph[i * W + j].push_back(tmp);
            }
        }
    }
//    rep (i, H) {
//        rep (j, W) {
//            printf("%02d ",v[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n");
    
}



int gy = -1, gx = -1;
int tx = -1, ty = -1;
vector<int> path;
vector<int> newPath;
int flag = 100;
int id = 0;



Vector2 solve(Vector2 aScrollPos, Vector2 aRabbitPos) {
    int sx = aRabbitPos.x;
    int sy = aRabbitPos.y;
//    printf("%d %d %d %d\n",sx,sy,tx,ty);
    if ((int)aScrollPos.x == gx && (int)aScrollPos.y == gy) {
        //同じ巻物
        int warn = path.size();
//        printf("%d %d\n",path[0]%W,path[0]/W);
        if (id == warn - 1) {
            return aScrollPos;
        }
        if (sx == tx && sy == ty) {
            // 新しい中継地点を探す
            id++;
//            printf("OK\n");
            tx = path[id] % W;
            ty = path[id] / W;
            Vector2 ret;
            ret.x = path[id] % W + 0.5;
            ret.y = path[id] / W + 0.5;
            return ret;
        } else {
//            if (flag<=0) {
//            Vector2 ret;
//                       ret.x = 0;
//                       ret.y = 0;
//                       return ret;
//            }
            //中継地点の更新なし
////            if (flag >= 1) {
//                flag --;
            Vector2 ret;
            ret.x = path[id] % W + 0.5;
            ret.y = path[id] / W + 0.5;
//                printf("%d\n",1);
//                printf("%f %f\n",ret.x,ret.y);
            return ret;
//            }
        }
    } else {
//        printf("%f %f\n",aRabbitPos.x,aRabbitPos.y);
        // 新しい巻物
//        flag = 100;
        int dij = (int)aRabbitPos.y * W + (int)aRabbitPos.x;
//        printf("%d\n",dij);
        dijkstra(dij);
        id = 0;
        int pat = int(aScrollPos.y) * W + int(aScrollPos.x);
//        printf("%f %f\n", aRabbitPos.x,aRabbitPos.y);
//        printf("\n");
        path = get_path(pat);
//            rep (i, path.size()) {
//                printf("%d %d\n", path[i]%W, path[i]/W);
//            }

//
        tx = path[id] % W;
        ty = path[id] / W;
        gx = aScrollPos.x;
        gy = aScrollPos.y;
        Vector2 ret;
        ret.x = path[id] % W + 0.5;
        ret.y = path[id] / W + 0.5;
        return ret;
    }
//    if (tx == (int)aPos.x && ty == (int)aPos.y && (gx != (int)p.x || gy != (int)p.y)) {
//        id++;
//        Vector2 ret;
//        int X = path[id] % W;
//        int Y = path[id] / W;
//        ret.x = X;
//        ret.y = Y;
//        return ret;
//    } else if (tx == (int)aPos.x && ty == (int)aPos.y && gx == (int)p.x && gy == (int)p.y) {
//        return ret;
//        if (id > path.size() - 1) id = path.size() - 1;
//        Vector2 ret;
//        ret.x = path[id] % W;
//        ret.y = path[id] / W;
//        return ret;
//    }
////    printf("%f %f\n",p.y , p.x);
//    int tmp = p.y * W + p.x;
////    printf("%d\n", p.y * W + p.x);
//    dijkstra(tmp);
//    id = 0;
//    int tmp2 = aPos.y * W + aPos.x;
//    path = get_path(tmp2);
//    Vector2 ret;
//    gx = (int)aPos.x;
//    gy = (int)aPos.y;
//    int X = path[id] % W;
//    int Y = path[id] / W;
//    ret.x = X;
//    ret.y = Y;
//    tx = X;
//    ty = Y;
//    printf("%f %f\n", ret.x, ret.y);
//    return ret;
}









//------------------------------------------------------------------------------
/// コンストラクタ
/// @detail 最初のステージ開始前に実行したい処理があればここに書きます
Answer::Answer()
{
}

//------------------------------------------------------------------------------
/// デストラクタ
/// @detail 最後のステージ終了後に実行したい処理があればここに書きます
Answer::~Answer()
{
}

//------------------------------------------------------------------------------
/// 各ステージ開始時に呼び出される処理
/// @detail 各ステージに対する初期化処理が必要ならここに書きます
/// @param aStage 現在のステージ
void Answer::initialize(const Stage& aStage)
{
    init(aStage);
}

//------------------------------------------------------------------------------
/// 毎フレーム呼び出される処理
/// @detail 移動先を決定して返します
/// @param aStage 現在のステージ
/// @return 移動の目標座標
Vector2 Answer::getTargetPos(const Stage& aStage)
{
    auto pos = aStage.rabbit().pos();
    for(auto scroll : aStage.scrolls()) {
        // まだ手に入れていない巻物を探して、そこに向かって飛ぶ
        if (!scroll.isGotten()) {
            //printf("%f %f\n",scroll.pos().x, scroll.pos().y);
            // Vector2 ret = solve(scroll.pos(), pos);
            Vector2 ret = solve(scroll.pos(), pos);
    //          printf("%f %f\n",ret.x,ret.y);
            return ret;
        }
    }
    return pos;
}

//------------------------------------------------------------------------------
/// 各ステージ終了時に呼び出される処理
/// @detail 各ステージに対する終了処理が必要ならここに書きます
/// @param aStage 現在のステージ
void Answer::finalize(const Stage& aStage)
{
}

} // namespace
// EOF
