// 一道wqs二分+决策单调性题目
const int N = 1e5 + 5;
int n, m;
int x[N];
ll s[N], dp[N];
struct node
{
    int j, l, r;
    node(int j = 0, int l = 0, int r = 0):j(j), l(l), r(r){}
}q[N];
int l, r;

ll get(int i, int j){ return dp[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1); } 

ll check(ll k)
{
    l = 1, r = 0;
    q[++r] = node(0, 1, n);
    for(int i = 1; i <= n; ++i)
    {
        while(l <= r && q[l].r == i - 1) ++l; // 无效决策舍弃
        q[l].l = i; // 合法位置很重要！
        dp[i] = dp[q[l].j ] + (s[i] - s[q[l].j ] + 1) * (s[i] - s[q[l].j ] + 1) - k;
        int pos = n + 1;
        while(l <= r)
        {
            // 整段都选i更优
            if(get(q[r].l , q[r].j ) > get(q[r].l , i))
            {
                pos = q[r].l , --r;
                continue;
            }
            // 整段选i都不优
            if(get(q[r].r , q[r].j ) <= get(q[r].r , i))
            {
                break;
            }
            // 部分优，部分不优，二分一下
            int L = q[r].l , R = q[r].r ;
            while(L < R)
            {
                int mid = (L + R) >> 1;
                if(get(mid, q[r].j ) <= get(mid, i)) L = mid + 1;
                else R = mid;
            }
            // 更新区间，记得break
            q[r].r = L - 1, pos = L;
            break;
        }
        if(pos <= n) q[++r] = node(i, pos, n);
    }
    return dp[n];
}

void solve()
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i) x[i] = read(), s[i] = s[i - 1] + x[i];
    
    ll l = -1e18, r = 0;
    while(l < r)
    {
        ll mid = (l + r) >> 1;
        ll t1 = check(mid) + mid * m, t2 = check(mid + 1) + (mid + 1) * m;
        if(t1 < t2) l = mid + 1;
        else r = mid;
    }
    printf("%lld\n", check(l) + l * m);
}

signed main()
{
    int T = 1;
    // T = read();
    while(T--) solve();
    return 0;
}