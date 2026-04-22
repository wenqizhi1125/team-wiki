#include <bits/stdc++.h>
#define ll long long
#define ls(x) son[x][0]
#define rs(x) son[x][1]
using namespace std;

//treap不像splay那样可以旋转到树根，一路更新，所以涉及到更新的都写成递归！！！ 
inline int read()
{
	int x(0), f(0); char c(getchar());
	while (c < '0' || c > '9') f = c == '-', c = getchar();
	while (c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
	return f ? -x : x;
}

const int N = 1e7 + 10;
int val[N], cnt[N], Size[N], son[N][2], Data[N], root, tot, inf = 0x7fffffff;

void updata(int k)
{
	if(k)
	{
		Size[k] = cnt[k];
		if(ls(k)) Size[k] += Size[ls(k)];
		if(rs(k)) Size[k] += Size[rs(k)];
	}
}

int New(int v)
{
	++tot;
	val[tot] = v, ls(tot) = rs(tot) = 0, cnt[tot] = Size[tot] = 1, Data[tot] = rand();
	return tot;
}

void build()
{
	root = New(-inf), New(inf);
	rs(root) = 2; updata(root);
}

void zuo(int &k)
{
	int p = rs(k);
	rs(k) = ls(p), ls(p) = k;
	updata(k), updata(p);
	k = p;
}

void you(int &k)
{
	int p = ls(k);
	ls(k) = rs(p), rs(p) = k;
	updata(k), updata(p);
	k = p;
}

int getrank(int v)
{
	int ans = 0, now = root;
	while (1)
	{
		if(now == 0) return ans + 1;
		if(v < val[now]) now = ls(now);
		else
		{
			ans += Size[ls(now)];
			if(v == val[now]) return ans + 1;
			ans += cnt[now];
			now = rs(now);
		}
	}
}

int getval(int k)
{
	int now = root;
	while (now)
	{
		if(k <= Size[ls(now)]) now = ls(now);
		else 
		{
			if(k <= Size[ls(now)] + cnt[now]) return val[now];
			k -= Size[ls(now)] + cnt[now];
			now = rs(now);
		}
	}
}

void insert(int &k, int v)
{
	if(k == 0){k = New(v); return ;} 
	if(val[k] == v){cnt[k]++, updata(k); return ;}
	if(v < val[k])
	{
		insert(ls(k), v);
		if(Data[ls(k)] > Data[k]) you(k);
	}else
	{
		insert(rs(k), v);
		if(Data[rs(k)] > Data[k]) zuo(k);
	}
	updata(k);
}

int getqian(int v)
{
	int ans = 1, now = root; //初始化答案为负无穷大 
	while (now)
	{
		if(val[now] == v)
		{
			if(ls(now))
			{
				now = ls(now);
				while (rs(now)) now = rs(now);
				ans = now;
			}
			break;
		}
		if(val[now] < v && val[now] > val[ans]) ans = now;
		now = son[now][v > val[now]];
	}
	return ans;
}

int gethou(int v)
{
	int ans = 2, now = root; //初始化答案为正无穷大 
	while (now)
	{
		if(val[now] == v)
		{
			if(rs(now))
			{
				now = rs(now);
				while (ls(now)) now = ls(now);
				ans = now;
			}
			break;
		} 
		if(val[now] > v && val[now] < val[ans]) ans = now;
		now = son[now][v > val[now]];
	}
	return ans;
}

void del(int &now, int v)
{
	if(now == 0) return ;
	if(val[now] == v)
	{
		if(cnt[now] > 1){cnt[now]--, updata(now); return ;}
		if(ls(now) || rs(now))
		{
			if(rs(now) == 0 || Data[ls(now)] > Data[rs(now)])
				you(now), del(rs(now), v);
			else zuo(now), del(ls(now), v);
			updata(now);
		}else now = 0;
		return ;
	}
	del(son[now][v > val[now]], v);
	updata(now);
}

int main ()
{
	build();
	int n = read();
	int opt, x;
	for (register int i(1); i <= n; ++i)
	{
		opt = read(), x = read();
		switch(opt)
		{
			case 1:
				insert(root, x);
				break;
			case 2:
				del(root, x);
				break;
			case 3:
				printf ("%d\n", getrank(x) - 1);
				break;
			case 4:
				printf ("%d\n", getval(x + 1));
				break;
			case 5:
				printf ("%d\n", val[getqian(x)]);
				break;
			case 6:
				printf ("%d\n", val[gethou(x)]);
				break;
		}
	}
	return 0;
}