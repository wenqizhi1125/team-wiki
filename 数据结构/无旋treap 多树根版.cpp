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

std::random_device rd;
mt19937 rnd(rd());

const int N = 1.1e6 + 5;
int n, m, a[N];

#define ls(x) son[x][0]
#define rs(x) son[x][1]
int w[N], son[N][2], Size[N]; // FHQ必要信息
int tot, root[N]; // FHQ必要信息
int val[N]; // 需要维护的信息，此处为权值
int sta[N], top; // 空间回收

int newnode(int v)
{
    int now = top ? sta[top--] : ++tot;
    val[now] = v, Size[now] = 1, ls(now) = rs(now) = 0, w[now] = rnd();
    return now;
}

void pushup(int k)
{
    Size[k] = Size[ls(k)] + Size[rs(k)] + 1;
}

void split(int rt, int rank, int &rt1, int &rt2)
{
    if(!rt){ rt1 = rt2 = 0; return ; }
    if(rank <= Size[ls(rt)])
    {
        split(ls(rt), rank, rt1, rt2);
        ls(rt) = rt2;
        pushup(rt);
        rt2 = rt;
    }else
    {
        split(rs(rt), rank - Size[ls(rt)] - 1, rt1, rt2);
        rs(rt) = rt1;
        pushup(rt);
        rt1 = rt;
    }
}

int merge(int rt1, int rt2)
{
    if(!rt1 || !rt2) return rt1 | rt2;
    if(w[rt1] < w[rt2])
    {
        rs(rt1) = merge(rs(rt1), rt2);
        pushup(rt1);
        return rt1;
    }else
    {
        ls(rt2) = merge(rt1, ls(rt2));
        pushup(rt2);
        return rt2;
    }
}

int mnrank(int rt, int v)
{
    if(!rt) return 1;
    if(v <= val[rt]) return mnrank(ls(rt), v);
    else return Size[ls(rt)] + 1 + mnrank(rs(rt), v);
}

int mxrank(int rt, int v)
{
    if(!rt) return 0;
    if(v < val[rt]) return mxrank(ls(rt), v);
    else return Size[ls(rt)] + 1 + mxrank(rs(rt), v);
}

void insert(int &root, int v)
{
    int now = newnode(v);
    int rk = mxrank(root, v);
    int rt1, rt2;
    split(root, rk, rt1, rt2);
    root = merge(merge(rt1, now), rt2);
}

void del(int &root, int v)
{
    int rk = mxrank(root, v);
    int rt1, rt2, rt3, temp;
    split(root, rk, rt1, rt2);
    split(rt1, rk - 1, rt3, temp);
    root = merge(rt3, rt2);

    if(temp) sta[++top] = temp; // 回收空间
}

int getkth(int &root, int k)
{
    int rt1, rt2, rt3, c;
    split(root, k, rt1, rt2);
    split(rt1, k - 1, rt3, c);
    root = merge(rt3, merge(c, rt2));
    return val[c];
}

int pre(int &root, int v)
{
    int rk = mnrank(root, v);
    return getkth(root, rk - 1);
}

int suf(int &root, int v)
{
    int rk = mxrank(root, v);
    return getkth(root, rk + 1);
}

void solve()
{
    n = read(), m = read();
    for(int i = 1; i <= n; ++i) a[i] = read(), insert(root[0], a[i]);
    int last = 0, ans = 0;
    while(m--)
    {
        int op = read(), x = read();
        x ^= last;
        if(op == 1)
        {
            insert(root[0], x);
        }else if(op == 2)
        {
            del(root[0], x);
        }else if(op == 3)
        {
            last = mnrank(root[0], x);
            ans ^= last;
        }else if(op == 4)
        {
            last = getkth(root[0], x);
            ans ^= last;
        }else if(op == 5)
        {
            last = pre(root[0], x);
            ans ^= last;
        }else
        {
            last = suf(root[0], x);
            ans ^= last;
        }
    }
    printf("%lld\n", ans);
}

signed main()
{
    int T = 1;
    while(T--) solve();
    return 0;
}