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
vector<int> E_DCC[N];
void tarjan(int k, int fa)
{
    dfn[k] = low[k] = ++tot;
    sta[++top] = k;
    // 判孤立点
    if(fa == 0 && head[k] == 0)
    {
        ++num;
        E_DCC[num].push_back(k);
        --top;
        return ;
    }
    int flag = 0;
    for(int i = head[k]; i; i = xing[i].next )
    {
        int v = xing[i].v ;
        if(v == fa && flag == 0){ flag = 1; continue; } 
        if(dfn[v] == 0)
        {
            tarjan(v, k);
            low[k] = min(low[k], low[v]);
        }else low[k] = min(low[k], dfn[v]);
    }

    if(dfn[k] == low[k])
    {
        ++num;
        while(sta[top] != k) E_DCC[num].push_back(sta[top--]);
        E_DCC[num].push_back(sta[top--]);
    }
}

int main()
{
    n = read(), m = read();
    for(int i = 1; i <= m; ++i)
    {
        int u = read(), v = read();
        if(u == v) continue;
        add(u, v), add(v, u);
    }

    for(int i = 1; i <= n; ++i)
        if(dfn[i] == 0) tarjan(i, 0);
    
    printf("%d\n", num);
    for(int i = 1; i <= num; ++i)
    {
        printf("%d ", E_DCC[i].size());
        for(int j = 0; j < E_DCC[i].size(); ++j)
        {
            printf("%d ", E_DCC[i][j]);
        }
        printf("\n");
    }
    return 0;
}