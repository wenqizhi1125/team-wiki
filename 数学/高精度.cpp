const int N = 1e4 + 10;
char a[N], b[N];
struct node
{
    ll ans[N];
    int len;

    node(){ memset(ans, 0, sizeof(ans)); len = 0; }

    // 字符串 转 高精度 
    void Into(char s[])
    {
        int lens = strlen(s + 1), res = lens % 8;
        for(int i = lens; i - 8 >= 0; i -= 8)
        {
            ++len;
            for(int j = i - 7; j <= i; ++j) ans[len] = (ans[len] << 1) + (ans[len] << 3) + (s[j] & 15);
        }
        if(res)
        {
            ++len;
            for(int j = 1; j <= res; ++j) ans[len] = (ans[len] << 1) + (ans[len] << 3) + (s[j] & 15);
        }
    }

    // ll型 转 高精度
    void into(ll x){ len = 0; while(x) ans[++len] = x % 100000000, x /= 100000000; }

    // 高精 × 高精 
    node friend operator * (const node &a, const node &b)
    {
        node c;
        c.len = a.len + b.len ;
        for(int i = 1; i <= a.len ; ++i)
        {
            for(int j = 1; j <= b.len ; ++j)
            {
                c.ans[i + j - 1] += a.ans[i] * b.ans[j];
                if(c.ans[i + j - 1] >= 100000000)
                {
                    c.ans[i + j] += c.ans[i + j - 1] / 100000000;
                    c.ans[i + j - 1] %= 100000000;
                }
            }
        }
        while(c.len > 1 && c.ans[c.len ] == 0) --c.len ;
        return c;
    }

    // 高精 × 高精
    void friend operator *= (node &a, const node &b)
    { a = a * b; }

    // 高精 × 低精
    node friend operator * (const node &a, const ll &b)
    {
        node c;
        c.into(b);
        return a * c;
    }

    // 高精 × 低精
    void friend operator *= (node &a, const ll &b)
    { a = a * b; }

    // 高精 + 高精
    node friend operator + (const node &a, const node &b)
    {
        node c;
        c.len = max(a.len , b.len ) + 1;
        for(int i = 1; i <= c.len ; ++i)
        {
            c.ans[i] += a.ans[i] + b.ans[i];
            if(c.ans[i] >= 100000000)
            {
                c.ans[i + 1] += c.ans[i] / 100000000;
                c.ans[i] %= 100000000;
            }
        }
        while(c.len > 1 && c.ans[c.len ] == 0) --c.len ;
        return c;
    }

    // 高精 ＋ 高精
    void friend operator += (node &a, const node &b)
    {  a = a + b; }

    // 高精 ＋ 低精
    node friend operator + (const node &a, const ll &b)
    {
        node c;
        c.into(b);
        return a + c;
    }

    // 高精 ＋ 低精
    void friend operator += (node &a, const ll &b)
    { a = a + b; }

    // 高精 与 高精 比大小 
    bool friend operator < (const node &a, const node &b)
    {
        if(a.len < b.len ) return true;
        if(a.len > b.len ) return false;
        for(int i = a.len ; i >= 1; --i)
        {
            if(a.ans[i] < b.ans[i]) return true;
            if(a.ans[i] > b.ans[i]) return false;
        }
        return false;
    }

    // 高精 与 高精 比大小
    bool friend operator <= (const node &a, const node &b)
    {
        if(a.len < b.len ) return true;
        if(a.len > b.len ) return false;
        for(int i = a.len ; i >= 1; --i)
        {
            if(a.ans[i] < b.ans[i]) return true;
            if(a.ans[i] > b.ans[i]) return false;
        }
        return true;
    }

    // 高精 与 低精 比大小
    bool friend operator < (const node &a, const ll &b)
    {
        node c;
        c.into(b);
        return a < c;
    }

    // 高精 与 低精 比大小
    bool friend operator <= (const node &a, const ll &b)
    {
        node c;
        c.into(b);
        return a <= c;
    }

    // 高精 － 高精 默认 大 － 小 
    node friend operator - (const node &a, const node &b)
    {
        node c;
        c.len = a.len ;
        for(int i = 1; i <= a.len ; ++i)
        {
            c.ans[i] += a.ans[i] - b.ans[i];
            if(c.ans[i] < 0) --c.ans[i + 1], c.ans[i] += 100000000;
        }
        while(c.len > 1 && c.ans[c.len ] == 0) --c.len ;
        return c;
    }

    // 高精 － 高精 默认 大 － 小 
    void friend operator -= (node &a, const node &b)
    { a = a - b; }

    // 高精 － 低精 默认 大 － 小
    node friend operator - (const node &a, const ll &b)
    {
        node c;
        c.into(b);
        return a - c;
    }

    // 高精 － 低精 默认 大 － 小
    void friend operator -= (node &a, const ll &b)
    { a = a - b; }

    // 高精 ÷ 低精 求商
    node friend operator / (const node &a, const ll &b)
    {
        node c;
        c.len = a.len ;
        ll res = 0;
        for(int i = a.len ; i >= 1; --i)
        {
            res *= 100000000;
            c.ans[i] = (res + a.ans[i]) / b;
            res = (res + a.ans[i]) % b;
        }
        while(c.len > 1 && c.ans[c.len ] == 0) --c.len ;
        return c;
    }

    // 高精 ÷ 低精 求商
    void friend operator /= (node &a, const ll &b)
    { a = a / b; }

    // 高精 ÷ 低精 求余数 取模 
    node friend operator % (const node &a, const ll &b)
    {
        ll res = 0;
        for(int i = a.len ; i >= 1; --i)
        {
            res *= 100000000;
            res = (res + a.ans[i]) % b;
        }
        node c;
        c.into(res);
        return c;
    }

    // 高精 ÷ 低精 求余数 取模 
    void friend operator %= (node &a, const ll &b)
    { a = a % b; }

    // 输出
    void print()
    {
        int Len = len;
        printf("%lld", ans[Len]);
        while(--Len) printf("%08lld", ans[Len]);
        printf("\n");
    }
}A, B;

int main()
{
    scanf("%s %s", a + 1, b + 1);
    A.Into(a), B.Into(b);
    A += B;
    A.print();
    return 0;
}