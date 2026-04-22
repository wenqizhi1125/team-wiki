// 线段树分裂
// 这道题是权值线段树，以第k小为分裂点，将一颗线段树分裂成两颗
#include<bits/stdc++.h>

#define ll long long
#define ull unsigned long long
using namespace std;

ll read()
{
    ll x = 0, f = 0; char c = getchar();
    while(c < '0') f |= (c == '-'), c = getchar();
    while(c >= '0') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

const int N = 2e5 + 10;
int n, m;
ll a[N];
#define ls(x) son[x][0]
#define rs(x) son[x][1]
int son[N << 5][2], root[N], tot, num = 1;
ll cnt[N << 5];
int sta[N << 5], top;

int node(){ return top ? sta[top--] : ++tot; }

void clear(int k){ sta[++top] = k, ls(k) = rs(k) = cnt[k] = 0; }

void motify(int &k, int l, int r, int pos, ll val)
{
    if(!k) k = node();
    cnt[k] += val;
    if(l == r) return ;
    int mid = (l + r) >> 1;
    if(pos <= mid) motify(ls(k), l, mid, pos, val);
    else motify(rs(k), mid + 1, r, pos, val);
}

ll query(int k, int l, int r, int L, int R)
{
    if(!k) return 0;
    if(L <= l && r <= R) return cnt[k]; 
    int mid = (l + r) >> 1;
    if(R <= mid) return query(ls(k), l, mid, L, R);
    if(L > mid) return query(rs(k), mid + 1, r, L, R);
    return query(ls(k), l, mid, L, R) + query(rs(k), mid + 1, r, L, R);
}

int Kth(int k, int l, int r, ll val)
{
    if(l == r) return l;
    int mid = (l + r) >> 1;
    if(val <= cnt[ls(k)]) return Kth(ls(k), l, mid, val);
    else return Kth(rs(k), mid + 1, r, val - cnt[ls(k)]);
}

void merge(int &k, int &p, int l, int r)
{
    if(!k || !p){ k += p; return ; }
    cnt[k] += cnt[p];
    if(l == r) return ;
    int mid = (l + r) >> 1;
    merge(ls(k), ls(p), l, mid);
    merge(rs(k), rs(p), mid + 1, r);
    clear(p);
}

void split(int k, int &p, ll val)
{
    if(!k) return ;
    p = node();
    if(cnt[ls(k)] < val) split(rs(k), rs(p), val - cnt[ls(k)]);
    else swap(rs(k), rs(p));
    if(cnt[ls(k)] > val) split(ls(k), ls(p), val);
    cnt[p] = cnt[k] - val;
    cnt[k] = val;
}

int main()
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i)
    {
        a[i] = read();
        if(a[i]) motify(root[1], 1, n, i, a[i]);
    }
    while(m--)
    {
        int opt = read();
        if(opt == 0)
        {
            int p = read(), x = read(), y = read();
            ll k1 = query(root[p], 1, n, 1, y), k2 = query(root[p], 1, n, x, y);
            split(root[p], root[++num], k1 - k2);
            int tmp = 0;
            split(root[num], tmp, k2);
            merge(root[p], tmp, 1, n);
        }else if(opt == 1)
        {
            int p = read(), t = read();
            merge(root[p], root[t], 1, n);
        }else if(opt == 2)
        {
            ll p = read(), x = read(), q = read();
            motify(root[p], 1, n, q, x);
        }else if(opt == 3)
        {
            int p = read(), x = read(), y = read();
            printf("%lld\n", query(root[p], 1, n, x, y));
        }else
        {
            ll p = read(), k = read();
            if(cnt[root[p]] < k) printf("-1\n");
            else printf("%d\n", Kth(root[p], 1, n, k));
        }
    }
    return 0;
}

