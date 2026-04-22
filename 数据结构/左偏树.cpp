#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long
#define i128 __int128

ll read()
{
    ll x = 0; bool f = false; char c = getchar();
    while(c < '0' || c > '9') f |= (c == '-'), c = getchar();
    while(c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

const int N = 1e5 + 5;
int n, m;
int a[N], f[N];
bool vis[N];

int find(int x){ return (x == f[x]) ? x : (f[x] = find(f[x])); }

void merge(int x, int y)
{
    x = find(x), y = find(y);
    if(x == y) return ;
    if(a[x] > a[y]) swap(x, y);
    f[y] = x;
}

int dis[N], ls[N], rs[N];

int Merge(int x, int y)
{
    if(!x || !y) return x | y;
    if(a[x] > a[y] || (a[x] == a[y] && x > y)) swap(x, y);
    rs[x] = Merge(rs[x], y);
    if(dis[ls[x]] < dis[rs[x]]) swap(ls[x], rs[x]);
    dis[x] = dis[rs[x]] + 1;
    return x;
}

int main()  
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i) a[i] = read(), f[i] = i, vis[i] = 1;
    while(m--)
    {
        int op = read();
        if(op == 1)
        {
            int x = read(), y = read();
            if(!vis[x] || !vis[y]) continue;
            x = find(x), y = find(y);
            if(x == y) continue;
            f[x] = f[y] = Merge(x, y);
        }else
        {
            int x = read();
            if(!vis[x]){ printf("-1\n"); continue; }
            x = find(x);
            printf("%d\n", a[x]);
            vis[x] = 0;
            f[x] = f[ls[x]] = f[rs[x]] = Merge(ls[x], rs[x]);
        }
    }
    return 0;
}