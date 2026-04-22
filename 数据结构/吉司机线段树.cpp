// 线段树3
// 需要支持的操作：
// 修改：区间加，区间取min，区间求和
// 查询：区间最大值，区间历史最大值
// 瓶颈在于区间取min
// 引入区间最大值，次大值，最大值的次数
// 实际上是把区间的数分为了最大值和非最大值
// 线段树需要维护的值有
// 区间和sum，区间最大值fi，区间次大值se, 区间历史最大值ma，区间最大值的次数cnt
// 区间最大值标记la1，区间非最大值标记la2，区间最大值历史标记la3，区间非最大值历史标记la4

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

const ll inf = 0x7fffffffff;
const int N = 5e5 + 10;
int n, m, a[N];

#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)
struct node
{
    ll fi, se, ma, cnt, sum;
    node(){ fi = se = ma = cnt = sum = 0; }
    node(ll fi, ll se, ll ma, ll cnt, ll sum)
    :fi(fi), se(se), ma(ma), cnt(cnt), sum(sum){}
}T[N << 2];
ll la1[N << 2], la2[N << 2], la3[N << 2], la4[N << 2], len[N << 2];

node operator + (const node & a, const node & b)
{
    node c;
    c.sum = a.sum + b.sum ;
    c.ma = max(a.ma , b.ma );
    c.fi = max(a.fi , b.fi );
    if(a.fi == b.fi )
    {
        c.cnt = a.cnt + b.cnt ;
        c.se = max(a.se , b.se );
    }else if(a.fi > b.fi )
    {
        c.cnt = a.cnt ;
        c.se = max(a.se , b.fi );
    }else 
    {
        c.cnt = b.cnt ;
        c.se = max(a.fi , b.se );
    }
    return c;
}

void build(int k, int l, int r)
{
    len[k] = r - l + 1;
    if(l == r)
    {
        T[k] = node(a[l], -inf, a[l], 1, a[l]);
        return ;
    }
    int mid = (l + r) >> 1;
    build(ls(k), l, mid);
    build(rs(k), mid + 1, r);
    T[k] = T[ls(k)] + T[rs(k)];
}

void update(int k, ll s1, ll s2, ll s3, ll s4)
{
    T[k].sum += T[k].cnt * s1 + (len[k] - T[k].cnt ) * s2;
    T[k].ma = max(T[k].ma , T[k].fi + s3); // 先更新一下历史最大值，用当前最大值加历史最大标记
    T[k].fi += s1;
    if(T[k].se != -inf) T[k].se += s2; // 判断一下有没有次小值
    la3[k] = max(la3[k], la1[k] + s3);
    la4[k] = max(la4[k], la2[k] + s4);
    la1[k] += s1;
    la2[k] += s2;
}

void pushdown(int k)
{
    ll maxx = max(T[ls(k)].fi , T[rs(k)].fi );
    if(T[ls(k)].fi == maxx)
        update(ls(k), la1[k], la2[k], la3[k], la4[k]);
    else
        update(ls(k), la2[k], la2[k], la4[k], la4[k]);
    if(T[rs(k)].fi == maxx)
        update(rs(k), la1[k], la2[k], la3[k], la4[k]);
    else
        update(rs(k), la2[k], la2[k], la4[k], la4[k]);
    // 注意左右儿子，不要写错
    la1[k] = la2[k] = la3[k] = la4[k] = 0;
} 

void add(int k, int l, int r, int L, int R, ll val) 
{ 
    if(L <= l && r <= R) 
    { 
        update(k, val, val, val, val); // 区间加，直接update
        return ; 
    } 
    pushdown(k); 
    int mid = (l + r) >> 1; 
    if(L <= mid) add(ls(k), l, mid, L, R, val); 
    if(R > mid) add(rs(k), mid + 1, r, L, R, val); 
    T[k] = T[ls(k)] + T[rs(k)]; 
} 

void change(int k, int l, int r, int L, int R, ll val) 
{ 
    if(T[k].fi <= val) return ; 
    if(L <= l && r <= R && T[k].se < val) 
    { 
        // 这里直接update就好了
        update(k, val - T[k].fi , 0, val - T[k].fi , 0);
        // 注意你要加什么值，还有，注意赋值的先后顺序，最后赋T[k].fi
        // T[k].sum -= T[k].cnt * (T[k].fi - val); 
        // la1[k] -= (T[k].fi - val), T[k].fi = val;
        return ; 
    } 
    pushdown(k); 
    int mid = (l + r) >> 1; 
    if(L <= mid) change(ls(k), l, mid, L, R, val); 
    if(R > mid) change(rs(k), mid + 1, r, L, R, val); 
    T[k] = T[ls(k)] + T[rs(k)]; 
}

node query(int k, int l, int r, int L, int R)
{
    if(L <= l && r <= R) return T[k];
    pushdown(k);
    int mid = (l + r) >> 1;
    if(R <= mid) return query(ls(k), l, mid, L, R);
    if(L > mid) return query(rs(k), mid + 1, r, L, R);
    else return query(ls(k), l, mid, L, R) + query(rs(k), mid + 1, r, L, R);
}

int main()
{
    n = read(), m = read();
    for (int i = 1; i <= n; ++i) a[i] = read();
    build(1, 1, n);
    while (m--)
    {
        int opt = read(), l = read(), r = read();
        if(opt == 1) add(1, 1, n, l, r, read());
        if(opt == 2) change(1, 1, n, l, r, read());
        if(opt == 3) printf ("%lld\n", query(1, 1, n, l, r).sum );
        if(opt == 4) printf ("%lld\n", query(1, 1, n, l, r).fi );
        if(opt == 5) printf ("%lld\n", query(1, 1, n, l, r).ma );
    }
    return 0;
}
