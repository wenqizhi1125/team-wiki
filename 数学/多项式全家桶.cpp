namespace poly
{
    #define vi vector<int>
    #define siz(a) ((int)((a).size()))
    #define int ll
    const int M = 1 << 21;
    const int mod = 998244353; // 默认模数
    int len, r[M + 5], w[2][M + 5], L;
    int qpow(int a, int b, int mod)
    {
        int ans = 1;
        while(b)
        {
            if(b & 1) ans = ans * a % mod;
            b >>= 1;
            a = a * a % mod;
        }
        return ans;
    }
    const int inv2 = qpow(2, mod - 2, mod);
    int add(int x, int y){ return (x + y >= mod ? x + y - mod : x + y); }
    void init(int n)
    {
        len = 1;
        for(; len < n; len <<= 1);
        w[0][0] = w[1][0] = 1;
        w[0][1] = qpow(3, (mod - 1) / len, mod);
        w[1][1] = qpow(w[0][1], mod - 2, mod);
        for(int i = 1; i < len; ++i)
        {
            r[i] = (r[i >> 1] >> 1) | (i & 1 ? (len >> 1) : 0);
            w[0][i] = w[0][i - 1] * w[0][1] % mod;
            w[1][i] = w[1][i - 1] * w[1][1] % mod;
        }
    }
    void NTT(vi &f, int o)
    {
        f.resize(len);
        for(int i = 0; i < len; ++i) if(i > r[i]) swap(f[i], f[r[i]]);
        for(int i = 1, q = len >> 1; i < len; i <<= 1, q >>= 1)
        {
            for(int j = 0; j < len; j += (i << 1))
            {
                for(int k = 0; k < i; ++k)
                {
                    int temp = f[j + k + i] * w[o][k * q] % mod;
                    f[j + k + i] = add(f[j + k], mod - temp);
                    f[j + k] = add(f[j + k], temp); 
                }
            }
        }
        if(o)
        {
            const int inv = qpow(len, mod - 2, mod);
            for(int i = 0; i < len; ++i) f[i] = f[i] * inv % mod;
        }
    }
    vi operator * (vi f, vi g)
    {
        int n = siz(f) + siz(g) - 1;
        init(n);
        NTT(f, 0), NTT(g, 0);
        for(int i = 0; i < len; ++i) f[i] = f[i] * g[i] % mod;
        NTT(f, 1);
        f.resize(n);
        return f;        
    }
    vi Inv(vi f)
    {
        if(siz(f) == 1) return {qpow(f[0], mod - 2, mod)};
        const int n = siz(f);
        vi g = f;
        g.resize((n + 1) / 2), g = Inv(g);
        init(n * 2), NTT(g, 0), NTT(f, 0);
        for(int i = 0; i < len; ++i) g[i] = g[i] * (mod + 2 - g[i] * f[i] % mod) % mod;
        NTT(g, 1), g.resize(n);
        return g;
    }
    vi Ln(vi f)
    {
        vi g = f;
        const int n = siz(f);
        for(int i = 0; i < n - 1; ++i) g[i] = (i + 1) * f[i + 1] % mod;
        f = Inv(f) * g;
        f.resize(n);
        for(int i = n - 1; i >= 1; --i) f[i] = f[i - 1] * qpow(i, mod - 2, mod) % mod;
        f[0] = 0;
        return f;
    }

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int rnd(int l, int r){ return uniform_int_distribution<int>(l, r)(rng); }
    #define fi first
    #define se second
    #define pii pair<int, int>
    // 二次剩余，需要保证mod是奇素数且n是二次剩余，返回较小值
    int sqrt(int n)
    {
        if(n == 0) return 0;
        if(n == 1) return 1;
        if(qpow(n, (mod - 1) / 2, mod) == mod - 1) return -1; // 表示n不是二次剩余
        int a;
        while(1)
        {
            a = rnd(1, mod - 1);
            if(qpow((a * a - n + mod) % mod, (mod - 1) / 2, mod) == mod - 1) break;
        }
        pii W = pii(a, 1), ans = pii(1, 0);
        int m = (mod + 1) / 2, t = (a * a - n + mod) % mod;
        while(m)
        {
            if(m & 1)
            {
                ans = pii((ans.fi * W.fi % mod + ans.se * W.se % mod * t % mod) % mod, 
                    (ans.fi * W.se + ans.se * W.fi ) % mod);
            }
            m >>= 1;
            W = pii((W.fi * W.fi + W.se * W.se % mod * t) % mod, (W.fi * W.se * 2) % mod);
        }
        assert(ans.se == 0);
        return min(ans.fi , (mod - ans.fi ) % mod);
    }

    vi sqrt(vi f)
    {
        if(siz(f) == 1) return {sqrt(f[0])};
        const int n = siz(f);
        vi g = f;
        g.resize((n + 1) / 2), g = sqrt(g);
        g.resize(n);
        vi h = Inv(g);
        init(n * 2), NTT(f, 0), NTT(h, 0);
        for(int i = 0; i < len; ++i) f[i] = f[i] * h[i] % mod;
        NTT(f, 1), g.resize(n);
        for(int i = 0; i < n; ++i) g[i] = add(g[i], f[i]) * inv2 % mod;
        return g;
    }
    
    vi operator * (vi f, int m)
    {
        for(int i = 0; i < siz(f); ++i) f[i] = f[i] * m % mod;
        return f;
    }

    vi operator + (vi f, int m)
    {
        f[0] = (f[0] + m) % mod;
        return f;
    }

    vi operator - (vi f, int m)
    {
        f[0] = (f[0] - m + mod) % mod;
        return f;
    }
    vi operator + (vi f, vi g)
    {
        vi res(max(siz(f), siz(g)));
        f.resize(siz(res)), g.resize(siz(res));
        for(int i = 0; i < siz(res); ++i) res[i] = add(f[i], g[i]);
        return res;
    }

    vi operator - (vi f, vi g)
    {
        for(int i = 0; i < siz(g); ++i) g[i] = mod - g[i];
        return f + g;
    }

    // 要求 f[0] = 0，否则求不了EXP
    vi Exp(vi f, int NN)
    {
        int n = 1;
        vi g(1, 1), h(1, 1), F;
        while(n < NN)
        {
            n <<= 1;
            g.resize(n);
            F = f;
            F.resize(n);
            g = g * (h - Ln(g) + F);
        }
        return g;
    }

    // f ^ k k1 = k % mod, k2 = k % (mod - 1), k3 = min(k, 1000000000)
    vi power(vi f, int k1, int k2, int k3)
    {
        const int n = siz(f);
        int j = n;
        for(int i = 0; i < n; ++i)
        {
            if(f[i])
            {
                j = i;
                break;
            }
        }
        if(j * k3 >= n) return vi(n, 0);
        for(int i = 0; i < n - j; ++i) f[i] = f[i + j];
        f.resize(n - j);
        const int p = f[0];
        for(int i = 0; i < n - j; ++i) f[i] = f[i] * qpow(p, mod - 2, mod) % mod;
        f = Ln(f);
        for(int i = 0; i < n - j; ++i) f[i] = f[i] * k1 % mod;
        f = Exp(f, siz(f));
        for(int i = 0; i < n - j; ++i) f[i] = f[i] * qpow(p, k2, mod) % mod;
        f.resize(n);
        for(int i = n - 1; i >= j * k3; --i) f[i] = f[i - j * k3];
        for(int i = 0; i < j * k3; ++i) f[i] = 0;
        return f;
    }

    vi operator / (vi f, vi g)
    {
        int n = siz(f), m = siz(g);
        reverse(f.begin(), f.end());
        reverse(g.begin(), g.end());
        f.resize(n - m + 1), g.resize(n - m + 1);
        vi res = f * Inv(g);
        res.resize(n - m + 1);
        reverse(res.begin(), res.end());
        return res;
    }

    vi operator % (vi f, vi g)
    {
        vi res = f / g;
        vi ans = f - g * res;
        ans.resize(siz(g) - 1);
        return ans;
    }
}
using namespace poly;