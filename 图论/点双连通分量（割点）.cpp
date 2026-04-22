const int N = 5e5 + 10;
int n, m;
int head[N], cnt;
struct Edge{ int v, next; } xing[N << 3];
void add(int u, int v)
{
    ++cnt;
    xing[cnt].v = v;
    xing[cnt].next = head[u];
    head[u] = cnt;
}

int dfn[N], low[N], tot;
int sta[N], top;
int num;
vector<int> SCC[N];
void tarjan(int k, int fa)
{
    dfn[k] = low[k] = ++tot;
    sta[++top] = k;
    if(fa == 0 && head[k] == 0)
    {
        ++num;
        SCC[num].push_back(k);
        --top;
        return ;
    }
    for(re int i = head[k]; i; i = xing[i].next )
    {
        int v = xing[i].v ;
        if(v == fa) continue;
        if(dfn[v] == 0)
        {
            tarjan(v, k);
            low[k] = min(low[k], low[v]);
            if(low[v] >= dfn[k])
            {
                ++num;
                while(sta[top] != v) SCC[num].push_back(sta[top--]);
                SCC[num].push_back(sta[top--]);
                SCC[num].push_back(k);
            }
        }else low[k] = min(low[k], dfn[v]);
    }
}

int main()
{
    n = read(), m = read();
    for(re int i = 1; i <= m; ++i)
    {
        int u = read(), v = read();
        if(u == v) continue;
        add(u, v), add(v, u);
    }

    for(re int i = 1; i <= n; ++i)
        if(dfn[i] == 0) tarjan(i, 0);
    
    printf("%d\n", num);
    for(re int i = 1; i <= num; ++i)
    {
        printf("%d ", SCC[i].size());
        for(re int j = 0; j < SCC[i].size(); ++j)
        {
            printf("%d ", SCC[i][j]);
        }
        printf("\n");
    }
    return 0;
}