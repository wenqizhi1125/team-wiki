// 建回文自动机，考虑fail的本质是当前回文串的最长回文后缀
// 那么沿着fail树向下走，走过几个节点就是有几个以当前字符结尾的后缀
// 每个节点多维护一个num，建树求fail的时候顺便继承就好了
const int N = 5e5 + 5;
char s[N];
int n, tot, last, ch[N][26], fail[N], len[N], num[N];

int getfail(int x, int n)
{
    while(s[n - len[x] - 1] != s[n]) x = fail[x];
    return x;
}

int main()
{
    scanf("%s", s + 1);
    n = strlen(s + 1);
    s[0] = -1, fail[0] = 1, len[1] = -1, tot = 1;
    int ans = 0;
    for(int i = 1; i <= n; ++i)
    {
        s[i] = (s[i] + ans - 97) % 26;
        int now = getfail(last, i);
        if(!ch[now][s[i]])
        {
            len[++tot] = len[now] + 2;
            fail[tot] = ch[getfail(fail[now], i)][s[i]];
            num[tot] = num[fail[tot]] + 1;
            ch[now][s[i]] = tot;
        }
        last = ch[now][s[i]];
        printf("%d ", ans = num[last]);
    }

    return 0;
}