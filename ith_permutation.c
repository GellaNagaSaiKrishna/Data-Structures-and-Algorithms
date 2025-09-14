#include <stdio.h>
#include <stdlib.h>
void perm(int n,long long i,int*permutation){
    long long *fact = (long long*)(malloc((n+1)*sizeof(long long)));
    int f=1;
    fact[0]=1;
    i--;
    for(int j=0;j<n;j++){
         f *= (j + 1);
         fact[j + 1] = f;
    }
    int*num = (int*)(malloc(n*sizeof(int)));
    for(int j=0;j<n;j++){
        num[j]=j;
    }
    for(int j = n;j>=1;j--){
        int remain = j-1;
        int index = (int)(i/fact[remain]);
        i %= fact[remain];
        permutation[n-j] = num[index];
        for(int z = index;z<j-1;z++){
            num[z]=num[z+1];
        }
    }
    free(fact);
    free(num);
}
int main(){
    int n = 5;
    long long i = 40;  // 1-based index

    int *permutation = malloc(n * sizeof(int));
    perm(n, i, permutation);

    printf("The %lld-th permutation of [0, %d):\n", i, n);
    for (int j = 0; j < n; j++) {
        printf("%d ", permutation[j]);
    }
    printf("\n");

    free(permutation);
    return 0;
}
