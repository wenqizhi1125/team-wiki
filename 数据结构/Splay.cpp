#include <bits/stdc++.h>
#define ll long long
#define ls(x) son[x][0]
#define rs(x) son[x][1]
using namespace std;
int read()
{
	int x = 0, f = 0; char c = getchar();
	while (c < '0' || c > '9') f = c == '-', c = getchar();
	while (c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar(); 
	return f ? -x : x;
}

const int N = 1e6 + 10;
int f[N], val[N], Size[N], cnt[N], son[N][2], tot, root, inf = 0x7fffffff;

void clean(int k)
{
	f[k] = ls(k) = rs(k) = Size[k] = val[k] = cnt[k] = 0;
}

void updata(int k)
{
	if(k)
	{
		Size[k] = cnt[k];
		if(ls(k)) Size[k] += Size[ls(k)];
		if(rs(k)) Size[k] += Size[rs(k)];
	}
}

bool get(int k)
{
	return k == rs(f[k]);
}

void rotata(int k)
{
	int fa = f[k], ffa = f[fa], w = get(k);
	son[fa][w] = son[k][w ^ 1];
	if(son[k][w ^ 1]) f[son[k][w ^ 1]] = fa;
	son[k][w ^ 1] = fa, f[fa] = k;
	f[k] = ffa;
	if(ffa) son[ffa][rs(ffa) == fa] = k;
	updata(fa), updata(k);
}

void splay(int k, int d)
{
	for (int fa; fa = f[k], fa != d; rotata(k))
		if(f[fa] != d) rotata((get(k) == get(fa)) ? fa : k); 
	if(d == 0) root = k, f[root] = 0;
}

void insert(int v)
{
	if(root == 0)
	{
		++tot;
		ls(tot) = rs(tot) = f[tot] = 0, val[tot] = v, Size[tot] = cnt[tot] = 1;
		root = tot;
		return ;
	}
	
	int now = root, fa = 0;
	while (1)
	{
		if(val[now] == v)
		{
			cnt[now]++;
			updata(now), updata(fa), splay(now, 0);
			return ;
		}
		fa = now, now = son[now][v > val[now]];
		if(now == 0)
		{
			++tot;
			ls(tot) = rs(tot) = 0;
			f[tot] = fa, son[fa][v > val[fa]] = tot;
			val[tot] = v, cnt[tot] = Size[tot] = 1;
			updata(fa), splay(tot, 0);
			return ;
		}
	}	
}

int getrank(int v)
{
	int ans = 0, now = root;
	while (1)
	{
		if(now == 0) return 0; 
		if(v < val[now]) now = ls(now);
		else
		{
			ans += Size[ls(now)];
			if(v == val[now]) return splay(now, 0), ans + 1;
			ans += cnt[now];
			now = rs(now);
		}
	}
}

int getval(int k)
{
	int now = root;
	while (1)
	{
		if(k <= Size[ls(now)]) now = ls(now);
		else
		{
			k -= Size[ls(now)];
			if(k <= cnt[now]) return val[now];
			k -= cnt[now];
			now = rs(now);
		}
	}
}

int getqian(int v)
{
	getrank(v);
	int now = ls(root);
	if(now == 0) return -inf;
	while (rs(now)) now = rs(now);
	return now;
}

int gethou(int v)
{
	getrank(v);
	int now = rs(root);
	if(now == 0) return inf;
	while (ls(now)) now = ls(now);
	return now;
}

void del(int v)
{
	getrank(v);
	
	if(cnt[root] > 1)
	{
		cnt[root]--; updata(root);
		return ;
	}
	
	if(!ls(root) && !rs(root))
	{
		clean(root);
		root = 0;
		return ;
	} 
	
	if(!rs(root))
	{
		int old = root;
		root = ls(root), f[root] = 0;
		clean(old); return ;
	}
	
	if(!ls(root))
	{
		int old = root;
		root = rs(root), f[root] = 0;
		clean(old); return ;
	}
	
	int qian = getqian(val[root]), old = root;
	splay(qian, 0);
	rs(root) = rs(old), f[rs(old)] = root;
	clean(old); updata(root); return ;
}

int main ()
{
	int t = read();
	while (t--)
	{
		int opt = read(), x = read();
		switch(opt)
		{
			case 1: insert(x); break;
			case 2: del(x); break;
			case 3: printf ("%d\n", getrank(x)); break;
			case 4: printf ("%d\n", getval(x)); break;
			case 5: insert(x), printf ("%d\n", val[getqian(x)]), del(x); break;
			case 6: insert(x), printf ("%d\n", val[gethou(x)]), del(x); break;
		}
	}
	return 0;
}