#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long

int read()
{
    int x = 0; bool f = false; char c = getchar();
    while(c < '0' || c > '9') f |= (c == '-'), c = getchar();
    while(c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

#define N 200000 // 点数
#define M 2000000 // 边数
#define inf 0x3f3f3f3f // 比最大边权还大

struct Edge
{
    int u, v, w;
    Edge(int u = 0, int v = 0, int w = 0):u(u), v(v), w(w){}
};
vector<Edge> E;

struct _EK
{
    int n, m, s, t;
    int head[N], cnt;
    struct edge{ int v, next; ll w; } xing[M];
    void add(int u, int v, ll w)
    {
        ++cnt;
        xing[cnt].v = v;
        xing[cnt].w = w;
        xing[cnt].next = head[u];
        head[u] = cnt;

        ++cnt;
        xing[cnt].v = u;
        xing[cnt].w = 0;
        xing[cnt].next = head[v];
        head[v] = cnt;
    }
    map< pair<int, int>, ll > mp;
    bool vis[N];
    int pre[N];
    ll dis[N], ans;

    // 找最短的增广路
    bool BFS()
    {
        queue<int> q; while(!q.empty()) q.pop();

        for(int i = 1; i <= n; ++i) vis[i] = 0;
        vis[s] = 1; dis[s] = inf; q.push(s);

        while(!q.empty())
        {
            int now = q.front(); q.pop();
            for(int i = head[now]; i; i = xing[i].next )
            {
                int v = xing[i].v ; ll w = xing[i].w ;
                if(!vis[v] && w)
                {
                    vis[v] = true;
                    pre[v] = i;
                    dis[v] = min(w, dis[now]);
                    q.push(v);
                    if(v == t) return true; // 增广出一条路，直接走
                }
            }
        }
        return false; // 已经没有增广路了
    }

    // 回溯，修改边权
    void update()
    {
        int now = t;
        while(now != s)
        {
            xing[pre[now]].w -= dis[t];
            xing[pre[now] ^ 1].w += dis[t];
            now = xing[pre[now] ^ 1].v ;
        }
        ans += dis[t];
    }

    ll solve(int nn, int S, int T, vector<Edge> &E)
    {
        n = nn, m = E.size(), s = S, t = T, cnt = 1;
        for(int i = 0; i < N; ++i) head[i] = pre[i] = 0;
        for(int i = 0; i < M; ++i) xing[i].v = xing[i].w = xing[i].next = 0;
        mp.clear();

        for(auto &[u, v, w] : E) 
        {
            if(mp.find(pair<int, int>(u, v)) == mp.end())
            {
                add(u, v, w);
                mp[pair<int, int>(u, v)] = cnt - 1;
            }else xing[mp[pair<int, int>(u, v)]].w += w;
        }

        ans = 0;
        while(BFS()) update();
        return ans;
    }

}EK;

struct _Dinic
{
    int n, m, s, t;
    int head[N], cnt;
    struct edge{ int v, next; ll w; } xing[M];
    void add(int u, int v, ll w)
    {
        ++cnt;
        xing[cnt].v = v;
        xing[cnt].w = w;
        xing[cnt].next = head[u];
        head[u] = cnt;

        ++cnt;
        xing[cnt].v = u;
        xing[cnt].w = 0;
        xing[cnt].next = head[v];
        head[v] = cnt;
    }
    int depth[N], cur[N]; // 分层图深度，当前弧优化
    ll ans;

    // 先搞一个分层图出来
    bool BFS()
    {
        queue<int> q; while(!q.empty()) q.pop();

        for(int i = 1; i <= n; ++i) depth[i] = n + 5; // inf
        depth[s] = 1; q.push(s);
        for(int i = 1; i <= n; ++i) cur[i] = head[i]; 
        // 建分层图的时候，要把当前弧优化撤销掉，因为可能有被跳过的边反悔，合法了，从头再找
        // 如果把这个写在下面分层的时候复杂度和连通块有关，不过优化效果不大，不如美观
        while(!q.empty())
        {
            int now = q.front(); q.pop();
            for(int i = head[now]; i; i = xing[i].next ) // 这里用head，从头开始
            {
                int v = xing[i].v ;
                if(depth[v] == n + 5 && xing[i].w )
                {
                    depth[v] = depth[now] + 1;
                    q.push(v);
                    if(v == t) return true; // 找到了就退出，能增广就增广
                }
            }
        }
        return false;
    }

    ll DFS(int now, ll res)
    {
        if(now == t) return res;
        ll tmp = 0, have = 0; // tmp 中间变量，have 一共使用了多少
        for(int i = cur[now]; i && res; i = xing[i].next )
        {
            cur[now] = i; // 当前弧优化！
            int v = xing[i].v ;
            if(xing[i].w > 0 && depth[v] == depth[now] + 1)
            {
                tmp = DFS(v, min(xing[i].w , res));
                // 在保证有流量的前提下，这个点已经不能增广了，删掉
                if(tmp == 0) depth[v] = n + 5; 
                res -= tmp, have += tmp;
                xing[i].w -= tmp, xing[i ^ 1].w += tmp; 
            }
        }
        return have; // 从这个点增广了多少流量
    }

    ll solve(int nn, int S, int T, vector<Edge> &E)
    {
        n = nn, m = E.size(), s = S, t = T, cnt = 1;
        for(int i = 0; i < N; ++i) head[i] = 0;
        for(int i = 0; i < M; ++i) xing[i].v = xing[i].w = xing[i].next = 0;

        for(auto &[u, v, w] : E) add(u, v, w);

        ans = 0;
        while(BFS()) ans += DFS(s, inf);
        return ans;
    }

}Dinic;

int main()
{
    int n = read(), m = read(), s = read(), t = read();
    for(int i = 1; i <= m; ++i)
    {
        int u = read(), v = read(); ll w = read();
        E.emplace_back((Edge){u, v, w});
    }
    ll ans = 0;
    // ans = EK.solve(n, s, t, E);
    ans = Dinic.solve(n, s, t, E);
    printf("%lld\n", ans);
    return 0;
}