#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long
#define i128 __int128

int read()
{
    int x = 0; bool f = false; char c = getchar();
    while(c < '0' || c > '9') f |= (c == '-'), c = getchar();
    while(c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

const int N = 2e5 + 5;
const double alpha = 0.8;

struct ST
{
    #define ls(x) son[x][0]
    #define rs(x) son[x][1]
    // 节点信息为 val, vis=cnt, Size, all;
    int root[N * 2], tot;
    int son[N * 100][2], vis[N * 100], Size[N * 100], all[N * 100];
    int val[N * 100];
    int tong[N * 100], cnt, sta[N * 100], top;
    
    ST()
    {
        tot = cnt = top = 0;
        memset(root, 0, sizeof(root));
        // memset(son, 0, sizeof(son));
        // memset(vis, 0, sizeof(vis));
        // memset(Size, 0, sizeof(Size));
        // memset(all, 0, sizeof(all));
        // memset(val, 0, sizeof(val));
        // memset(tong, 0, sizeof(tong));
        // memset(sta, 0, sizeof(sta));
        cnt = top = tot = ls(0) = rs(0) = vis[0] = Size[0] = all[0] = val[0] = 0;
    }

    void pushup(int k)
    {
        Size[k] = Size[ls(k)] + Size[rs(k)] + vis[k];
        all[k] = all[ls(k)] + all[rs(k)] + 1;
    }

    int newnode(int v, int x = 1)
    {
        int k = top ? sta[top--] : ++tot;
        val[k] = v, ls(k) = rs(k) = 0;
        vis[k] = Size[k] = x;
        all[k] = 1;
        return k; 
    }

    void dfs(int k)
    {
        if(ls(k)) dfs(ls(k));
        if(vis[k]) tong[++cnt] = k;
        else sta[++top] = k;
        if(rs(k)) dfs(rs(k));
    }

    void build(int &k, int l, int r)
    {
        k = 0; if(l > r) return ;
        int mid = (l + r) >> 1;
        k = tong[mid];
        build(ls(k), l, mid - 1), build(rs(k), mid + 1, r);
        pushup(k);
    }

    void rebuild(int &k){ cnt = 0, dfs(k), build(k, 1, cnt); }
    
    int *flag;
    void insert(int &k, int v)
    {
        if(!k){ k = newnode(v); return ; }
        if(v == val[k])
        {
            ++vis[k], ++Size[k];
            return ;
        }
        if(v < val[k]) ins(ls(k), v);
        else ins(rs(k), v);
        pushup(k);
        if(max(all[ls(k)], all[rs(k)]) >= alpha * all[k]) flag = &k;
    }

    void ins(int &k, int v)
    {
        flag = NULL;
        insert(k, v);
        if(flag != NULL) rebuild(*flag);
    }

	void del(int k, int v)
	{
        if(!k) return ;
		if(v == val[k])
		{
            if(!vis[k]) return ; 
            else --vis[k], --Size[k];
			return ;
		} 
		if(v < val[k]) del(ls(k), v);
		else del(rs(k), v);
		pushup(k);
	}

    void clear(int k)
    { 
        if(!k) return ;
        if(ls(k)) clear(ls(k));
        sta[++top] = k;
        if(rs(k)) clear(rs(k));
    }

    int getrk(int k, int v)
    {
        if(!k) return 1;
        if(v == val[k]) return Size[ls(k)] + 1;
        if(v < val[k]) return getrk(ls(k), v);
        else return getrk(rs(k), v) + Size[ls(k)] + vis[k];
    }

	int find(int k, int v = 1)
	{
        if(Size[ls(k)] >= v) return find(ls(k), v);
        if(Size[ls(k)] + vis[k] >= v) return val[k];
        return find(rs(k), v - Size[ls(k)] - vis[k]);
	}

    int getans(int k, int v){ return Size[k] - getrk(k, v) + 1; }
}T;

void solve()
{
    int n = read();
    while(n--)
    {
        int op = read(), x = read();
        if(op == 1)
        {
            T.ins(T.root[0], x);
        }else if(op == 2)
        {
            T.del(T.root[0], x);
        }else if(op == 3)
        {
            printf("%d\n", T.getrk(T.root[0], x));
        }else if(op == 4)
        {
            printf("%d\n", T.find(T.root[0], x));
        }else if(op == 5)
        {
            printf("%d\n", T.find(T.root[0], T.getrk(T.root[0], x) - 1));
        }else
        {
            printf("%d\n", T.find(T.root[0], T.getrk(T.root[0], x + 1)));
        }
    }
}

int main()
{
    int T = 1;
    while(T--) solve();
    return 0;
}