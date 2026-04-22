namespace O1_gcd
{
    const int M = 1e6 + 5, T = 1e3 + 5;
    
    int pre[T][T];
    int fac[M][3], prime[M], tot;
    bool vis[M];
    
    /*
        使用方法：
        M 为值域+5，T为根号值域+5
        预处理调用init(M - 5);
        复杂度：O(值域)预处理 O(1)gcd
    */

    void init(int AX = M - 5)
    {
        fac[1][0] = fac[1][1] = fac[1][2] = 1;
        for(int i = 2; i <= AX; ++i)
        {
            if(!vis[i])
            {
                fac[i][0] = fac[i][1] = 1;
                fac[i][2] = i;
                prime[++tot] = i;
            }
            for(int j = 1; prime[j] * i <= AX; ++j)
            {
                int tmp = prime[j] * i;
                vis[tmp] = true;
                fac[tmp][0] = fac[i][0] * prime[j];
                fac[tmp][1] = fac[i][1];
                fac[tmp][2] = fac[i][2];
                if(fac[tmp][0] > fac[tmp][1])
                {
                    fac[tmp][0] ^= fac[tmp][1] ^= fac[tmp][0] ^= fac[tmp][1];
                }
                if(fac[tmp][1] > fac[tmp][2])
                {
                    fac[tmp][1] ^= fac[tmp][2] ^= fac[tmp][1] ^= fac[tmp][2];
                }
                if(i % prime[j] == 0)
                {
                    break;
                }
            }
        }
        for(int i = 0; i <= T - 5; ++i)
        {
            pre[0][i] = pre[i][0] = i;
        }
        for(int i = 1; i <= T - 5; ++i)
        {
            for(int j = 1; j <= i; ++j)
            {
                pre[i][j] = pre[j][i] = pre[j][i % j];
            }
        }
    }

    int gcd(int a, int b)
    {
        int ans = 1;
        for(int i = 0; i < 3; ++i)
        {
            int tmp = (fac[a][i] > T) ? (b % fac[a][i] ? 1 : fac[a][i]) : pre[fac[a][i]][b % fac[a][i]];
            b /= tmp;
            ans *= tmp;
        }
        return ans;
    }
}
using namespace O1_gcd;