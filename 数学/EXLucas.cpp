const int N = 55;
ll prime[N], cnt[N], tot;
ll xx, yy;
ll x[N], mod[N];

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

ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if(!b){ x = 1, y = 0; return a; }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}

ll excrt(ll x[], ll mod[], int n)
{
    ll M = mod[1], X = x[1], lcm = mod[1];
    for(int i = 2; i <= n; ++i)
    {
        ll d = exgcd(lcm, mod[i], xx, yy);
        xx *= (x[i] - X) / d;
        xx = (xx % (mod[i] / d) + (mod[i] / d)) % (mod[i] / d);
        M = lcm / d * mod[i];
        X = (X + xx * lcm) % M;
        lcm = M;
    }
    return X;
}

void into(ll n)
{
    for(ll i = 2; i * i <= n; ++i)
    {
        if(n % i == 0)
        {
            prime[++tot] = i;
            while(n % i == 0) ++cnt[tot], n /= i;
        }
    }
    if(n != 1) prime[++tot] = n, cnt[tot] = 1;
}

ll solve(ll n, ll p, ll pk)
{
    if(n == 0) return 1;
    ll ans = solve(n / p, p, pk), tmp = 1, lim = n % pk;
    for(ll i = 1; i < pk; ++i) if(i % p != 0) tmp = tmp * i % pk;
    tmp = qpow(tmp, n / pk, pk);
    ans = ans * tmp % pk;
    for(ll i = 1; i <= lim; ++i) if(i % p != 0) ans = ans * i % pk;
    return ans;
}

ll get(ll n, ll p)
{
    ll ans = 0;
    for(ll now = p; now <= n; now = now * p) ans += n / now;
    return ans;
}

ll exlucas(ll n, ll m, ll p)
{
    if(n < 0 || m < 0 || n < m) return 0;
    into(p);
    for(int i = 1; i <= tot; ++i)
    {
        mod[i] = qpow(prime[i], cnt[i], 9223372036854775807);
        ll a = solve(n, prime[i], mod[i]);
        ll b = solve(m, prime[i], mod[i]);
        ll c = solve(n - m, prime[i], mod[i]);
        x[i] = qpow(prime[i], get(n, prime[i]) - get(m, prime[i]) - get(n - m, prime[i]), mod[i]);
        exgcd(b, mod[i], xx, yy);
        ll inv1 = (xx % mod[i] + mod[i]) % mod[i];
        exgcd(c, mod[i], xx, yy);
        ll inv2 = (xx % mod[i] + mod[i]) % mod[i];
        x[i] = x[i] * a % mod[i] * inv1 % mod[i] * inv2 % mod[i];
    }
    return excrt(x, mod, tot);
}

int main()
{
    ll n = read(), m = read(), p = read();
    printf("%lld\n", exlucas(n, m, p));
    return 0;
}