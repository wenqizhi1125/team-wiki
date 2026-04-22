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

struct node
{
    ll l, r;
    // mutable 用于直接修改在set里面的值。因为it是一个常量迭代器，无法修改
    // mutable 可以突破const的限制
    mutable ll x;
    node(ll l = 0, ll r = 0, ll x = 0):l(l), r(r), x(x){}
    // 左端点排序
    bool friend operator < (const node &a, const node &b){ return a.l < b.l; }
};

set<node> s;

set<node>::iterator spilt(int pos)
{
    // 刚好是左端点，不用切割
    set<node>::iterator it = s.lower_bound(node(pos));  
    if(it != s.end() && (*it).l == pos) return it;
    --it;
    // 超出范围
    if((*it).r < pos) return s.end();
    ll l = (*it).l , r = (*it).r , x = (*it).x ;
    s.erase(it), s.insert(node(l, pos - 1, x));
    return s.insert(node(pos, r, x)).first ;
}

void assign(ll l, ll r, ll x)
{
    // 先右后左，[itl,itr)
    set<node>::iterator itr = spilt(r + 1), itl = spilt(l);
    s.erase(itl, itr);
    s.insert(node(l, r, x));
}

void add(ll l, ll r, ll x)
{
    set<node>::iterator itr = spilt(r + 1), itl = spilt(l);
    for(auto it = itl; it != itr; ++it) (*it).x += x; // mutable修饰后可以直接修改
}

ll kth(ll l, ll r, ll k)
{
    set<node>::iterator itr = spilt(r + 1), itl = spilt(l);
    vector< pair<ll, ll> > temp;
    for(auto it = itl; it != itr; ++it)
        temp.emplace_back(pair<ll, ll>((*it).x , (*it).r - (*it).l + 1));
    sort(temp.begin(), temp.end());
    for(int i = 0; i < temp.size(); ++i)
    {
        if(temp[i].second < k) k -= temp[i].second ;
        else return temp[i].first ;
    }
    return 0; // 一般不会到这行
}

ll qpow(ll a, ll b, ll mod)
{
    ll ans = 1;
    a %= mod; if(a == 0) return 0;
    while(b)
    {
        if(b & 1) ans = ans * a % mod;
        b >>= 1;
        a = a * a % mod;
    }
    return ans;
}

ll calc(ll l, ll r, ll x, ll y)
{
    auto itr = spilt(r + 1), itl = spilt(l);
    ll ans = 0;
    for(auto it = itl; it != itr; ++it)
        ans = (ans + qpow((*it).x , x, y) * ((*it).r - (*it).l + 1)) % y;
    return ans;
}

const int N = 5e5 + 5;
ll seed, mod = 1e9 + 7, vmax, n, m, a[N];

ll rnd() {
    ll ret = seed;
    seed = (seed * 7 + 13) % mod;
    return ret;
}

void solve()
{   
    n = read(), m = read(), seed = read(), vmax = read();
    for(int i = 1; i <= n; ++i)
    {
        a[i] = (rnd() % vmax) + 1;
        s.insert(node(i, i, a[i]));
    }
    for(int i = 1; i <= m; ++i)
    {
        ll op, l, r, x, y;
        op = (rnd() & 3) + 1;
        l = (rnd() % n) + 1;
        r = (rnd() % n) + 1;
        if(l > r) swap(l, r);
        if(op == 3) x = (rnd() % (r - l + 1)) + 1;
        else x = (rnd() % vmax) + 1;
        if(op == 4) y = (rnd() % vmax) + 1;
        if(op == 1) add(l, r, x);
        else if(op == 2) assign(l, r, x);
        else if(op == 3) printf("%lld\n", kth(l, r, x));
        else printf("%lld\n", calc(l, r, x, y));
    }
}   

signed main()
{
    int T = 1;
    while(T--) solve();
    return 0;
}