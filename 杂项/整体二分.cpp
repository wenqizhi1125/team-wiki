// 整体二分
// 其实只要保证每一层的总修改次数为O（n）复杂度就对了
const int N = 505, M = 6e4 + 5;
int n, m;

int cnt;
struct Node{ int x, y, val; } a[N * N];
bool operator < (const Node &x, const Node &y)
{ return x.val < y.val ; }

int ans[M];
struct node{ int x1, y1, x2, y2, K, id; } wen[M], sta1[M], sta2[M];
struct BIT
{
    int c[N][N];
    BIT(){ memset(c, 0, sizeof(c)); }
    void add(int x, int y, int val) { for(int i = x; i <= n; i += (i & -i)) for(int j = y; j <= n; j += (j & -j)) c[i][j] += val; }
    int query(int x, int y) { int ans = 0; for(int i = x; i; i ^= (i & -i)) for(int j = y; j; j ^= (j & -j)) ans += c[i][j]; return ans; }
}T;

int get(const node &a)
{
    int ans = 0;
    ans += T.query(a.x2 , a.y2 );
    ans += T.query(a.x1 - 1, a.y1 - 1);
    ans -= T.query(a.x2 , a.y1 - 1);
    ans -= T.query(a.x1 - 1, a.y2 );
    return ans;
}

void solve(int l, int r, int L, int R)
{
    if(L > R) return ;
    if(l == r){ for(int i = L; i <= R; ++i) ans[wen[i].id ] = a[l].val ; return ; }
    int mid = (l + r) >> 1;
    for(int i = l; i <= mid; ++i) T.add(a[i].x , a[i].y , 1);
    int s1 = 0, s2 = 0;
    for(int i = L; i <= R; ++i)
    {
        int now = get(wen[i]);
        if(now >= wen[i].K ) sta1[++s1] = wen[i];
        else sta2[++s2] = wen[i];
    }
    int pos = L - 1;
    for(int i = 1; i <= s1; ++i) wen[++pos] = sta1[i];
    for(int i = 1; i <= s2; ++i) wen[++pos] = sta2[i];
    solve(mid + 1, r, L + s1, R);
    for(int i = l; i <= mid; ++i) T.add(a[i].x , a[i].y , -1);
    solve(l, mid, L, L + s1 - 1);
}

int main()
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i) for(int j = 1; j <= n; ++j) a[++cnt] = (Node){i, j, read()};
    sort(a + 1, a + cnt + 1);
    for(int i = 1; i <= m; ++i) wen[i] = (node){read(), read(), read(), read(), read(), i};
    solve(1, cnt, 1, m);
    for(int i = 1; i <= m; ++i) printf("%d\n", ans[i]);
    return 0;
}