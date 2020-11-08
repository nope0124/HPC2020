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
int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dy[8] = {0, 0, -1, 1, -1, 1, -1, 1};
int INF = 1e9;

//------------------------------------------------------------------------------
namespace hpc {



int H = 50, W = 50;
int n = H * W;
vector<vector<int> > v(H, vector<int>(W, 0));
vector<vector<int> > board(H, vector<int>(W, 0));
vector<vector<int> > acc_sum_plain(H + 1, vector<int>(W + 1, 0));
vector<vector<int> > acc_sum_bush(H + 1, vector<int>(W + 1, 0));
vector<vector<int> > acc_sum_sand(H + 1, vector<int>(W + 1, 0));
vector<vector<int> > acc_sum_pond(H + 1, vector<int>(W + 1, 0));

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



vector<int> get_pond_path(vector<int> vec, int start) {
    vector<int> ret;
    int S = vec.size();
    rep (i, S) {
        if (i == S - 1) ret.push_back(vec[S - 1]);
        if (board[getY(vec[i])][getX(vec[i])] != 1) ret.push_back(vec[i]);
    }
    return ret;
}


vector<int> get_dia_path(vector<int> vec, int start) {
    vector<int> ret;
    int S = vec.size();
    if (S == 1) return vec;
    int fromX, fromY, toX, toY;
    bool tmpFlag = false;
    rep (i, S - 1) {
        if (tmpFlag) {
            tmpFlag = false;
            continue;
        }
        fromX = min(getX(start), getX(vec[i + 1]));
        fromY = min(getY(start), getY(vec[i + 1]));
        toX = max(getX(start), getX(vec[i + 1]));
        toY = max(getY(start), getY(vec[i + 1]));
//        printf("%d\n",acc_sum[toY + 1][toX + 1] + acc_sum[fromY][fromX] - acc_sum[toY + 1][fromX] - acc_sum[fromY][toX + 1]);
        int tmpValue = (toY + 1 - fromY) * (toX + 1 - fromX) * 1.1;
        if ((acc_sum_plain[toY + 1][toX + 1] + acc_sum_plain[fromY][fromX] - acc_sum_plain[toY + 1][fromX] - acc_sum_plain[fromY][toX + 1] <= tmpValue) || (acc_sum_bush[toY + 1][toX + 1] + acc_sum_bush[fromY][fromX] - acc_sum_bush[toY + 1][fromX] - acc_sum_bush[fromY][toX + 1] <= tmpValue) || (acc_sum_sand[toY + 1][toX + 1] + acc_sum_sand[fromY][fromX] - acc_sum_sand[toY + 1][fromX] - acc_sum_sand[fromY][toX + 1] <= tmpValue) || (acc_sum_pond[toY + 1][toX + 1] + acc_sum_pond[fromY][fromX] - acc_sum_pond[toY + 1][fromX] - acc_sum_pond[fromY][toX + 1] <= tmpValue)) {
            ret.push_back(vec[i + 1]);
            tmpFlag = true;
        } else {
            ret.push_back(vec[i]);
            start = vec[i];
        }
        
    }
//    printf("\n");
    if (!tmpFlag) ret.push_back(vec[S - 1]);
    return ret;
}


vector<int> get_new_path(vector<int> vec) {
    vector<int> ret;
    int x = getX(vec[0]);
    int y = getY(vec[0]);
    int dir = -1; //0ならy, 1ならx, 2なら左上, 3なら右上
    rep (i, vec.size()) {
        int warn = vec.size();
        if (i == warn - 1) {
            if (dir == 0) {
                if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 3;
                } else if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 2;
                } else if (x != getX(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 1;
                }
            } else if (dir == 1) {
                if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 3;
                } else if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 2;
                } else if (y != getY(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 0;
                }
            } else if (dir == 2) {
                if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 3;
                } else if (x != getX(vec[i]) && y == getY(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 1;
                } else if (x == getX(vec[i]) && y != getY(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 0;
                }
            } else if (dir == 3) {
                if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                    ret.push_back(vec[i - 1]);
                    dir = 2;
                } else if (x != getX(vec[i]) && y == getY(vec[i])) {
                    ret.push_back(vec[i - 1]);
                    dir = 1;
                } else if (x == getX(vec[i]) && y != getY(vec[i])) {
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
            } else if (getX(vec[i]) != x && getY(vec[i]) == y){
                dir = 1;
                x = getX(vec[i]);
                y = getY(vec[i]);
            } else if ((getX(vec[i]) - 1 == x && getY(vec[i]) - 1 == y) || (getX(vec[i]) + 1 == x && getY(vec[i]) + 1 == y)) {
                dir = 2;
                x = getX(vec[i]);
                y = getY(vec[i]);
            } else {
                dir = 3;
                x = getX(vec[i]);
                y = getY(vec[i]);
            }
            continue;
        }
        if (dir == 0) {
            if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 3;
            } else if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 2;
            } else if (x != getX(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 1;
            }
        } else if (dir == 1) {
            if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 3;
            } else if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 2;
            } else if (y != getY(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 0;
            }
        } else if (dir == 2) {
            if ((x + 1 == getX(vec[i]) && y - 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y + 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 3;
            } else if (x != getX(vec[i]) && y == getY(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 1;
            } else if (x == getX(vec[i]) && y != getY(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 0;
            }
        } else if (dir == 3) {
            if ((x + 1 == getX(vec[i]) && y + 1 == getY(vec[i])) || (x - 1 == getX(vec[i]) && y - 1 == getY(vec[i]))) {
                ret.push_back(vec[i - 1]);
                dir = 2;
            } else if (x != getX(vec[i]) && y == getY(vec[i])) {
                ret.push_back(vec[i - 1]);
                dir = 1;
            } else if (x == getX(vec[i]) && y != getY(vec[i])) {
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
    int start = path[0];
    vector<int> ret = get_new_path(path);
    vector<int> newret = get_dia_path(ret, start);
    vector<int> newnewret = get_pond_path(newret, start);
    return newnewret;
}



bool onBoard(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return false;
    else return true;
}


void init(const Stage& aStage) {
//    printf("\n");
    rep (i, H + 1) {
        acc_sum_plain[i][0] = 0;
        acc_sum_bush[i][0] = 0;
        acc_sum_sand[i][0] = 0;
        acc_sum_pond[i][0] = 0;
    }
    rep (j, W + 1) {
        acc_sum_plain[0][j] = 0;
        acc_sum_bush[0][j] = 0;
        acc_sum_sand[0][j] = 0;
        acc_sum_pond[0][j] = 0;
    }
    rep (i, MAX_V) graph[i].clear();
    
    rep (i, H) {
        rep (j, W) {
            Vector2 tmp;
            tmp.x = j;
            tmp.y = i;
            if (aStage.terrain(tmp) == Terrain(0)) v[i][j] = 100, board[i][j] = 10;
            else if (aStage.terrain(tmp) == Terrain(1)) v[i][j] = 166, board[i][j] = 6;
            else if (aStage.terrain(tmp) == Terrain(2)) v[i][j] = 333, board[i][j] = 3;
            else if (aStage.terrain(tmp) == Terrain(3)) v[i][j] = 1000, board[i][j] = 1;
        }
    }
    rep (i, H) rep (j, W) {
        if (board[i][j] == 10) {
            acc_sum_plain[i + 1][j + 1] = acc_sum_plain[i][j + 1] + acc_sum_plain[i + 1][j] - acc_sum_plain[i][j] + 1;
            acc_sum_bush[i + 1][j + 1] = acc_sum_bush[i][j + 1] + acc_sum_bush[i + 1][j] - acc_sum_bush[i][j] + 2;
            acc_sum_sand[i + 1][j + 1] = acc_sum_sand[i][j + 1] + acc_sum_sand[i + 1][j] - acc_sum_sand[i][j] + 4;
            acc_sum_pond[i + 1][j + 1] = acc_sum_pond[i][j + 1] + acc_sum_pond[i + 1][j] - acc_sum_pond[i][j] + 8;
        } else if (board[i][j] == 6) {
            acc_sum_plain[i + 1][j + 1] = acc_sum_plain[i][j + 1] + acc_sum_plain[i + 1][j] - acc_sum_plain[i][j] + 2;
            acc_sum_bush[i + 1][j + 1] = acc_sum_bush[i][j + 1] + acc_sum_bush[i + 1][j] - acc_sum_bush[i][j] + 1;
            acc_sum_sand[i + 1][j + 1] = acc_sum_sand[i][j + 1] + acc_sum_sand[i + 1][j] - acc_sum_sand[i][j] + 4;
            acc_sum_pond[i + 1][j + 1] = acc_sum_pond[i][j + 1] + acc_sum_pond[i + 1][j] - acc_sum_pond[i][j] + 8;
        } else if (board[i][j] == 3) {
            acc_sum_plain[i + 1][j + 1] = acc_sum_plain[i][j + 1] + acc_sum_plain[i + 1][j] - acc_sum_plain[i][j] + 2;
            acc_sum_bush[i + 1][j + 1] = acc_sum_bush[i][j + 1] + acc_sum_bush[i + 1][j] - acc_sum_bush[i][j] + 2;
            acc_sum_sand[i + 1][j + 1] = acc_sum_sand[i][j + 1] + acc_sum_sand[i + 1][j] - acc_sum_sand[i][j] + 1;
            acc_sum_pond[i + 1][j + 1] = acc_sum_pond[i][j + 1] + acc_sum_pond[i + 1][j] - acc_sum_pond[i][j] + 8;
        } else if (board[i][j] == 1) {
            acc_sum_plain[i + 1][j + 1] = acc_sum_plain[i][j + 1] + acc_sum_plain[i + 1][j] - acc_sum_plain[i][j] + 2;
            acc_sum_bush[i + 1][j + 1] = acc_sum_bush[i][j + 1] + acc_sum_bush[i + 1][j] - acc_sum_bush[i][j] + 2;
            acc_sum_sand[i + 1][j + 1] = acc_sum_sand[i][j + 1] + acc_sum_sand[i + 1][j] - acc_sum_sand[i][j] + 2;
            acc_sum_pond[i + 1][j + 1] = acc_sum_pond[i][j + 1] + acc_sum_pond[i + 1][j] - acc_sum_pond[i][j] + 1;
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
//    rep (i, H) {
//        rep (j, W) {
//            printf("%02d ",v[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n");
    
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
        scrollPosX[ti] = scroll.pos().x;
        scrollPosY[ti] = scroll.pos().y;
        int tmpx = scrollPosX[ti];
        int tmpy = scrollPosY[ti];
        list[ti] = tmpy * W + tmpx;
        ti++;
    }
    count = ti; // count = 21;
    int tmpx = aStage.rabbit().pos().x;
    int tmpy = aStage.rabbit().pos().y;
    list[0] = tmpy * W + tmpx;
    rep (i, count) {
        dijkstra(list[i]);
//        printf("%d\n", list[i]);
        rep (j, count) {
            if (j == 0) scrollDist[i][j] = 0;
//            printf("%d\n",list[j]);
            else {
                scrollDist[i][j] = dist[list[j]];
            }
        }
    }
//    printf("\n");
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
//    rep(i, count) {
//        printf("%d",next[i]);
//    }
//    printf("\n");
}



int gy = -1, gx = -1;
int tx = -1, ty = -1;
vector<int> path;
vector<int> newPath;
int flag = 100;
int id = 0;
int scrollCount = 0;


bool binarySearch(float nx, float ny, int n) {
    int Ix = nx;
    int Iy = ny;
    if (board[Iy][Ix] == n) return false;
    else return true;
}

bool binaryFlag = false;

Vector2 solve(Vector2 aScrollPos, Vector2 aRabbitPos) {
    int sx = aRabbitPos.x;
    int sy = aRabbitPos.y;
    float can = 1;
    rep (i, scrollCount) can *= 1.1;
    can *= (float)board[sy][sx] / 10;
    
    if ((int)aScrollPos.x == gx && (int)aScrollPos.y == gy) {
        //同じ巻物
        
        int warn = path.size();
        if (id == warn - 1) {


            Vector2 ret;
            if (aRabbitPos.y >= gy + 1) {
                ret.y = gy + 0.99;
                if (aRabbitPos.x >= gx + 1) {
                    ret.x = gx + 0.99;
                } else if (aRabbitPos.x <= gx) {
                    ret.x = gx + 0.01;
                } else {
                    ret.x = gx + 0.5;
                }
            } else if (aRabbitPos.y <= gy) {
                ret.y = gy + 0.01;
                if (aRabbitPos.x >= tx + 1) {
                    ret.x = gx + 0.99;
                } else if (aRabbitPos.x <= gx) {
                    ret.x = gx + 0.01;
                } else {
                    ret.x = gx + 0.5;
                }
            } else {
                ret.y = gy + 0.5;
                if (aRabbitPos.x >= gx + 1) {
                    ret.x = gx + 0.99;
                } else if (aRabbitPos.x <= gx) {
                    ret.x = gx + 0.01;
                } else {
                    ret.x = gx + 0.5;
                }
            }
            
            float distanceGoal = sqrt((aRabbitPos.x - (ret.x)) * (aRabbitPos.x - (ret.x)) + (aRabbitPos.y - (ret.y)) * (aRabbitPos.y - (ret.y)));
            float dirX = (can * (ret.x) + (distanceGoal - can) * aRabbitPos.x) / distanceGoal;
            float dirY = (can * (ret.y) + (distanceGoal - can) * aRabbitPos.y) / distanceGoal;
            int IdirX = dirX;
            int IdirY = dirY;
            
            
            
            
            if (distanceGoal <= can) {
                binaryFlag = false;
                return ret;
            } else if (binaryFlag) {
                binaryFlag = false;
            } else if (board[sy][sx] != 1 && board[IdirY][IdirX] == 1) {
                float okx = aRabbitPos.x;
                float oky = aRabbitPos.y;
                float ngx = dirX;
                float ngy = dirY;
//                printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
            //                printf("%f %f\n",okx,oky);
            //                printf("%f %f\n",ngx,ngy);
                rep (i, 10) {
                    float midx = (okx + ngx) / 2;
                    float midy = (oky + ngy) / 2;
                    if (binarySearch(midx, midy, board[IdirY][IdirX])) {
                        okx = midx;
                        oky = midy;
                    } else {
                        ngx = midx;
                        ngy = midy;
                    }
                }
                        
                binaryFlag = true;
                Vector2 ret;
                ret.x = okx;
                ret.y = oky;
                return ret;
            } else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 3) {
                float okx = aRabbitPos.x;
                float oky = aRabbitPos.y;
                float ngx = dirX;
                float ngy = dirY;
//               printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
            //                printf("%f %f\n",okx,oky);
            //                printf("%f %f\n",ngx,ngy);
                rep (i, 10) {
                    float midx = (okx + ngx) / 2;
                    float midy = (oky + ngy) / 2;
                    if (binarySearch(midx, midy, board[IdirY][IdirX])) {
                        okx = midx;
                        oky = midy;
                    } else {
                        ngx = midx;
                        ngy = midy;
                    }
                }
                binaryFlag = true;
                Vector2 ret;
                ret.x = okx;
                ret.y = oky;
                return ret;
            }
//            else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 6) {
//                float okx = aRabbitPos.x;
//                float oky = aRabbitPos.y;
//                float ngx = dirX;
//                float ngy = dirY;
////               printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
//            //                printf("%f %f\n",okx,oky);
//            //                printf("%f %f\n",ngx,ngy);
//                rep (i, 10) {
//                    float midx = (okx + ngx) / 2;
//                    float midy = (oky + ngy) / 2;
//                    if (binarySearch(midx, midy, board[IdirY][IdirX])) {
//                        okx = midx;
//                        oky = midy;
//                    } else {
//                        ngx = midx;
//                        ngy = midy;
//                    }
//                }
//                binaryFlag = true;
//                Vector2 ret;
//                ret.x = okx;
//                ret.y = oky;
//                return ret;
//            }
            binaryFlag = false;
            return ret;
        }
        
        
        
        if (sx == tx && sy == ty) id++;
            // 新しい中継地点を探す
            
            
            
            
            
            
            //たどり着けるなら頑張る
        Vector2 retGoal;
        if (aRabbitPos.y >= gy + 1) {
            retGoal.y = gy + 0.99;
            if (aRabbitPos.x >= gx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        } else if (aRabbitPos.y <= gy) {
            retGoal.y = gy + 0.01;
            if (aRabbitPos.x >= tx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        } else {
            retGoal.y = gy + 0.5;
            if (aRabbitPos.x >= gx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        }
            
        float distanceGoal = sqrt((aRabbitPos.x - (retGoal.x)) * (aRabbitPos.x - (retGoal.x)) + (aRabbitPos.y - (retGoal.y)) * (aRabbitPos.y - (retGoal.y)));
        if (distanceGoal <= can) {
            binaryFlag = false;
            return retGoal;
        }
            

            /////////
        if (id + 1 < warn) {
            int fromX = min(sx, getX(path[id + 1]));
            int fromY = min(sy, getY(path[id + 1]));
            int toX = max(sx, getX(path[id + 1]));
            int toY = max(sy, getY(path[id + 1]));
            int tmpValue = (toY + 1 - fromY) * (toX + 1 - fromX) * 1.1;
            if ((acc_sum_plain[toY + 1][toX + 1] + acc_sum_plain[fromY][fromX] - acc_sum_plain[toY + 1][fromX] - acc_sum_plain[fromY][toX + 1] <= tmpValue) || (acc_sum_bush[toY + 1][toX + 1] + acc_sum_bush[fromY][fromX] - acc_sum_bush[toY + 1][fromX] - acc_sum_bush[fromY][toX + 1] <= tmpValue) || (acc_sum_sand[toY + 1][toX + 1] + acc_sum_sand[fromY][fromX] - acc_sum_sand[toY + 1][fromX] - acc_sum_sand[fromY][toX + 1] <= tmpValue) || (acc_sum_pond[toY + 1][toX + 1] + acc_sum_pond[fromY][fromX] - acc_sum_pond[toY + 1][fromX] - acc_sum_pond[fromY][toX + 1] <= tmpValue)) {
                id++;
            }
        }

        tx = path[id] % W;
        ty = path[id] / W;
        float distance = sqrt((aRabbitPos.x - (tx + 0.5)) * (aRabbitPos.x - (tx + 0.5)) + (aRabbitPos.y - (ty + 0.5)) * (aRabbitPos.y - (ty + 0.5)));
        float dirX = (can * (tx + 0.5) + (distance - can) * aRabbitPos.x) / distance;
        float dirY = (can * (ty + 0.5) + (distance - can) * aRabbitPos.y) / distance;
        int IdirX = dirX;
        int IdirY = dirY;
        
        
        if (distance <= can) {
            
            Vector2 ret;
            //                printf("%d\n",2);
            ret.x = tx + 0.5;
            ret.y = ty + 0.5;
            binaryFlag = false;
            return ret;
        } else if (binaryFlag) {
            binaryFlag = false;
        } else if (board[sy][sx] != 1 && board[IdirY][IdirX] == 1) {
            float okx = aRabbitPos.x;
            float oky = aRabbitPos.y;
            float ngx = dirX;
            float ngy = dirY;
//            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
            //                printf("%f %f\n",okx,oky);
            //                printf("%f %f\n",ngx,ngy);
            rep (i, 10) {
                float midx = (okx + ngx) / 2;
                float midy = (oky + ngy) / 2;
                if (binarySearch(midx, midy, board[dirY][dirX])) {
                    okx = midx;
                    oky = midy;
                } else {
                    ngx = midx;
                    ngy = midy;
                }
            }
            
            
            Vector2 ret;
            ret.x = okx;
            ret.y = oky;
            binaryFlag = true;
            return ret;
        } else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 3) {
            float okx = aRabbitPos.x;
            float oky = aRabbitPos.y;
            float ngx = dirX;
            float ngy = dirY;
//            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
            //                printf("%f %f\n",okx,oky);
            //                printf("%f %f\n",ngx,ngy);
            rep (i, 10) {
                float midx = (okx + ngx) / 2;
                float midy = (oky + ngy) / 2;
                if (binarySearch(midx, midy, board[IdirY][IdirX])) {
                    okx = midx;
                    oky = midy;
                } else {
                    ngx = midx;
                    ngy = midy;
                }
            }
                        
            
            Vector2 ret;
            ret.x = okx;
            ret.y = oky;
            binaryFlag = true;
            return ret;
        }
//        else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 6) {
//            float okx = aRabbitPos.x;
//            float oky = aRabbitPos.y;
//            float ngx = dirX;
//            float ngy = dirY;
//            //            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
//            //                printf("%f %f\n",okx,oky);
//            //                printf("%f %f\n",ngx,ngy);
//            rep (i, 10) {
//                float midx = (okx + ngx) / 2;
//                float midy = (oky + ngy) / 2;
//                if (binarySearch(midx, midy, board[IdirY][IdirX])) {
//                    okx = midx;
//                    oky = midy;
//                } else {
//                    ngx = midx;
//                    ngy = midy;
//                }
//            }
//
//
//            Vector2 ret;
//            ret.x = okx;
//            ret.y = oky;
//            binaryFlag = true;
//            return ret;
//        }
        
        Vector2 ret;
        ret.x = path[id] % W + 0.5;
        ret.y = path[id] / W + 0.5;
//           printf("%d\n",1);
        binaryFlag = false;
        return ret;






            
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
        
        
        //たどり着けるなら頑張る
        Vector2 retGoal;
        if (aRabbitPos.y >= gy + 1) {
            retGoal.y = gy + 0.99;
            if (aRabbitPos.x >= gx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        } else if (aRabbitPos.y <= gy) {
            retGoal.y = gy + 0.01;
            if (aRabbitPos.x >= tx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        } else {
            retGoal.y = gy + 0.5;
            if (aRabbitPos.x >= gx + 1) {
                retGoal.x = gx + 0.99;
            } else if (aRabbitPos.x <= gx) {
                retGoal.x = gx + 0.01;
            } else {
                retGoal.x = gx + 0.5;
            }
        }
            
        float distanceGoal = sqrt((aRabbitPos.x - (retGoal.x)) * (aRabbitPos.x - (retGoal.x)) + (aRabbitPos.y - (retGoal.y)) * (aRabbitPos.y - (retGoal.y)));
        if (distanceGoal <= can) {
            binaryFlag = false;
            return retGoal;
        }



        
        float distance = sqrt((aRabbitPos.x - (tx + 0.5)) * (aRabbitPos.x - (tx + 0.5)) + (aRabbitPos.y - (ty + 0.5)) * (aRabbitPos.y - (ty + 0.5)));
        float dirX = (can * (tx + 0.5) + (distance - can) * aRabbitPos.x) / distance;
        float dirY = (can * (ty + 0.5) + (distance - can) * aRabbitPos.y) / distance;
        int IdirX = dirX;
        int IdirY = dirY;
        
        
        
        if (distance <= can) {
            Vector2 ret;
            ret.x = tx + 0.5;
            ret.y = ty + 0.5;
            binaryFlag = false;
            return ret;
        } else if (binaryFlag) {
            binaryFlag = false;
        } else if (board[sy][sx] != 1 && board[IdirY][IdirX] == 1) {
            float okx = aRabbitPos.x;
            float oky = aRabbitPos.y;
            float ngx = dirX;
            float ngy = dirY;
//            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
        //                printf("%f %f\n",okx,oky);
        //                printf("%f %f\n",ngx,ngy);
            rep (i, 10) {
                float midx = (okx + ngx) / 2;
                float midy = (oky + ngy) / 2;
                if (binarySearch(midx, midy, board[dirY][dirX])) {
                    okx = midx;
                    oky = midy;
                } else {
                    ngx = midx;
                    ngy = midy;
                }
                
            }
            
            Vector2 ret;
            ret.x = okx;
            ret.y = oky;
            binaryFlag = true;
            return ret;
        } else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 3) {
            float okx = aRabbitPos.x;
            float oky = aRabbitPos.y;
            float ngx = dirX;
            float ngy = dirY;
//            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
            //                printf("%f %f\n",okx,oky);
                   //                printf("%f %f\n",ngx,ngy);
            rep (i, 10) {
                float midx = (okx + ngx) / 2;
                float midy = (oky + ngy) / 2;
                if (binarySearch(midx, midy, board[dirY][dirX])) {
                    okx = midx;
                    oky = midy;
                } else {
                    ngx = midx;
                    ngy = midy;
                }
            }
                               
                   
            Vector2 ret;
            ret.x = okx;
            ret.y = oky;
            binaryFlag = true;
            return ret;
        }
//        else if (board[sy][sx] == 10 && board[IdirY][IdirX] == 6) {
//            float okx = aRabbitPos.x;
//            float oky = aRabbitPos.y;
//            float ngx = dirX;
//            float ngy = dirY;
//            //            printf("%d %f %f %f %f\n", board[sy][sx], aRabbitPos.x, aRabbitPos.y, dirX, dirY);
//                        //                printf("%f %f\n",okx,oky);
//                               //                printf("%f %f\n",ngx,ngy);
//            rep (i, 10) {
//                float midx = (okx + ngx) / 2;
//                float midy = (oky + ngy) / 2;
//                if (binarySearch(midx, midy, board[dirY][dirX])) {
//                    okx = midx;
//                    oky = midy;
//                } else {
//                    ngx = midx;
//                    ngy = midy;
//                }
//            }
//
//
//            Vector2 ret;
//            ret.x = okx;
//            ret.y = oky;
//            binaryFlag = true;
//            return ret;
//        }
//
        
        
        
        
        Vector2 ret;
        ret.x = tx + 0.5;
        ret.y = ty + 0.5;
        binaryFlag = false;
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
        scrollCount = i;
        Vector2 ret = solve(aStage.scrolls()[next[i] - 1].pos(), pos);
        int x = ret.x;
        int y = ret.y;
//        if (board[y][x] == 1) printf("%f %f\n",ret.x,ret.y);
        return ret;
    }
    
//    for(auto scroll : aStage.scrolls()) {
//        // まだ手に入れていない巻物を探して、そこに向かって飛ぶ
//        if (!scroll.isGotten()) {
//            //printf("%f %f\n",scroll.pos().x, scroll.pos().y);
//            // Vector2 ret = solve(scroll.pos(), pos);
//            Vector2 ret = solve(scroll.pos(), pos);
//    //          printf("%f %f\n",ret.x,ret.y);
//            return ret;
//        }
//    }
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
