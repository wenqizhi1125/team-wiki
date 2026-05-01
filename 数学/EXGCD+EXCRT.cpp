// 求一组特解(x,y) 满足 ax+by=gcd(a,b)
ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if(!b){ x = 1, y = 0; return a; }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}
// 求解最小的 k = a_i (mod b_i)，second 为 lcm(b_1,b_2...b_n)
// 无解返回pll(-1, 0)
pair<ll, ll> excrt(int n, vector<ll> a, vector<ll> b)
{
    ll M = 1, x, y;
    for(int i = 0; i < n; ++i) M = M / exgcd(M, b[i], x, y) * b[i];
    ll X = a[0], lcm = b[0];
    for(int i = 1; i < n; ++i)
    {
        ll d = exgcd(lcm, b[i], x, y);
        if((a[i] - X) % d != 0) return pll(-1, 0);
        x *= (a[i] - X) / d;
        x = (x % (b[i] / d) + (b[i] / d)) % (b[i] / d);
        X = (X + x * lcm) % M;
        lcm = lcm / d * b[i];
    }
    return pll(X, M);
}