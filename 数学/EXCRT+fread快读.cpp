#include<bits/stdc++.h>
using namespace std;

#define ll __int128
// #define ll long long
#define ull unsigned long long

char buf[1 << 23], *p1 = buf, *p2 = buf;
#define Get() ((p1 == p2) && (p2 = (p1 = buf) + fread(buf, 1, 1 << 23, stdin), p1 == p2) ? EOF : *p1++)

ll read()
{
    ll x = 0; bool f = false; char c = Get();
    while(c != EOF && c < '0') f |= (c == '-'), c = Get();
    while(c != EOF && c >= '0') x = (x << 1) + (x << 3) + (c & 15), c = Get();
    return f ? -x : x;
}

const int N = 1e5 + 5;
int n;
ll a[N], m[N], ans, x, y, M = 1; 

ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if(!b){ x = 1, y = 0; return a; }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}

void solve()
{
    n = read();
    for(int i = 1; i <= n; ++i) m[i] = read(), a[i] = read(), M = M / exgcd(M, m[i], x, y) * m[i];
    ll X = a[1], lcm = m[1];
    for(int i = 2; i <= n; ++i)
    {
        ll d = exgcd(lcm, m[i], x, y);
        if((a[i] - X) % d != 0) return ;
        x *= (a[i] - X) / d;
        x = (x % (m[i] / d) + (m[i] / d)) % (m[i] / d);
        X = (X + x * lcm) % M;
        lcm = lcm / d * m[i];
    }
    printf("%lld\n", X);
}

int main()
{
    solve();
    return 0;
}