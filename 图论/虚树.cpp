// 虚树大概就是说，从原来的树中抽出一些对答案有贡献的点（边）建成一颗小规模的保证存在答案的树
// 虚树建立时，按照dfs序建立，即先建立左子树，再建立右子树，我们始终维护最右链
// 最开始时，将第一个点直接加入栈，接下来依次加入其他点
// 考虑新加入的点，计算一下新加入的点和栈顶元素的LCA（栈顶元素一定是最靠左的）
// 新加入的点有这么几种情况
// 1.它在栈顶元素的右子树里，这时候LCA就是栈顶元素，直接把新加入的点加入栈即可
// 2.LCA在栈顶元素和栈第二个元素中间，这个时候也需要把LCA放到栈里，
// 具体实现是先从LCA向栈顶元素建一条边，然后将栈顶元素弹出（因为按照DFS序加入，所以后面加入的点一定不会再接在栈顶元素上了）
// 然后将LCA加入栈，新节点加入栈
// 3.LCA就是栈第二个元素，直接从栈第二个元素向栈顶元素加一条边，然后将栈顶元素弹出（原因同上）然后新节点加入栈
// 4.LCA可能是很远很远，比如在栈第4个元素（或者栈3栈4中间），我们先在当前栈顶栈2建边，然后弹出栈顶，不断重复，最后回来123情况
// 这就是建树了，然后怎么查询，在虚树上DFS即可，弄一个DP，然后注意虚树上有的点是对答案有贡献的，有的只是中间祖先（保证虚树完整的）
// 所以转移的时候分两种转移，具体情况具体分析
const ll N = 5e5 + 10;
ll cnt, head[N];
struct edge{ ll v, w, next ; } xing[N << 2];
void add(ll u, ll v, ll w)
{
    ++cnt;
    xing[cnt].v = v;
    xing[cnt].w = w;
    xing[cnt].next = head[u];
    head[u] = cnt;
}

ll n;
ll dfn[N], tot, f[N][23], depth[N], minn[N];
void dfs(ll k, ll fa)
{
    dfn[k] = ++tot, f[k][0] = fa, depth[k] = depth[fa] + 1;
    for (re ll i = 1; f[k][i - 1]; ++i)
        f[k][i] = f[f[k][i - 1]][i - 1];
    for (re ll i = head[k]; i ; i = xing[i].next )
    {
        ll v = xing[i].v , w = xing[i].w ;
        if(v == fa) continue;
        minn[v] = min(minn[k], w); // 预处理DP
        dfs(v, k);
    }
}

ll lca(ll x, ll y)
{
    if(depth[x] < depth[y]) swap(x, y);
    if(depth[x] != depth[y])
    {
        for (re ll i = 21; i >= 0; --i)
        {
            if(depth[f[x][i]] >= depth[y])
                x = f[x][i];
        }
    }
    if(x == y) return x;
    for (re ll i = 21; i >= 0; --i)
    {
        if(f[x][i] != f[y][i])
            x = f[x][i], y = f[y][i];
    }
    return f[x][0];
}

ll xx[N], top, vis[N];
ll que[N], r;

bool cmp(ll a, ll b)
{
    return dfn[a] < dfn[b];
}

ll zong, tou[N];
struct e{ ll v, next; } bian[N];
void newadd(ll u, ll v)
{
    ++zong;
    bian[zong].v = v;
    bian[zong].next = tou[u];
    tou[u] = zong;
}

ll dfs2(ll k, ll fa)
{
    ll sum = 0, ans;
    for (re ll i = tou[k]; i; i = bian[i].next )
    {
        ll v = bian[i].v ;
        sum += dfs2(v, k);
    }
    if(vis[k]) ans = minn[k], vis[k] = 0; // 如果当前节点是关键点的话，这个点必须切掉，所以整个子树的贡献就它这条边
    else ans = min(sum, minn[k]); // 否则，可以切这条边，也可以切子树的边，取最小值
    tou[k] = 0;
    return ans;
}

int main()
{
    n = read();
    for (re ll i = 1; i < n; ++i)
    {
        ll s1 = read(), s2 = read(), s3 = read();
        add(s1, s2, s3), add(s2, s1, s3);
    }
    minn[1] = 0x7ffffffffffffff; //  因为1是根节点，所以1不能切，后面的由自己的边更新
    dfs(1, 0);
    ll T = read();
    while (T--)
    {
        ll m = read();
        r = 0;
        for (re ll i = 1; i <= m; ++i)
            que[++r] = read(), vis[que[r]] = 1;
        sort(que + 1, que + r + 1, cmp);
        top = 0;
        xx[++top] = que[1];
        zong = 0;
        for (re ll i = 2; i <= r; ++i)
        {
            ll d = lca(xx[top], que[i]);
            while (1)
            {
                if(depth[d] >= depth[xx[top - 1]])
                {
                    if(d == xx[top]) xx[++top] = que[i];
                    else if(d == xx[top - 1])
                    {
                        newadd(xx[top - 1], xx[top]);
                        xx[top] = que[i]; // 先把原栈顶元素出栈，再加入当前节点
                    }else 
                    {
                        newadd(d, xx[top]);
                        xx[top] = d; // 原栈顶元素出栈，加入lca 
                        xx[++top] = que[i];
                    }
                    break;
                }else
                {
                    newadd(xx[top - 1], xx[top]);
                    top--;
                }
            }
        }
        while (top != 1)
        {
            newadd(xx[top - 1], xx[top]);
            --top;
        }
        printf ("%lld\n", dfs2(xx[top], 0));
    }
    return 0;
}