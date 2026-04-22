const int N = 2e6 + 5;
int n;
char s[N];

struct SAM
{
    int ch[N][26], len[N], f[N], tot;
    int cnt[N]; 
    
    void into(){ tot = 1; }
    
    int add(int c, int last)
    {
        int p = last, now = ++tot;
        len[now] = len[p] + 1, cnt[now] = 1;
        for(; p && !ch[p][c]; p = f[p]) ch[p][c] = now;
        if(!p){ f[now] = 1; return now; }
        int q = ch[p][c];
        if(len[q] == len[p] + 1){ f[now] = q; return now; }
        int clone = ++tot;
        len[clone] = len[p] + 1;
        for(int i = 0; i <= 25; ++i) ch[clone][i] = ch[q][i];
        for(; p && ch[p][c] == q; p = f[p]) ch[p][c] = clone;
        f[clone] = f[q], f[q] = f[now] = clone;
        return now;
    }

    int tong[N], id[N];
    void solve()
    {
        for(int i = 2; i <= tot; ++i) ++tong[len[i]];
        for(int i = 2; i <= tot; ++i) tong[i] += tong[i - 1];
        for(int i = 2; i <= tot; ++i) id[tong[len[i]]--] = i;
        ll ans = 0;
        for(int i = tot; i >= 2; --i)
        {
            int x = id[i];
            cnt[f[x]] += cnt[x];
            if(cnt[x] > 1) ans = max(ans, 1ll * cnt[x] * len[x]);
        }
        printf("%lld\n", ans);
    }
}sam;

int main()
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    int last = 1;
    sam.into();
    for(int i = 1; i <= n; ++i) last = sam.add(s[i] - 'a', last);
    sam.solve();
    return 0;
}