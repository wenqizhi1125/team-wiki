namespace manacher
{
    // N为要处理的字符串长度
    const int N = 1.1e7 + 5;
    char s[N << 1];
    int R[N << 1], ans;

    // 输入从 0 开始的字符串
    // R[i]-1 为以这个字符为回文中心的回文长度
    void solve(char *a)
    {
        ans = 0;
        int len = strlen(a), Len = 1;
        //形如 # | c | c | c | c |
        s[0] = '#', s[1] = '|';
        for(int i = 0; i < len; ++i) s[++Len] = a[i], s[++Len] = '|'; 
        for(int i = 1, mid = 0, r = 0; i <= Len; ++i)
        {
            if(i <= r) R[i] = min(R[(mid << 1) - i], r - i + 1);
            while(s[i + R[i]] == s[i - R[i]]) ++R[i];
            if(R[i] + i - 1 > r) r = R[i] + i - 1, mid = i;
            ans = max(ans, R[i] - 1);
        }
    }
}