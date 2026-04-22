const int N = 2e7 + 5;
char a[N], b[N];
int z[N], p[N], n, m;

void Z(char *s, int n)
{
    for(int i = 1; i <= n; ++i) z[i] = 0;
    z[1] = n;
    for(int i = 2, l = 0, r = 0; i <= n; ++i)
    {
        if(i <= r) z[i] = min(z[i - l + 1], r - i + 1);
        while(i + z[i] <= n && s[i + z[i]] == s[z[i] + 1]) ++z[i];
        if(i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
}

// 求t 与 s的所有后缀 的LCP长度（对t求Z函数）
void exkmp(char *s, int n, char *t, int m)
{
    Z(t, m);
    for(int i = 1; i <= n; ++i) p[i] = 0;
    for(int i = 1, l = 0, r = 0; i <= n; ++i)
    {
        if(i <= r) p[i] = min(z[i - l + 1], r - i + 1);
        while(i + p[i] <= n && s[i + p[i]] == t[p[i] + 1]) ++p[i];
        if(i + p[i] - 1 > r) l = i, r = i + p[i] - 1;
    }
}

int main()
{
    scanf("%s %s", a + 1, b + 1);
    int n = strlen(a + 1), m = strlen(b + 1);
    exkmp(a, n, b, m);
    ll ans1 = 0, ans2 = 0;
    for(int i = 1; i <= m; ++i) ans1 ^= 1ll * i * (z[i] + 1);
    for(int i = 1; i <= n; ++i) ans2 ^= 1ll * i * (p[i] + 1);
    printf("%lld\n%lld\n", ans1, ans2);

    return 0;   
}