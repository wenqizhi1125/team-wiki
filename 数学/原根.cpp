const int N = 1e6 + 5;
int prime[N], cnt, vis[N], phi[N];
int p[N], tot, have[N];
int g[N], num;

void into()
{
    phi[1] = 1;
    for(int i = 2; i <= 1e6; ++i)
    {
        if(!vis[i]) prime[++cnt] = i, phi[i] = i - 1;
        for(int j = 1; j <= cnt; ++j)
        {
            int d = i * prime[j];
            if(d > 1e6) break;
            vis[d] = 1;
            if(i % prime[j] == 0){ phi[d] = phi[i] * prime[j]; break; }
            else phi[d] = phi[i] * (prime[j] - 1);
        }
    }
    have[2] = have[4] = 1;
    for(int i = 2; i <= cnt; ++i)
    {
        for(ll j = prime[i]; j <= 1e6; j *= prime[i]) have[j] = 1;
        for(ll j = prime[i] * 2; j <= 1e6; j *= prime[i]) have[j] = 1;
    }
}

int gcd(int x, int y)
{
    if(x < y) swap(x, y);
    while(y) x %= y, swap(x, y);
    return x;
}

ll qpow(ll a, ll b, ll mod)
{
    ll ans = 1;
    while(b)
    {
        if(b & 1) ans = ans * a % mod;
        b >>= 1;
        a = a * a % mod;
    }
    return ans;
}

void getp(int n)
{
    for(int i = 2; i * i <= n; ++i)
    {
        if(n % i == 0)
        {
            p[++tot] = i;
            while(n % i == 0) n /= i;
        }
    }
    if(n > 1) p[++tot] = n;
}

bool check(int x, int n)
{
    if(qpow(x, phi[n], n) != 1) return false;
    for(int i = 1; i <= tot; ++i)
        if(qpow(x, phi[n] / p[i], n) == 1) return false;
    return true;
}

int getg(int n)
{
    for(int i = 1; i < n; ++i)
        if(check(i, n)) return i;
    return 0;
}

void solve(int G, int n)
{
    for(int i = 1, now = G; i <= phi[n]; ++i, now = 1ll * now * G % n)
        if(gcd(i, phi[n]) == 1) g[++num] = now;
}

int main()
{
    into();
    int T = read();
    while(T--)
    {
        int n = read(), d = read();
        if(have[n])
        {
            tot = num = 0;
            getp(phi[n]);
            int G = getg(n);
            solve(G, n);
            sort(g + 1, g + num + 1);
            printf("%d\n", num);
            for(int i = 1; i <= num / d; ++i) printf("%d ", g[i * d]);
            printf("\n");
        }else printf("0\n\n");
    }

    return 0;
}