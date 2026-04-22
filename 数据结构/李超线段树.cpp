#include<bits/stdc++.h>
using namespace std;

#define int ll
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

const double eps = 1e-8;
const int N = 1e5 + 10;
int n;

struct line{ double k, b; } xian[N];

double calc(const line &a, const double &x)
{ return a.k * x + a.b; }

int check(const double &x, const double &y)
{
    if(x - y > eps) return 1;
    if(y - x > eps) return -1;
    return 0;
}

struct LCT
{
    #define ls(x) (x << 1)
    #define rs(x) (x << 1 | 1)

    int vis[N << 2];

    LCT(){ memset(vis, 0, sizeof(vis)); }

    // 这个也是初始化vis
    // void build(int k, int l, int r)
    // {
    //     vis[k] = 0;
    //     if(l == r) return ;

    //     int mid = (l + r) >> 1;

    //     build(ls(k), l, mid);
    //     build(rs(k), mid + 1, r);
    // }

    void update(int k, int l, int r, int id)
    {
        if(vis[k] == 0){ vis[k] = id; return ; }

        int mid = (l + r) >> 1;

        if(check(calc(xian[id], mid), calc(xian[vis[k]], mid)) == 1) swap(vis[k], id);

        int dl = check(calc(xian[id], l), calc(xian[vis[k]], l));
        int dr = check(calc(xian[id], r), calc(xian[vis[k]], r));

        if(dl == 1 || (dl == 0 && id < vis[k])) update(ls(k), l, mid, id);
        if(dr == 1 || (dr == 0 && id < vis[k])) update(rs(k), mid + 1, r, id);
    }

    void motify(int k, int l, int r, int L, int R, int id)
    {
        if(L <= l && r <= R){ update(k, l, r, id); return ; }
        
        int mid = (l + r) >> 1;

        if(L <= mid) motify(ls(k), l, mid, L, R, id);
        if(R > mid) motify(rs(k), mid + 1, r, L, R, id);
    }

    int query(int k, int l, int r, int pos)
    {
        if(l == r) return vis[k]; 

        int mid = (l + r) >> 1, ans = 0;

        if(pos <= mid) ans = query(ls(k), l, mid, pos);
        if(pos > mid) ans = query(rs(k), mid + 1, r, pos);

        int x = check(calc(xian[vis[k]], pos), calc(xian[ans], pos));

        if(x == 1) return vis[k];
        if(x == -1) return ans;
        else return min(vis[k], ans);
    }
}T;

int main()
{
    n = read();
    int last = 0, num = 0;
    xian[0].b = -0x7ffffffffffffff;

    for(int i = 1; i <= n; ++i)
    {
        int opt = read();
        if(opt == 0)
        {
            ll p = read();
            p = (p + last - 1 + 39989) % 39989 + 1;

            last = T.query(1, 1, 39989, p);
            
            printf("%d\n", last);
        }
        else
        {   
            ++num;
            ll s1 = read(), s2 = read(), s3 = read(), s4 = read();
            s1 = (s1 + last - 1 + 39989) % 39989 + 1;
            s2 = (s2 + last - 1 + 1000000000) % 1000000000 + 1;
            s3 = (s3 + last - 1 + 39989) % 39989 + 1;
            s4 = (s4 + last - 1 + 1000000000) % 1000000000 + 1;

            if(s1 > s3) swap(s1, s3), swap(s2, s4);

            if(s1 == s3) xian[num].k = 0, xian[num].b = max(s2, s4);
            else xian[num].k = (double)(s4 - s2) / (double)(s3 - s1), xian[num].b = 1.0 * s4 - xian[num].k * s3;
            
            T.motify(1, 1, 39989, s1, s3, num);
        }
    }

    return 0;
}
