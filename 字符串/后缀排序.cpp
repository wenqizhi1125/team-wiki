namespace SA
{
    // sa[i]：排名为i的后缀是以sa[i]开头的后缀
    // rk[i]：以i开头的后缀排名为rk[i]
    // h[i]：排名为i的后缀和排名为i-1的后缀的LCP
    /* 
        使用方法：将字符串或数字串离散化后传入Sa函数
        其中，n为数字串长度，m为数字串值域，数字属于[0,m]
        若为字符串可直接设m=127，可以处理小写字母，大写字母，字符0~9
    */
    const int N = 1e6 + 5;
    int sa[N], rk[N], cnt[N], tp[N], h[N];
    void Qsort(int n, int m)
    {
        for(int i = 0; i <= m; ++i) cnt[i] = 0;
        for(int i = 1; i <= n; ++i) ++cnt[rk[i]];
        for(int i = 1; i <= m; ++i) cnt[i] += cnt[i - 1];
        for(int i = n; i >= 1; --i) sa[cnt[rk[tp[i]]]--] = tp[i];
    }
    
    void Sa(int *a, int n, int m)
    {
        for(int i = 1; i <= n; ++i) rk[i] = a[i], tp[i] = i;
        Qsort(n, m);
        
        for(int w = 1, p = 0; p < n; m = p, w <<= 1)
        {
            p = 0;
            for(int i = 1; i <= w; ++i) tp[++p] = n - w + i;
            for(int i = 1; i <= n; ++i) if(sa[i] > w) tp[++p] = sa[i] - w;
            Qsort(n, m);
            
            swap(rk, tp), rk[sa[1]] = p = 1;
            for(int i = 2; i <= n; ++i)
            rk[sa[i]] = (tp[sa[i - 1]] == tp[sa[i]] && tp[sa[i - 1] + w] == tp[sa[i] + w]) ? p : ++p;
        }
        
        for(int i = 1, k = 0; i <= n; ++i)
        {
            if(rk[i] == 1) continue;
            if(k) --k;
            while(a[i + k] == a[sa[rk[i] - 1] + k]) ++k;
            h[rk[i]] = k;
        }
    }
}