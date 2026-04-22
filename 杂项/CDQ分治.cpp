/*
三维偏序
所以CDQ是什么...还不是很理解，反正是个分治，可以同时排序并统计答案
对于第一维，直接排序，第二维用CDQ，第三维用树状数组维护
有相同的花，考虑去重，计数

具体实现：
第一维sort解决
第二维，先分治两个区间，l ~~ mid, mid + 1 ~~ r， 分别处理完两个子区间，考虑合并+计算贡献
因为已经按第一维排好序，左区间的第一维一定小于右区间的第一维 （注意：区间内部第一维不单调） 考虑贡献是不必判断第一维
将两个区间分别按第二维排序，然后像归并排序那样，双指针，如果当前左区间指针的第二维小于等于当前右区间指针的第二维，把左区间指针加入树状数组，指针++
对于每一个右区间指针，先把满足要求的左区间指针加入后，在树状数组里找第三维小于等于它的数的个数d，这个右区间指针的答案+=d
每一个数的答案会在递归中不断累加，最后就把所有三维小于等于它的数都算上了
然后就是给答案记个数，输出
*/
const int N = 1e5 + 10;
int n, k, m;
struct node
{
    int a, b, c, ans, num;
    bool operator < (const node & x) const
    {
        if(a != x.a ) return a < x.a ;
        else if(b != x.b ) return b < x.b ;
        else return c < x.c ;
    }
    bool operator == (const node & x) const 
    {
        if(a == x.a && b == x.b && c == x.c ) return true;
        else return false;
    }
}a[N], b[N]; //a为输入后的原数据，b为我们自己处理去重后的数据

//按照第二维b排序
bool cmp(node a, node b)
{
    if(a.b != b.b ) return a.b < b.b ;
    else return a.c < b.c ;
}

struct BIT
{
    int b[N << 1], len;
    void clear()
    {
        for (re int i = 1; i <= len; ++i) b[i] = 0;
        len = 0;
    }
    void add(int x, int v)
    {
        while (x <= len)
        {
            b[x] += v;
            x += x & -x;
        }
    }
    int query(int x)
    {
        int ans = 0;
        while (x)
        {
            ans += b[x];
            x ^= x & -x;
        }
        return ans;
    }
}tree; //树状数组

void CDQ(int l, int r)
{
    if(l == r) return ; //递归边界
    int mid = (l + r) >> 1;
    CDQ(l, mid), CDQ(mid + 1, r); //先二分处理左右区间
    sort(b + l, b + mid + 1, cmp);
    sort(b + mid + 1, b + r + 1, cmp); //左右区间分别按照第二维b排序
    int s1 = l, s2 = mid + 1; //双指针
    while (s2 <= r) //计算答案贡献
    {
        while (s1 <= mid && b[s1].b <= b[s2].b ) //要比较第二维！ 不能写 b[s1] < b[s2] 因为第一维直接就判断成立了 
        {
            tree.add(b[s1].c , b[s1].num ); 
            ++s1;
        }
        b[s2].ans += tree.query(b[s2].c ); //所有第一维和第二维小于等于当前s2的都在树状数组里，找第三维小于等于它的数的个数，加上
        ++s2;
    }
    for (re int i = l; i < s1; ++i)
        tree.add(b[i].c , -b[i].num ); //这一次分治结束，树状数组要清空，下一次用，注意边界处理，不要减多了
}

int ans[N];

int main()
{
    n = read(), k = read();
    tree.len = k; //记得初始化树状数组的长度 虽然大家都喜欢用n做长度...
    for (re int i = 1; i <= n; ++i)
        a[i].a = read(), a[i].b = read(), a[i].c = read();
    sort(a + 1, a + n + 1); //按照第一维a排序
    //先排序，再去重
    int cnt = 0;
    for (re int i = 1; i <= n; ++i)
    {
        ++cnt;
        if(a[i] == a[i + 1]) continue;
        b[++m] = a[i], b[m].num = cnt;
        cnt = 0;//去重+计数
    }
    CDQ(1, m);
    for (re int i = 1; i <= m; ++i)
        ans[b[i].ans + b[i].num - 1] += b[i].num ; //所有小于它的和完全等于它的值，还要减去自己
    for (re int i = 0; i < n; ++i)
        printf ("%d\n", ans[i]);
    return 0;
}