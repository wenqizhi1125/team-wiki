const int N = 2e6 + 5;
int len1, len2;
char s1[N], s2[N];

int Next[N];

void KMP(char *a)
{
    for(int i = 2, j = 0; i <= len2; ++i)
    {
        while(j && a[j + 1] != a[i]) j = Next[j];
        if(a[i] == a[j + 1]) ++j;
        Next[i] = j;
    }
}

int f[N];
bool vis[N];
// a在b中出现的次数（求a的next数组）
// a为模板串，b为匹配串
void calc(char *a, char *b)
{
    for(int i = 1, j = 0; i <= len1; ++i)
    {
        while(j && (j == len2 || a[j + 1] != b[i])) j = Next[j];
        if(a[j + 1] == b[i]) ++j;
        if(j == len2) vis[i - len2 + 1] = true;
    }
}

int main()
{   
    scanf(" %s %s", &*s1 + 1, &*s2 + 1);
    len1 = strlen(s1 + 1), len2 = strlen(s2 + 1);
    KMP(s2);
    calc(s2, s1);
    for(int i = 1; i <= len1; ++i) if(vis[i]) printf("%d\n", i);
    for(int i = 1; i <= len2; ++i) printf("%d ", Next[i]);
    return 0;
}