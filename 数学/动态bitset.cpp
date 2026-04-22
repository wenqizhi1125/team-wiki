template<int len = 1> ll get()
{
    if(n >= len) return get<min(2 * len, N - 5)>();
    bitset<len> dp;
}