const int N = 1 << 21;
namespace poly_FFT
{
    const double Pi = acos(-1.0);
    struct node
    {
        double x, y;
        node(){ x = y = 0; }
        node(double x, double y): x(x), y(y){}
        node operator + (node a){ return node(x + a.x , y + a.y ); }
        node operator - (node a){ return node(x - a.x , y - a.y ); }
        node operator * (node a){ return node(x * a.x - y * a.y , x * a.y + y * a.x ); }
        node operator / (int a){ return node(x / a, y / a); }
    }t1[N], t2[N], t3[N], w[2][N];
    int n, L, R[N];

    void init(int A, int B)
    {   
        for(n = 1, L = 0; n < A + B; n <<= 1, ++L);
        for(int i = 0; i < n; ++i) R[i] = (R[i >> 1] >> 1) | ((i & 1) << (L - 1));
        for(int i = 0; i < n; ++i)
        {
            w[0][i] = node(cos(2.0 * i * Pi / n), sin(2.0 * i * Pi / n));
            w[1][i] = node(cos(2.0 * i * Pi / n), -sin(2.0 * i * Pi / n));
        }
    }

    void FFT(node *a, int n, int type)
    {
        for(int i = 0 ; i < n; ++i) if(R[i] > i) swap(a[R[i]], a[i]);
        for(int t = n >> 1, d = 1; d < n; t >>= 1, d <<= 1)
            for(int i = 0; i < n; i += (d << 1))
                for(int j = 0; j < d; ++j)
                {
                    node tmp = w[type][t * j] * a[i + j + d];
                    a[i + j + d] = a[i + j] - tmp;
                    a[i + j] = a[i + j] + tmp;
                }
        if(type == 1) for(int i = 0; i < n; ++i) a[i] = a[i] / n;
    }

    // FFT使用方法：传入A-1阶多项式系数和B-1阶多项式系数以及接受答案的数组（ll型）
    void mul(ll *a, int A, ll *b, int B, ll *ans)
    {
        init(A, B);
        for(int i = 0; i < A; ++i) t1[i] = node(a[i], 0);
        for(int i = 0; i < B; ++i) t2[i] = node(b[i], 0);
        for(int i = A; i < n; ++i) t1[i] = node(0, 0);
        for(int i = B; i < n; ++i) t2[i] = node(0, 0);
        FFT(t1, n, 0), FFT(t2, n, 0);
        for(int i = 0; i < n; ++i) t1[i] = t1[i] * t2[i];
        FFT(t1, n, 1);
        for(int i = 0; i < n; ++i) ans[i] = (ll)(t1[i].x + 0.5);
    }

    // 任意模数FFT：多传入一个模数，需要保证系数已经对mod取模
    ll MOD;
    ll get(double x){ return ((ll)(x + 0.5)) % MOD; }

    void mul(ll *a, int A, ll *b, int B, ll *ans, ll mod)
    {
        MOD = mod, init(A, B);
        for(int i = 0; i < A; ++i) t1[i] = node(a[i] & ((1 << 15) - 1), 0);
        for(int i = 0; i < A; ++i) t2[i] = node(a[i] >> 15, 0);
        for(int i = 0; i < B; ++i) t3[i] = node(b[i] & ((1 << 15) - 1), b[i] >> 15);
        for(int i = A; i < n; ++i) t1[i] = t2[i] = node(0, 0);
        for(int i = B; i < n; ++i) t3[i] = node(0, 0);
        FFT(t1, n, 0), FFT(t2, n, 0), FFT(t3, n, 0);
        for(int i = 0; i < n; ++i) t1[i] = t1[i] * t3[i], t2[i] = t2[i] * t3[i];
        FFT(t1, n, 1), FFT(t2, n, 1);
        for(int i = 0; i < n; ++i)
        {
            ans[i] = get(t1[i].x ) + (get(t1[i].y ) + get(t2[i].x )) * (1ll << 15) % mod;
            ans[i] = (ans[i] + (1ll << 30) % mod * get(t2[i].y ) % mod) % mod;
        }
    }
}

// NTT使用方法：定义好模数mod，原根G和inv2，需要保证系数已经对mod取模
namespace poly_NTT
{
    const ll mod = 998244353;
    const ll G = 3;
    const ll inv2 = (mod + 1) / 2;
    ll g[2][N], t1[N], t2[N], t3[N], t4[N], t5[N], t6[N], jc[N], inv[N];
    int n, L, R[N];

    ll qpow(ll a, ll b, ll mod)
    {
        ll ans = 1;
        while(b)
        {
            if(b & 1) ans = ans * a % mod;
            b >>= 1, a = a * a % mod;
        }
        return ans;
    }

    void init(int A, int B)
    {
        for(n = 1, L = 0; n < A + B; n <<= 1, ++L);
        g[1][0] = g[0][0] = 1, g[0][1] = qpow(G, (mod - 1) / n, mod);
        g[1][1] = qpow(qpow(G, mod - 2, mod), (mod - 1) / n, mod);
        for(int i = 2; i < n; ++i)
        {
            g[0][i] = g[0][i - 1] * g[0][1] % mod;
            g[1][i] = g[1][i - 1] * g[1][1] % mod;
        }
        for(int i = 0; i < n; ++i) R[i] = (R[i >> 1] >> 1) | ((i & 1) << (L - 1)); 
    }

    void NTT(ll *a, int n, int type)
    {
        for(int i = 0; i < n; ++i) if(R[i] > i) swap(a[R[i]], a[i]);
        for(int t = n >> 1, d = 1; d < n; d <<= 1, t >>= 1)
            for(int i = 0; i < n; i += (d << 1))
                for(int j = 0; j < d; ++j)
                {
                    ll temp = g[type][t * j] * a[i + j + d] % mod;
                    a[i + j + d] = (a[i + j] - temp + mod) % mod;
                    a[i + j] = (a[i + j] + temp) % mod;
                }
        if(type == 1)
        {
            ll temp = qpow(n, mod - 2, mod);
            for(int i = 0; i < n; ++i) a[i] = a[i] * temp % mod;
        }
    }

    // 传入A-1阶多项式系数和B-1阶多项式系数以及接受答案的数组（ll型）
    void mul(ll *a, int A, ll *b, int B, ll *ans)
    {
        init(A, B);
        for(int i = 0; i < A; ++i) t1[i] = a[i];
        for(int i = 0; i < B; ++i) t2[i] = b[i];
        for(int i = 0; i < n; ++i) ans[i] = 0;
        NTT(t1, n, 0), NTT(t2, n, 0);
        for(int i = 0; i < n; ++i) ans[i] = t1[i] * t2[i] % mod;
        NTT(ans, n, 1);
        for(int i = 0; i < n; ++i) t1[i] = t2[i] = 0;
    }

    // 使用 In Inv Exp Sqrt 之前调用 get(N - 1)
    void get(int N)
    {
        jc[0] = jc[1] = 1;
        for(int i = 2; i <= N; ++i) jc[i] = jc[i - 1] * i % mod;
        inv[N] = qpow(jc[N], mod - 2, mod);
        for(int i = N - 1; i >= 2; --i) inv[i] = inv[i + 1] * (i + 1) % mod;
        inv[0] = inv[1] = 1;
        for(int i = 2; i <= N; ++i) inv[i] = inv[i] * jc[i - 1] % mod;
    }

    void Dev(ll *a, ll *b, int A)
    {
        for(int i = 1; i < A; ++i) b[i - 1] = a[i] * i % mod;
        b[A - 1] = 0;
    }

    void InDev(ll *a, ll *b, int A)
    {
        for(int i = 1; i < A; ++i) b[i] = a[i - 1] * inv[i] % mod;
        b[0] = 0;
    }

    void Inv(ll *a, ll *b, int A)
    {
        b[0] = qpow(a[0], mod - 2, mod);
        for(int d = 1, t = 2; d < (A << 1); d <<= 1, t <<= 1)
        {
            for(int i = 0; i < d; ++i) t1[i] = a[i], t2[i] = b[i];
            init(d, d);
            NTT(t1, t, 0), NTT(t2, t, 0);
            for(int i = 0; i < t; ++i) t1[i] = (2ll - t1[i] * t2[i] % mod + mod) % mod * t2[i] % mod;
            NTT(t1, t, 1);
            for(int i = 0; i < d; ++i) b[i] = t1[i];
            for(int i = 0; i < t; ++i) t1[i] = t2[i] = 0;
        }
    }

    void Sqrt(ll *a, ll *b, int A)
    {
        b[0] = 1;
        for(int d = 1, t = 2; d < (A << 1); d <<= 1, t <<= 1)
        {
            for(int i = 0; i < d; ++i) t3[i] = a[i];
            Inv(b, t4, d);
            init(d, d);
            NTT(t3, t, 0), NTT(t4, t, 0);
            for(int i = 0; i < t; ++i) t3[i] = t3[i] * t4[i] % mod;
            NTT(t3, t, 1);
            for(int i = 0; i < t; ++i) b[i] = (b[i] + t3[i]) * inv2 % mod;
            for(int i = 0; i < t; ++i) t3[i] = t4[i] = 0;
        }
    }

    void In(ll *a, ll *b, int A)
    {
        Dev(a, t3, A);
        Inv(a, t4, A);
        init(A, A);
        for(int i = A; i < n; ++i) t3[i] = t4[i] = 0;
        NTT(t3, n, 0), NTT(t4, n, 0);
        for(int i = 0; i < n; ++i) t3[i] = t3[i] * t4[i] % mod;
        NTT(t3, n, 1);
        InDev(t3, b, A);
        for(int i = 0; i < n; ++i) t3[i] = t4[i] = 0;
    }

    void Exp(ll *a, ll *b, int A)
    {
        b[0] = 1;
        for(int d = 2, t = 4; d < (A << 1); d <<= 1, t <<= 1)
        {
            In(b, t6, d);
            for(int i = 0; i < d; ++i) t6[i] = ((i == 0) - t6[i] + a[i] + mod) % mod;
            for(int i = 0; i < d; ++i) t5[i] = b[i];
            init(d, d);
            NTT(t5, t, 0), NTT(t6, t, 0);
            for(int i = 0; i < t; ++i) t5[i] = t5[i] * t6[i] % mod;
            NTT(t5, t, 1);
            for(int i = 0; i < d; ++i) b[i] = t5[i];
            for(int i = 0; i < t; ++i) t5[i] = t6[i] = 0;
        }
    }
}

// FWT使用方法：定义好模数mod和2的逆元inv2，需要保证系数已经对mod取模
namespace poly_FWT
{
    const ll mod = 998244353;
    const ll inv2 = (mod + 1) / 2;
    ll t1[N], t2[N];
    int n;

    void OR(ll *a, ll type)
    {
        for(int d = 1; (d << 1) <= n; d <<= 1)
            for(int i = 0; i < n; i += (d << 1))
                for(int j = 0; j < d; ++j)
                    a[i + j + d] = (a[i + j + d] + a[i + j] * type) % mod;
    }

    void AND(ll *a, ll type)
    {
        for(int d = 1; (d << 1) <= n; d <<= 1)
            for(int i = 0; i < n; i += (d << 1))
                for(int j = 0; j < d; ++j)
                    a[i + j] = (a[i + j] + a[i + j + d] * type) % mod;
    }

    void XOR(ll *a, ll type)
    {
        for(int d = 1; (d << 1) <= n; d <<= 1)
            for(int i = 0; i < n; i += (d << 1))
                for(int j = 0; j < d; ++j)
                {
                    ll x = a[i + j], y = a[i + j + d];
                    a[i + j] = (x + y) * type % mod;
                    a[i + j + d] = (x - y + mod) * type % mod;
                }
    }

    // 传入A-1阶多项式a和B-1阶多项式b，和存储答案的数组ans
    // 一般A=B=2^n-1
    // type=1 求 | 卷积 type=2 求 & 卷积 type=3 求 ^ 卷积
    void mul(ll *a, int A, ll *b, int B, ll *ans, int type)
    {
        for(n = 1; n < max(A, B); n <<= 1);
        for(int i = 0; i < A; ++i) t1[i] = a[i];
        for(int i = 0; i < B; ++i) t2[i] = b[i];
        if(type == 1)
        {
            OR(t1, 1), OR(t2, 1);
            for(int i = 0; i < n; ++i) t1[i] = t1[i] * t2[i] % mod;
            OR(t1, mod - 1);
        }
        if(type == 2)
        {
            AND(t1, 1), AND(t2, 1);
            for(int i = 0; i < n; ++i) t1[i] = t1[i] * t2[i] % mod;
            AND(t1, mod - 1);
        }
        if(type == 3)
        {
            XOR(t1, 1), XOR(t2, 1);
            for(int i = 0; i < n; ++i) t1[i] = t1[i] * t2[i] % mod;
            XOR(t1, inv2);
        }
        for(int i = 0; i < n; ++i) ans[i] = t1[i];
        for(int i = 0; i < n; ++i) t1[i] = t2[i] = 0;
    }
}