#include <stdio.h>
#include <stdlib.h>
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
void do_combinations(int *a,size_t n,int*b,size_t k,size_t k1){
    if(n<k-k1){
        return;
    }
    if(k1==k){
        for(size_t i=0;i<k;i++){
            printf("%d ",b[i]);
        }
        printf("\n");
        return;
    }
    b[k1]=a[0];
    do_combinations(a+1,n-1,b,k,k1+1);
    do_combinations(a+1,n-1,b,k,k1);
}
void combinations(int *a,size_t n,int *b,size_t k){
    do_combinations(a,n,b,k,0);
}
int main(){
    int a[] = {1, 2, 3, 4};
    size_t n = sizeof(a) / sizeof(a[0]);
    size_t k = 2;
    int *b = (int *)malloc(k* sizeof(int));
     // Choose 2 elements for combinations 
    combinations(a, n, b, k);
    free(b);
    return 0;
}
