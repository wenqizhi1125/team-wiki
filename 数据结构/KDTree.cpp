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

// 求第K远点对的距离
const int N = 1e5 + 10;
int n, K;
int sta[N];
struct dot { ll x, y; } dian[N];
bool cmpx(int a, int b) { return dian[a].x < dian[b].x ; }
bool cmpy(int a, int b) { return dian[a].y < dian[b].y ; }
double pow2(double a) { return a * a; }
ll Pow(ll a) { return a * a; } // double的精度误差好大，唉，还得写ll

int id[N], ls[N], rs[N], tot, root;
int xup[N], xdown[N], yup[N], ydown[N];

void pushup(int k)
{
    xup[k] = xdown[k] = dian[id[k]].x ;
    yup[k] = ydown[k] = dian[id[k]].y ;
    if(ls[k])
    {
        xup[k] = max(xup[k], xup[ls[k]]);
        xdown[k] = min(xdown[k], xdown[ls[k]]);
        yup[k] = max(yup[k], yup[ls[k]]);
        ydown[k] = min(ydown[k], ydown[ls[k]]);
    }
    if(rs[k])
    {
        xup[k] = max(xup[k], xup[rs[k]]);
        xdown[k] = min(xdown[k], xdown[rs[k]]);
        yup[k] = max(yup[k], yup[rs[k]]);
        ydown[k] = min(ydown[k], ydown[rs[k]]);
    }
}

int get(int l, int r)
{  
    double sumx = 0, sumy = 0, avx = 0, avy = 0;
    for (int i = l; i <= r; ++i)
        avx += dian[sta[i]].x , avy += dian[sta[i]].y ;
    avx /= (double)(r - l + 1), avy /= (double)(r - l + 1);
    for (int i = l; i <= r; ++i)
        sumx += pow2(avx - dian[sta[i]].x ), sumy += pow2(avy - dian[sta[i]].y );
    if(sumx > sumy) return 1;
    else return 0;
}

void build(int &k, int l, int r)
{
    if(l > r) return ;
    k = ++tot;
    int mid = (l + r) >> 1;
    if(get(l, r))
        nth_element(sta + l, sta + mid, sta + r + 1, cmpx);
    else   
        nth_element(sta + l, sta + mid, sta + r + 1, cmpy);
    id[k] = sta[mid];
    build(ls[k], l, mid - 1), build(rs[k], mid + 1, r);
    pushup(k);
}

priority_queue<ll, vector<ll>, greater<ll> > xx;
ll x, y;
ll getmax(int k)
{
    ll dis = 0;
    dis += Pow(max(abs(xup[k] - x), abs(xdown[k] - x)));
    dis += Pow(max(abs(yup[k] - y), abs(ydown[k] - y)));
    return dis;
}

void query(int k)
{
    if(k == 0) return ;
    ll dis = Pow(dian[id[k]].x - x) + Pow(dian[id[k]].y - y);
    if(dis > xx.top()) xx.pop(), xx.push(dis);
    ll l = getmax(ls[k]), r = getmax(rs[k]);
    if(l > r)
    {
        if(l >= xx.top()) query(ls[k]);
        if(r >= xx.top()) query(rs[k]);
    }else
    {
        if(r >= xx.top()) query(rs[k]);
        if(l >= xx.top()) query(ls[k]);
    }
}

int main()
{
    n = read(), K = read();
    for (int i = 1; i <= n; ++i)
        dian[i].x = read(), dian[i].y = read(), sta[i] = i;
    K *= 2;
    while (K--) xx.push(-1);
    build(root, 1, n);
    for (int i = 1; i <= n; ++i)
    {
        x = dian[i].x , y = dian[i].y ;
        query(root);
    }
    printf ("%lld\n", xx.top());
    return 0;
}