const int N = 15;
int n;
ll a[N], m[N], ans, x, y, M; 

// 模数互质但不是质数！！！exgcd求逆元
void exgcd(ll a, ll b, ll &x, ll &y)
{
    if(!b){ x = 1, y = 0; return ; }
    exgcd(b, a % b, y, x);
    y -= a / b * x;
}

void solve()
{
    n = read(),  M = 1; // 记得给 M 初始化
    for(int i = 1; i <= n; ++i) m[i] = read(), a[i] = read(), M *= m[i];
    for(int i = 1; i <= n; ++i)
    {
        exgcd(M / m[i], m[i], x, y);
        x = (x % m[i] + m[i]) % m[i];
        ans = (ans + M / m[i] * x * a[i]) % M;
    }
    printf("%lld\n", ans);
}

int main()
{
    solve();
    return 0;
}