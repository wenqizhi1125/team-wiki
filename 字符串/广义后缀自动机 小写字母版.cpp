// 广义后缀自动机
// 先把所有建到trie树上
// 然后按照BFS序插入，last是当前节点的父亲
// 然后其他的和普通的后缀自动机差不多
// 注意p直接取，因为trie树已经把我们想要的建出来了
// 注意clone的ch不能直接取，可能那个节点trie树上有但是SAM上没有，可以通过len判断那个点SAM有没有，有再复制

// 小写字母版，优点速度快，缺点空间26倍
// 后缀自动机还要开二倍空间
const int N = 2e5 + 5;

struct Trie
{
    int tot, ch[N][26], w[N], f[N];
    // w[i] 为指向 i 节点的边上的字符
    // f[i] 为指向 i 节点的前任节点

    Trie()
    {
        tot = 1; // root = 1;
        memset(ch, 0, sizeof(ch));
        memset(w, 0, sizeof(w));
        memset(f, 0, sizeof(f));
    }

    void insert(string &s)
    {
        int now = 1;
        for(auto c : s)
        {
            if(!ch[now][c - 'a']) 
            {
                ch[now][c - 'a'] = ++tot;
                w[tot] = c - 'a';
                f[tot] = now;
            }
            now = ch[now][c - 'a'];
        }
    }
}trie;

struct SAM
{
    int tot, f[N], len[N], ch[N][26];
    int id[N]; 
    // id[i]表示trie树上节点i在sam上的对应节点编号
    // 用于辅助构建广义后缀自动机

    // f[i]构成后缀树，可用于显示建树
    // 字符串的前缀节点构成endpos集合，可用线段树合并显示维护endpos集合
    SAM()
    {
        tot = 1;
        memset(ch, 0, sizeof(ch));
        memset(f, 0, sizeof(f));
        memset(len, 0, sizeof(len));
        memset(id, 0, sizeof(id));
    }
    
    int add(int c, int last)
    {
        int p = last, now = ++tot;
        len[now] = len[p] + 1;
        for(; p && !ch[p][c]; p = f[p]) ch[p][c] = now;
        if(!p){ f[now] = 1; return now; }
        int q = ch[p][c];
        if(len[q] == len[p] + 1){ f[now] = q; return now; }
        int clone = ++tot;
        len[clone] = len[p] + 1;
        for(int i = 0; i < 26; ++i) ch[clone][i] = ch[q][i];
        f[clone] = f[q];

        for(; p && ch[p][c] == q; p = f[p]) ch[p][c] = clone;
        f[now] = f[q] = clone;
        return now;
    }

    void build()
    {
        queue<int> q;
        for(int i = 0; i < 26; ++i) if(trie.ch[1][i]) q.push(trie.ch[1][i]);
        id[1] = 1;
        while(!q.empty())
        {
            int now = q.front();
            q.pop();
            id[now] = add(trie.w[now], id[trie.f[now]]);
            for(int i = 0; i < 26; ++i) if(trie.ch[now][i]) q.push(trie.ch[now][i]);
        }
    }

}sam;