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

const int MAX_V=250000;
struct edge{int to,cost;};
#define rep(i, n) for(int i = int(0); i < int(n); i++)
#define ALL(a)  (a).begin(),(a).end()
int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dy[8] = {0, 0, -1, 1, -1, 1, -1, 1};
int INF = 2147483647;


//------------------------------------------------------------------------------
namespace hpc {


int H = 500, W = 500;
vector<vector<int> > v(H, vector<int>(W, 0));

vector<edge> graph[MAX_V];
int dist[MAX_V];
int pre[MAX_V];

void dijkstra(int s){
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > que;
    fill(dist, dist + MAX_V, INF);
    fill(pre, pre + MAX_V, -1);
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


vector<int> get_path(int t) {
    vector<int> path;
    for (int cur = t; cur != -1; cur = pre[cur]) {
        path.push_back(cur);
    }
    reverse(ALL(path));
    return path;
}

bool onBoard(int x, int y) {
    if (x < 0 || x >= W  || y < 0 || y >= H) return false;
    else return true;
}

void init(const Stage& aStage) {
    rep (i, MAX_V) graph[i].clear();
    rep (i, H) {
        rep (j, W) {
            Vector2 tmp;
            tmp.x = (float)j / 10;
            tmp.y = (float)i / 10;
            if (aStage.terrain(tmp) == Terrain(0)) v[i][j] = 100;
            else if (aStage.terrain(tmp) == Terrain(1)) v[i][j] = 166;
            else if (aStage.terrain(tmp) == Terrain(2)) v[i][j] = 333;
            else if (aStage.terrain(tmp) == Terrain(3)) v[i][j] = 1000;
        }
    }
    
    rep (i, H) {
        rep (j, W) {
            rep (k, 8) {
                int nx = j + dx[k];
                int ny = i + dy[k];
                if (!onBoard(nx, ny)) continue;
                edge c;
                if (k <= 3) {
                    edge tmp = {ny * W + nx, v[i][j]};
                    c = tmp;
                }
                if (k >= 4) {
                    int cost = v[i][j] * 1.4;
                    edge tmp = {ny * W + nx, cost};
                    c = tmp;
                }
                graph[i * W + j].push_back(c);
            }
        }
    }
    
}


int count = 0;
vector<vector<int> > dp((1LL << 21), vector<int>(21, INF)), tmpPos((1LL << 21), vector<int>(21, INF)), scrollDist(21, vector<int>(21, INF));
vector<float> scrollPosX(21), scrollPosY(21);
vector<int> list(21);
vector<int> next;

void bitDP(const Stage& aStage) {
    next.clear();
    rep (i,(1LL << 21)) rep (j, 21) dp[i][j] = INF;
    rep (i, 21) rep (j, 21) {
        if (i == j || j == 0) scrollDist[i][j] = 0;
        else scrollDist[i][j] = INF;
    }
    int ti = 1;
    for(auto scroll: aStage.scrolls()) {
        scrollPosX[ti] = scroll.pos().x * 10;
        scrollPosY[ti] = scroll.pos().y * 10;
        int tmpx = scrollPosX[ti];
        int tmpy = scrollPosY[ti];
        list[ti] = tmpy * W + tmpx;
        ti++;
    }
    count = ti; // count = 21;
    int tmpx = aStage.rabbit().pos().x * 10 + 0.01;
    int tmpy = aStage.rabbit().pos().y * 10 + 0.01;
    list[0] = tmpy * W + tmpx;
    rep (i, count) {
        dijkstra(list[i]);
        rep (j, count) {
            if (j == 0) scrollDist[i][j] = 0;
            else scrollDist[i][j] = dist[list[j]];
        }
    }
    
    rep (i, 21) scrollDist[i][i] = 0;
    dp[0][0] = 0;
    rep (bit, (1LL << count)) {
        if (bit == 0) continue;
        rep (i, count) {
            if (bit & (1LL << i)) {
                rep (j, count) {
                    if (dp[bit][i] > dp[bit - (1LL << i)][j] + scrollDist[j][i]) {
                        dp[bit][i] = dp[bit - (1LL << i)][j] + scrollDist[j][i];
                        tmpPos[bit][i] = j;
                    }
                }
            }
        }
    }
    int S = (1LL << count) - 1, cur = 0;
    rep (i, count) {
        next.push_back(tmpPos[S][cur]);
        int to = tmpPos[S][cur];
        S = (S - (1LL << cur));
        cur = to;
    }
    reverse(next.begin(), next.end());
}



int gy = -1, gx = -1;
int tx = -1, ty = -1;
vector<int> path;
int id = 0;




Vector2 solve(Vector2 aScrollPos, Vector2 aRabbitPos) {
    int sx = aRabbitPos.x * 10 + 0.01;// (0<=sx<=500)
    int sy = aRabbitPos.y * 10 + 0.01;
    
    if ((int)(aScrollPos.x * 10 + 0.01) == gx && (int)(aScrollPos.y * 10 + 0.01) == gy) {
        //同じ巻物
        
        int warn = path.size();
        if (id == warn - 1) {
            Vector2 ret;
            ret.x = (float)gx / 10 + 0.01;
            ret.y = (float)gy / 10 + 0.01;
            return ret;
        }

        if (sx == tx && sy == ty) id++;
        // 新しい中継地点を探す
        tx = path[id] % W;
        ty = path[id] / W;
        Vector2 ret;
        ret.x = (float)tx / 10 + 0.01;
        ret.y = (float)ty / 10 + 0.01;

        
        return ret;
    } else {
        // 新しい巻物
        int dij = sy * W + sx;
        dijkstra(dij);
        id = 0;
        int pat = sy * W + sx;
        path = get_path(pat);
        tx = path[id] % W;
        ty = path[id] / W;
        gx = aScrollPos.x * 10 + 0.01;
        gy = aScrollPos.y * 10 + 0.01;
        Vector2 ret;
        ret.x = (float)tx / 10 + 0.01;
        ret.y = (float)ty / 10 + 0.01;
        return ret;
    }

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
void Answer::initialize(const Stage& aStage)///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
//    printf(".......\n");
    init(aStage);
    bitDP(aStage);
}

//------------------------------------------------------------------------------
/// 毎フレーム呼び出される処理
/// @detail 移動先を決定して返します
/// @param aStage 現在のステージ
/// @return 移動の目標座標
Vector2 Answer::getTargetPos(const Stage& aStage)
{
    auto pos = aStage.rabbit().pos();
    
    rep (i, count) {
        if (i == 0) continue;
        if (aStage.scrolls()[next[i] - 1].isGotten()) continue;
        Vector2 ret = solve(aStage.scrolls()[next[i] - 1].pos(), pos);
        return ret;
    }
    Vector2 ret;
    ret.x = 0;
    ret.y = 0;
    return ret;
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
