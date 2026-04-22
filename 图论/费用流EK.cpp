// "费用流只学spfa费用流就够了"
// spfa费用流其实就是EK费用流
// 每次增广一条边，增广一条单位费用最小的路径，保证最小费用，不断增广，保证最大流
// 注意反向边的费用设置为-w
// 复杂度：O( V^2 E^2 )

#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long

ll read()
{
    ll x = 0; bool f = false; char c = getchar();
    while(c < '0' || c > '9') f |= (c == '-'), c = getchar();
    while(c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

#define N 200000 // 点数
#define M 2000000 // 边数 
#define inf 0x3f3f3f3f // 比最大流量还大

struct Edge{ int u, v; ll flow, w; };
vector<Edge> E;

struct _EK
{
    int n, m, s, t;
    int head[N], cnt;
    struct edge{ int v, next; ll flow, w; } xing[M];
    void add(int u, int v, ll flow, ll w)
    { 
        xing[++cnt] = (edge){v, head[u], flow, w}; head[u] = cnt;
        xing[++cnt] = (edge){u, head[v], 0, -w}; head[v] = cnt;
    }
    bool vis[N];
    int pre[N];
    ll Flow, W, flow[N], dis[N];
    bool spfa()
    {
        for(int i = 1; i <= n; ++i) dis[i] = inf, flow[i] = inf, vis[i] = 0;
        queue<int> q; while(!q.empty()) q.pop();
        dis[s] = 0, vis[s] = 1; q.push(s);

        while(!q.empty())
        {
            int now = q.front(); q.pop(); vis[now] = 0;
            for(int i = head[now]; i; i = xing[i].next )
            {
                int v = xing[i].v ;
                if(xing[i].flow > 0 && dis[v] > dis[now] + xing[i].w )
                {
                    dis[v] = dis[now] + xing[i].w ;
                    pre[v] = i;
                    flow[v] = min(flow[now], xing[i].flow );
                    if(!vis[v]) vis[v] = 1, q.push(v);
                }
            }
        }
        if(dis[t] == inf || flow[t] <= 0) return false;
        else return true;
    }

    void update()
    {
        Flow += flow[t], W += dis[t] * flow[t];
        int now = t;
        while(now != s)
        {
            xing[pre[now]].flow -= flow[t], xing[pre[now] ^ 1].flow += flow[t];
            now = xing[pre[now] ^ 1].v ;
        }
    }

    pair<ll, ll> solve(int nn, int S, int T, vector<Edge> &E)
    {
        n = nn, m = E.size(), s = S, t = T, cnt = 1;
        for(int i = 0; i < N; ++i) head[i] = 0;
        for(int i = 0; i < M; ++i) xing[i].v = xing[i].next = xing[i].flow = xing[i].w = 0;

        for(auto &[u, v, flow, w] : E) add(u, v, flow, w);
        Flow = W = 0;
        while(spfa()) update();
        return pair<ll, ll>(Flow, W);
    }

}EK;

int main()
{
    int n = read(), m = read(), S = read(), T = read();
    for(int i = 1; i <= m; ++i)
    {
        int u = read(), v = read();
        ll flow = read(), w = read();
        E.emplace_back((Edge){u, v, flow, w});
    }
    pair<ll, ll> ans = EK.solve(n, S, T, E);
    printf("%lld %lld\n", ans.first , ans.second );
    return 0; 
}