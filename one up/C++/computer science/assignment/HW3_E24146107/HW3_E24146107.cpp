#include<stdio.h>

int main()
{
    //user input = n
    int n;
    scanf("%d", &n);

    //find f[n] and output f[0] to f[n]
    long long f[n+1];
    f[0] = 0;
    f[1] = 1;
    f[2] = 1;
    
    printf("%lld ", f[0]);
    printf("%lld ", f[1]);
    printf("%lld ", f[2]);
    
    for(int i=3; i<=n; i++)
    {
        f[i] = f[i-1] + f[i-2];
        printf("%lld ", f[i]);
    }
    return 0;

}