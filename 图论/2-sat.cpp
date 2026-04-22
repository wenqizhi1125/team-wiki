const int N = 2e6 + 5;
int n, m;
vector<int> to[N];
int dfn[N], low[N], tot;
int vis[N], sta[N], top;
int belong[N], num;

void tarjan(int k)
{
    vis[k] = 1, sta[++top] = k, dfn[k] = low[k] = ++tot;
    for(int v : to[k])
    {
        if(!dfn[v]) tarjan(v), low[k] = min(low[v], low[k]);
        else if(vis[v]) low[k] = min(low[k], dfn[v]);
    }
    if(dfn[k] == low[k])
    {
        ++num;
        while(sta[top] != k) belong[sta[top]] = num, vis[sta[top]] = 0, --top;
        belong[sta[top]] = num, vis[sta[top]] = 0, --top;
    }
}

int main()
{
    n = read(), m = read();
    for(int i = 1; i <= m; ++i)
    {
        int u = read(), a = read(), v = read(), b = read();
        to[u + a * n].emplace_back(v + (b ^ 1) * n);
        to[v + b * n].emplace_back(u + (a ^ 1) * n);
    }
    for(int i = 1; i <= (n << 1); ++i) if(!dfn[i]) tarjan(i);
    int flag = 1;
    for(int i = 1; i <= n; ++i) flag &= belong[i] != belong[i + n];
    if(!flag){ printf("IMPOSSIBLE\n"); return 0; }
    printf("POSSIBLE\n");
    for(int i = 1; i <= n; ++i) printf("%d ", belong[i] < belong[i + n]);
    return 0;
}