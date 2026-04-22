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

const int N = 1e5 + 5;
ll jc[N], inv[N];

void init(ll mod)
{
    jc[0] = jc[1] = inv[0] = inv[1] = 1;
    for(int i = 2; i < mod; ++i) jc[i] = jc[i - 1] * i % mod;
    inv[mod - 1] = qpow(jc[mod - 1], mod - 2, mod);
    for(int i = mod - 2; i >= 2; --i) inv[i] = inv[i + 1] * (i + 1) % mod;
}

ll C(ll n, ll m, ll mod)
{
    if(n < 0 || m < 0 || n < m) return 0;
    return jc[n] * inv[m] % mod * inv[n - m] % mod;
}

ll Lucas(ll n, ll m, ll mod)
{
    if(m == 0) return 1;
    return Lucas(n / mod, m / mod, mod) * C(n % mod, m % mod, mod) % mod;
}

void solve()
{
    ll n = read(), m = read(), mod = read();
    init(mod);
    printf("%lld\n", Lucas(n + m, n, mod));
}

int main()
{
    int T = read();
    while(T--) solve();
    return 0;
}