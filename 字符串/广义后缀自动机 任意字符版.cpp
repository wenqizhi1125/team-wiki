// unordered_map 版，适用任意字符，优点空间小，缺点速度慢
// 后缀自动机还要开二倍空间
const int N = 2e5 + 5;

struct Trie
{
    int tot, w[N], f[N];
    unordered_map<int, int> ch[N];
    // w[i] 为指向 i 节点的边上的字符
    // f[i] 为指向 i 节点的前任节点

    Trie()
    {
        tot = 1; // root = 1;
        memset(w, 0, sizeof(w));
        memset(f, 0, sizeof(f));
    }

    void insert(string &s)
    {
        int now = 1;
        for(auto c : s)
        {
            if(ch[now].find(c - 'a') == ch[now].end()) 
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
    int tot, f[N], len[N];
    unordered_map<int, int> ch[N];
    int id[N]; 
    // id[i]表示trie树上节点i在sam上的对应节点编号
    // 用于辅助构建广义后缀自动机

    // f[i]构成后缀树，可用于显示建树
    // 字符串的前缀节点构成endpos集合，可用线段树合并显示维护endpos集合
    SAM()
    {
        tot = 1;
        memset(f, 0, sizeof(f));
        memset(len, 0, sizeof(len));
        memset(id, 0, sizeof(id));
    }
    
    int add(int c, int last)
    {
        int p = last, now = ++tot;
        len[now] = len[p] + 1;
        for(; p && ch[p].find(c) == ch[p].end(); p = f[p]) ch[p][c] = now;
        if(!p){ f[now] = 1; return now; }
        int q = ch[p][c];
        if(len[q] == len[p] + 1){ f[now] = q; return now; }
        int clone = ++tot;
        len[clone] = len[p] + 1;
        ch[clone] = ch[q];
        f[clone] = f[q];

        for(; p && ch[p].find(c) != ch[p].end() && ch[p][c] == q; p = f[p]) ch[p][c] = clone;
        f[now] = f[q] = clone;
        return now;
    }

    void build()
    {
        queue<int> q;
        for(auto [c, id] : trie.ch[1]) q.push(id);
        id[1] = 1;
        while(!q.empty())
        {
            int now = q.front();
            q.pop();
            id[now] = add(trie.w[now], id[trie.f[now]]);
            for(auto [c, id] : trie.ch[now]) q.push(id);
        }
    }

}sam;