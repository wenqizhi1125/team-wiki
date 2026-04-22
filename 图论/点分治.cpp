const int N = 1e4 + 10, M = 1e7 + 10;
int n, m, query[110], ok[110], cnt, head[N];
struct edge{ int v, next, w; } xing[N << 1];
void add(int u, int v, int w)
{
    xing[++cnt].v = v;
    xing[cnt].w = w;
    xing[cnt].next = head[u];
    head[u] = cnt;
}

int maxx[N], size[N], root, sum, xx[N], dis[N];
int vis[N];
void getroot(int k, int fa)
{
    size[k] = 1, maxx[k] = 0;
    for (re int i = head[k]; i; i = xing[i].next )
    {
        int v = xing[i].v ;
        if(vis[v] || v == fa) continue;
        getroot(v, k);
        size[k] += size[v];
        maxx[k] = max(maxx[k], size[v]);
    }
    maxx[k] = max(maxx[k], sum - maxx[k]);
    if(maxx[k] < maxx[root]) root = k;
}

int d[N], b[N], tot; //存子树所有节点，属于哪个子树

void getdis(int k, int fa, int zu)
{
    d[++tot] = k;
    b[k] = zu;
    for (re int i = head[k]; i; i = xing[i].next )
    {
        int v = xing[i].v, w = xing[i].w ;
        if(vis[v] || v == fa) continue;
        dis[v] = dis[k] + w;
        getdis(v, k, zu);
    }
}

bool cmp(int x, int y)
{
    return dis[x] < dis[y];
}

void solve(int k)
{
    tot = 0;
    d[++tot] = k, dis[k] = 0, b[k] = k;
    for (re int i = head[k]; i ; i = xing[i].next )
    {
        int v = xing[i].v ;
        if(vis[v]) continue;
        dis[v] = xing[i].w ;
        getdis(v, k, v);
    }
    sort(d + 1, d + tot + 1, cmp);
    for (re int i = 1; i <= m; ++i)
    {
        int l = 1, r = tot;
        if(ok[i]) continue;
        while (l < r)
        {
            if(dis[d[l]] + dis[d[r]] > query[i]) --r;
            else if(dis[d[l]] + dis[d[r]] < query[i]) ++l;
            else if(b[d[l]] == b[d[r]])
            {
                if(dis[d[r]] == dis[d[r - 1]]) --r;
                else ++l;
            }  
            else{ ok[i] = 1; break ; }
        }
    }
}

void work(int k)
{
    vis[k] = 1;
    solve(k);
    for (re int i = head[k]; i ; i = xing[i].next )
    {
        int v = xing[i].v ;
        if(vis[v]) continue;
        root = 0, sum = size[v];
        getroot(v, 0);
        work(root);
    }
}

int main()
{
    n = read(), m = read();
    for (re int i = 1; i < n; ++i)
    {
        int s1 = read(), s2 = read(), s3 = read();
        add(s1, s2, s3), add(s2, s1, s3);
    }
    for (re int i = 1; i <= m; ++i) query[i] = read();
    maxx[0] = 0x7fffffff, sum = n, root = 0;
    getroot(1, 0);
    work(root);
    for (re int i = 1; i <= m; ++i)
        if(ok[i] || query[i] == 0) printf ("AYE\n");
        else printf ("NAY\n");
    return 0;
}