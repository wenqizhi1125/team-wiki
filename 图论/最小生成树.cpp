const int N = 4e5 + 5;
int n, m;
int head[N], cnt;
struct Edge{ int v, w, next; } e[N];
void add(int u, int v, int w){ e[++cnt] = (Edge){v, w, head[u]}, head[u] = cnt; }

int ans;
bool AC;

int dis[N], num; // dis是当前点到已经确定的集合的最短距离，也就是会被选入的边的长度
bool vis[N];
void prim()
{
    for(int i = 2; i <= n; ++i) dis[i] = 0x7fffffff;
    priority_queue< pair<int, int> > q;
    q.push(pair<int, int>(0, 1));
    while(q.size())
    {
        int now = q.top().second ;
        q.pop();
        if(vis[now]) continue;
        vis[now] = 1;
        ++num;
        ans += dis[now];
        dis[now] = 0;
        for(int i = head[now]; i; i = e[i].next )
        {
            int v = e[i].v ;
            if(!vis[v] && dis[v] > e[i].w )
            {
                dis[v] = e[i].w ;
                q.push(pair<int, int>(-dis[v], v));
            }
        }
    }
}

int fa[N], len;

int find(int x){ return (x == fa[x]) ? x : fa[x] = find(fa[x]); }
struct node{ int u, v, w; } E[N];

bool operator < (node a, node b)
{ return a.w < b.w ; }

void Add(int u, int v, int w){ E[++len] = (node){u, v, w}; }

void kruskal()
{
    for(int i = 1; i <= n; ++i) fa[i] = i;
    sort(E + 1, E + len + 1);
    ++num;
    for(int i = 1; i <= len; ++i)
    {
        int u = E[i].u , v = E[i].v , w = E[i].w ;
        u = find(u), v = find(v);
        if(u == v) continue;
        ++num;
        ans += w;
        fa[u] = v;
    }
}

int main()
{   
    n = read(), m = read();
    for(int i = 1; i <= m; ++i)
    {
        int u = read(), v = read(), w = read();
        // add(u, v, w);
        // add(v, u, w);
        Add(u, v, w);
    }
    // prim();
    kruskal();   
    if(num == n) printf("%d\n", ans);
    else printf("orz\n");
    return 0;
}