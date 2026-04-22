ll a, b, p;

ll gcd(ll a, ll b)
{
    if(a < b) a ^= b, b ^= a, a ^= b;
    while (b)
    {
        a %= b;
        a ^= b, b ^= a, a ^= b;
    }
    return a;
}

ll qpow(ll a, ll b, ll p)
{
    ll ans = 1;
    while (b)
    {
        if(b & 1) ans = ans * a % p;
        b >>= 1;
        a = a * a % p;
    }
    return ans;
}

ll BSGS(ll a, ll b, ll mod, ll add)
{
    a %= mod, b %= mod;
    unordered_map<ll, ll> mp;
    mp.clear();
    ll t = ceil(sqrt(mod)), tmp = b, now = qpow(a, t, mod);
    if(now == 0) return tmp == 0 ? 1 : -1;
    
    mp[tmp] = 0;
    for (int i = 1; i <= t; ++i)
    {
        tmp = tmp * a % mod;
        mp[tmp] = i;
    }
    for (int i = 1; i <= t; ++i)
    {
        add = add * now % mod;
        if(mp.find(add) != mp.end()) return i * t - mp[add];
    }
    return -1;
}

ll EXBSGS(ll a, ll b, ll mod)
{
    a %= mod, b %= mod;
    if(b == 1 || mod == 1) return 0;
    if(a == 0) return b == 0 ? 1 : -1;
    ll k = 0, d, x = 1;
    while (d = gcd(a, mod), d != 1)
    {
        if(b % d != 0) return -1;
        ++k;
        b /= d, mod /= d, x = x * (a / d) % mod;
        if(x == b) return k;
    }
    ll ans = BSGS(a, b, mod, x);
    if(ans == -1) return -1;
    else return ans + k;
}

int main()
{
    while (a = read(), p = read(), b = read(), a != 0 && b != 0 && p != 0)
    {
        ll ans = EXBSGS(a, b, p);
        if(ans == -1) printf ("No Solution\n");
        else printf ("%lld\n", ans);
    }
    return 0;
}