struct DFA
{
    int n, s, st;
    vector< vector<int> > go;
    vector<int> out;
    DFA(int n = 0, int s = 0, int st = 0):n(n), s(s), st(st), go(n, vector<int>(s, -1)), out(n, 0){}
};

struct MinRes
{
    DFA d;
    vector<int> bel;
    vector< vector<int> > grp;
};

MinRes minimize_dfa(DFA a)
{
    int N = a.n, S = a.s;

    vector<int> vis(N, 0), ord, mp(N, -1);
    queue<int> q;
    vis[a.st] = 1, q.push(a.st);
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        mp[u] = siz(ord);
        ord.emplace_back(u);
        for(int c = 0; c < S; ++c)
        {
            int v = a.go[u][c];
            if(v != -1 && !vis[v]) vis[v] = 1, q.push(v);
        }
    }

    DFA b(siz(ord), S, mp[a.st]);
    for(int i = 0; i < b.n; ++i)
    {
        int u = ord[i];
        b.out[i] = a.out[u];
        for(int c = 0; c < S; ++c)
        {
            int v = a.go[u][c];
            b.go[i][c] = (v == -1 ? -1 : mp[v]);
        }
    }
    a = b;

    bool need = 0;
    for(int i = 0; i < a.n; ++i)
        for(int c = 0; c < S; ++c)
            if(a.go[i][c] == -1) need = 1;

    if(need)
    {
        set<int> ss;
        for(int x : a.out) ss.insert(x);
        int tag = inf;
        while(ss.count(tag)) --tag;

        int sink = a.n++;
        a.go.push_back(vector<int>(S, sink));
        a.out.push_back(tag);

        for(int i = 0; i < sink; ++i)
            for(int c = 0; c < S; ++c)
                if(a.go[i][c] == -1) a.go[i][c] = sink;
    }

    int n = a.n;
    vector< vector<char> > bad(n, vector<char>(n, 0));

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < i; ++j)
            if(a.out[i] != a.out[j]) bad[i][j] = 1;

    bool flag = 1;
    while(flag)
    {
        flag = 0;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < i; ++j)
            {
                if(bad[i][j]) continue;
                for(int c = 0; c < S; ++c)
                {
                    int x = a.go[i][c], y = a.go[j][c];
                    if(x == y) continue;
                    if(x < y) swap(x, y);
                    if(bad[x][y])
                    {
                        bad[i][j] = 1;
                        flag = 1;
                        break;
                    }
                }
            }
    }

    vector<int> f(n);
    iota(f.begin(), f.end(), 0);

    function<int(int)> find = [&](int x)
    {
        return x == f[x] ? x : (f[x] = find(f[x]));
    };

    auto merge = [&](int x, int y)
    {
        x = find(x), y = find(y);
        if(x != y) f[x] = y;
    };

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < i; ++j)
            if(!bad[i][j]) merge(i, j);

    vector<int> id(n, -1);
    int m = 0;
    for(int i = 0; i < n; ++i)
    {
        int r = find(i);
        if(id[r] == -1) id[r] = m++;
    }

    DFA res(m, S, id[find(a.st)]);
    vector< vector<int> > grp(m);
    for(int old = 0; old < N; ++old)
    {
        if(mp[old] == -1) continue;
        int k = id[find(mp[old])];
        grp[k].emplace_back(old);
    }

    for(int i = 0; i < n; ++i)
    {
        int x = id[find(i)];
        res.out[x] = a.out[i];
        for(int c = 0; c < S; ++c)
            res.go[x][c] = id[find(a.go[i][c])];
    }

    vector<int> bel(N, -1);
    for(int old = 0; old < N; ++old)
    {
        if(mp[old] == -1) continue;
        bel[old] = id[find(mp[old])];
    }

    return (MinRes){res, bel, grp};
}