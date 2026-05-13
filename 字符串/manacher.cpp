struct Manacher
{
    int n;
    string s;

    // odd[i]：以 i 为中心的最长奇回文半径
    // 区间为 [i - odd[i] + 1, i + odd[i] - 1]
    // 长度为 2 * odd[i] - 1
    vector<int> odd;

    // even[i]：以 i - 1 和 i 中间为中心的最长偶回文半径
    // 区间为 [i - even[i], i + even[i] - 1]
    // 长度为 2 * even[i]
    vector<int> even;

    int ansLen, ansL, ansR;

    Manacher() : n(0), ansLen(0), ansL(0), ansR(-1) {}

    Manacher(const string &t){ solve(t); }

    void solve(const string &t)
    {
        s = t; n = (int)s.size();
        odd.assign(n, 0), even.assign(n, 0);
        ansLen = 0, ansL = 0, ansR = -1;

        // 奇回文
        for(int i = 0, l = 0, r = -1; i < n; ++i)
        {
            int k = (i > r) ? 1 : min(odd[l + r - i], r - i + 1);
            while(i - k >= 0 && i + k < n && s[i - k] == s[i + k]) ++k;
            odd[i] = k;
            int L = i - k + 1, R = i + k - 1, len = R - L + 1;
            if(len > ansLen) ansLen = len, ansL = L, ansR = R;
            if(R > r) l = L, r = R;
        }

        // 偶回文
        for(int i = 0, l = 0, r = -1; i < n; ++i)
        {
            int k = (i > r) ? 0 : min(even[l + r - i + 1], r - i + 1);
            while(i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) ++k;
            even[i] = k;
            int L = i - k, R = i + k - 1, len = R - L + 1;
            if(len > ansLen) ansLen = len, ansL = L, ansR = R;
            if(R > r) l = L, r = R;
        }
    }

    // 以 i 为中心的最长奇回文长度
    int odd_len(int i){ return 2 * odd[i] - 1; }

    // 以 i - 1 和 i 中间为中心的最长偶回文长度
    int even_len(int i){ return 2 * even[i]; }

    // 以 i 为中心的最长奇回文区间
    pair<int, int> get_odd(int i){ return {i - odd[i] + 1, i + odd[i] - 1}; }

    // 以 i - 1 和 i 中间为中心的最长偶回文区间
    // 若 even[i] == 0，返回空区间 [i, i - 1]
    pair<int, int> get_even(int i){ return {i - even[i], i + even[i] - 1}; }

    // 全串最长回文子串区间
    pair<int, int> get_ans(){ return {ansL, ansR}; }

    // 判断 s[l...r] 是否为回文，默认下标合法
    bool is_pal(int l, int r)
    {
        if(l > r) return true;
        int len = r - l + 1;
        if(len & 1)
        {
            int mid = (l + r) >> 1;
            return odd[mid] >= (len + 1) / 2;
        }
        else
        {
            int mid = (l + r + 1) >> 1;
            return even[mid] >= len / 2;
        }
    }

    // 所有奇回文子串数量
    long long count_odd()
    {
        long long ans = 0;
        for(int x : odd) ans += x;
        return ans;
    }

    // 所有偶回文子串数量
    long long count_even()
    {
        long long ans = 0;
        for(int x : even) ans += x;
        return ans;
    }

    // 所有回文子串数量
    long long count_all(){ return count_odd() + count_even(); }

    // 最长回文前缀区间
    pair<int, int> longest_prefix()
    {
        int best = 0;
        for(int i = 0; i < n; ++i)
        {
            if(odd[i] >= i + 1) best = max(best, 2 * i + 1);
            if(even[i] >= i) best = max(best, 2 * i);
        }
        return {0, best - 1};
    }

    // 最长回文后缀区间
    pair<int, int> longest_suffix()
    {
        int best = 0;
        for(int i = 0; i < n; ++i)
        {
            if(odd[i] >= n - i) best = max(best, 2 * (n - i) - 1);
            if(even[i] >= n - i) best = max(best, 2 * (n - i));
        }
        return {n - best, n - 1};
    }
};
