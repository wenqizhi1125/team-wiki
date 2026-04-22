namespace o1_inv
{
    // O(p^{2/3}) 预处理，O(1) 求逆元
    /*
    使用方法：
    定义mod，默认为998244353
    调用init()，预处理

    求inv(n) 直接调用
    */
    const int mod = 998244353;
    const int N = (1 << 21) | 1, B = 1024;
    int K, p[N * 2], *iv = p + N;
    struct M
    {
        int x, d;
    }mp[N];
    int inv(int n)
    {
        M d = mp[n >> 10];
        return 1ll * iv[n * d.x - d.d ] * (mod + d.x ) % mod;
    }
    void init(int mod = 998244353)
    {
        K = mod / B;
        int R = mod - K;
        for(int i = 1; i <= B; ++i)
        {
            int t1 = 0, t2 = i * B;
            for(int j = 0; j <= K; )
            {
                if(t1 <= K) mp[j].x = i;
                else if(t1 > R) mp[j].x = -i;
                else
                {
                    int A = (R - t1) / t2;
                    t1 += A * t2, j += A;
                }
                t1 += t2, ++j;
                if(t1 >= mod) t1 -= mod;
            }
        }
        int cnt = 0;
        for(int i = 1; i <= K; ++i)
        {
            if(mp[i].x > 0) mp[i].d = 1ll * mp[i].x * i * B / mod * mod, ++cnt;
            else mp[i].d = 1ll * mp[i].x * i * B / mod * mod - mod, ++cnt;
        }
        iv[1] = 1;
        for(int i = 2; i < N; ++i) iv[i] = 1ll * iv[mod % i] * (mod - mod / i) % mod;
        for(int i = 1; i < N; ++i) iv[-i] = mod - iv[i];
    }
}

using namespace o1_inv;