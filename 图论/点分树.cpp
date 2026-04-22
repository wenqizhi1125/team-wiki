const int inf = 0x7fffffff;
const int N = 1e5 + 5;
int n, m, a[N];
vector<int> to[N];
int Size[N], Son[N], depth[N], top[N], f[N];

void getson(int k, int fa)
{
    depth[k] = depth[fa] + 1, f[k] = fa, Size[k] = 1;
    for(int v : to[k])
    {
        if(v == fa) continue;
        getson(v, k);
        Size[k] += Size[v];
        if(Size[v] > Size[Son[k]]) Son[k] = v;
    }
}

void gettop(int k, int zu)
{
    top[k] = zu;
    if(Son[k]) gettop(Son[k], zu);
    for(int v : to[k]) if(v != f[k] && v != Son[k]) gettop(v, v);
}

int LCA(int x, int y)
{
    while(top[x] != top[y])
    {
        if(depth[top[x]] < depth[top[y]]) y = f[top[y]];
        else x = f[top[x]];
    }
    if(depth[x] < depth[y]) return x;
    else return y;
}

int dis(int x, int y){ return depth[x] + depth[y] - 2 * depth[LCA(x, y)]; }

int rt;
int Fa[N], vis[N], SIZE[N], mx[N];

void getrt(int k, int fa, int tot)
{
    SIZE[k] = 1, mx[k] = 0;
    for(int v : to[k])
    {
        if(vis[v] || v == fa) continue;
        getrt(v, k, tot);
        mx[k] = max(mx[k], SIZE[v]);
        SIZE[k] += SIZE[v];
    }
    mx[k] = max(mx[k], tot - SIZE[k]);
    if(mx[k] < mx[rt]) rt = k;
}

void solve(int k, int tot)
{
    vis[k] = 1;
    int Siz = 0;
    for(int v : to[k])
    {
        if(vis[v]) continue;
        rt = 0, Siz = (SIZE[v] > SIZE[k]) ? (tot - SIZE[k]) : (SIZE[v]);
        getrt(v, k, Siz), Fa[rt] = k, solve(rt, Siz);
    }
}

#define ls(x) son[x][0]
#define rs(x) son[x][1]
int root[N << 1], tot;
int son[N * 80][2];
int sum[N * 80];

void update(int &k, int l, int r, int pos, int val)
{
    if(!k) k = ++tot;
    if(l == r){ sum[k] += val; return ; }
    int mid = (l + r) >> 1;
    if(pos <= mid) update(ls(k), l, mid, pos, val);
    else update(rs(k), mid + 1, r, pos, val);
    sum[k] = sum[ls(k)] + sum[rs(k)];
}

int query(int k, int l, int r, int L, int R)
{
    if(!k) return 0; 
    if(L <= l && r <= R) return sum[k];
    int mid = (l + r) >> 1;
    if(R <= mid) return query(ls(k), l, mid, L, R);
    if(L > mid) return query(rs(k), mid + 1, r, L, R);
    return query(ls(k), l, mid, L, R) + query(rs(k), mid + 1, r, L, R);
}

void modify(int k, int val)
{
    int now = k;
    while(now)
    {
        update(root[now], 0, n - 1, dis(now, k), val);
        if(Fa[now]) update(root[now + n], 0, n - 1, dis(Fa[now], k), val);
        now = Fa[now];
    }
}

int Query(int k, int K)
{
    int now = k;
    int ans = 0;
    ans += query(root[now], 0, n - 1, 0, K);
    while(Fa[now])
    {
        int D = dis(k, Fa[now]);
        if(D > K){ now = Fa[now]; continue; }
        ans += query(root[Fa[now]], 0, n - 1, 0, K - D);
        ans -= query(root[now + n], 0, n - 1, 0, K - D);
        now = Fa[now];
    }
    return ans;
}

int main()
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i) a[i] = read();
    for(int i = 1; i < n; ++i)
    {
        int u = read(), v = read();
        to[u].emplace_back(v);
        to[v].emplace_back(u);
    }

    getson(1, 0);
    gettop(1, 1);

    mx[0] = inf, rt = 0;
    getrt(1, 0, n);
    solve(rt, n);

    for(int i = 1; i <= n; ++i) modify(i, a[i]);

    int last = 0;
    for(int t = 1; t <= m; ++t)
    {
        int opt = read(), x = read() ^ last, y = read() ^ last;
        if(opt == 0) printf("%d\n", last = Query(x, y));
        else modify(x, y - a[x]), a[x] = y; 
    }

    return 0;
}