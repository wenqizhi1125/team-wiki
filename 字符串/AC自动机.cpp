const int N = 2e5 + 5, M = 2e6 + 6;
char s[N], t[M];
int trie[N][26], tot;
int fail[N], val[N], id[N];

int ru[N];

void insert(char *s, int ID)
{
    int now = 0, len = strlen(s + 1);
    for(int i = 1; i <= len; ++i)
    {
        if(!trie[now][s[i] - 'a']) trie[now][s[i] - 'a'] = ++tot;
        now = trie[now][s[i] - 'a'];
    }
    id[ID] = now;
}

void build()
{
    queue<int> q;
    for(int i = 0; i <= 25; ++i) if(trie[0][i]) q.push(trie[0][i]);

    while(!q.empty())
    {
        int now = q.front(); q.pop();
        for(int i = 0; i <= 25; ++i)
        {
            if(trie[now][i])
            {
                fail[trie[now][i]] = trie[fail[now]][i];
                q.push(trie[now][i]);
                ++ru[trie[fail[now]][i]];
            }else trie[now][i] = trie[fail[now]][i];
        }
    }

}

void query(char *s)
{
    int now = 0, len = strlen(s + 1);
    for(int i = 1; i <= len; ++i)
    {
        now = trie[now][s[i] - 'a'];
        ++val[now]; // 这个位置沿着fail树到根的节点都加1
    }
}

int n;

// 拓扑内向fail树统计答案
// 从fail树上从上往下遍历也对
void getans()
{
    queue<int> q;
    for(int i = 1; i <= tot; ++i) if(!ru[i]) q.push(i);

    while(!q.empty())
    {
        int now = q.front(); q.pop();
        int v = fail[now];
        val[v] += val[now];
        --ru[v];
        if(ru[v] == 0) q.push(v);
    }
    for(int i = 1; i <= n; ++i) printf("%d\n", val[id[i]]);
}

int main()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i) scanf("%s", s + 1), insert(s, i);
    build();
    scanf("%s", t + 1);
    query(t);
    getans();
    return 0;
}