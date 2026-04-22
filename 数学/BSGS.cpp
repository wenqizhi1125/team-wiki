ll mod, a, b;

ll qpow(ll a, ll b, ll mod)
{
    ll ans = 1;
    while(b)
    {
        if(b & 1) (ans *= a) %= mod;
        b >>= 1;
        (a *= a) %= mod;
    }
    return ans;
}

ll BSGS(ll a, ll b, ll mod)
{
    unordered_map<ll, ll> mp; mp.clear();
    ll t = ceil(sqrt(mod)), tmp = b % mod, now = qpow(a, t, mod);
    mp[tmp] = 0;
    for(int i = 1; i <= t; ++i)
    {
        tmp = tmp * a % mod;
        mp[tmp] = i;
    }
    tmp = 1;
    for(int i = 1; i <= t; ++i)
    {
        tmp = tmp * now % mod;
        if(mp.find(tmp) != mp.end()) return i * t - mp[tmp];
    }
    return -1;
}

int main()
{
    mod = read(), a = read(), b = read();
    ll ans = BSGS(a, b, mod);
    if(ans == -1) printf("no solution\n");
    else printf("%lld\n", ans);
    return 0;
}
