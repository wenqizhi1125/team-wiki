namespace Ksum
{
    const ll mod = 1e9 + 7;
    const int N = 1e6 + 5;
    ll x[N], y[N], suf[N], pre[N], jc[N], inv[N];
    
    /*
        使用方法：
        定义 mod 与 N
        调用solve(n,k)
    */

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
    ll solve(ll n, ll K)
    {
        for(int i = 0; i <= K + 4; ++i)
        {
            x[i] = y[i] = suf[i] = pre[i] = jc[i] = inv[i] = 0;
        }
        for(int i = 1; i <= K + 2; ++i) y[i] = (y[i - 1] + qpow(i, K, mod)) % mod;
        suf[K + 3] = pre[0] = jc[0] = jc[1] = inv[0] = inv[1] = 1;
        for(int i = 1; i <= K + 2; ++i) pre[i] = (n - i + mod) % mod * pre[i - 1] % mod;
        for(int i = K + 2; i >= 1; --i) suf[i] = (n - i + mod) % mod * suf[i + 1] % mod;
        for(int i = 2; i <= K + 2; ++i) jc[i] = jc[i - 1] * i % mod, inv[i] = (mod - mod / i) * inv[mod % i] % mod;
        for(int i = 2; i <= K + 2; ++i) inv[i] = inv[i - 1] * inv[i] % mod;
        ll ans = 0;
        for(int i = 1; i <= K + 2; ++i)
            ans += (pre[i - 1] * suf[i + 1] % mod * inv[i - 1] % mod * inv[K + 2 - i] % mod * y[i] % mod) * (((K + 2 - i) & 1) ? -1 : 1);
        printf("%lld\n", (ans % mod + mod) % mod);
    }

}