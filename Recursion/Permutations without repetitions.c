#include<stdio.h>
#include<stdlib.h>
void swap(int *x,int *y){
    int temp=*x;
    *x=*y;
    *y=temp;
}
void do_permutaions(int *a,size_t n,int *b,size_t k,size_t k1){
    
    if(k1==k){
        for(size_t i=0;i<k;i++){
            printf("%d ",b[i]);
        }
        printf("\n");
        return;
    }
    for(size_t i =0;i<n;i++){
        swap(&a[0],&a[i]);
        b[k1] = a[0];
        do_permutaions(a+1,n-1,b,k,k1+1);
        swap(&a[0],&a[i]); // backtrack
    }
    
}
    
void permutations(int *a,size_t n,int *b,size_t k){
    if(n<k){
        return;
    }
    do_permutaions(a,n,b,k,0);
}

int main() {
    int a[] = {1, 2, 3}; // Input array
    size_t n = sizeof(a) / sizeof(a[0]); // Length of input array
    size_t k = 3; // Length of permutations to generate
    int b[k]; // Output array to store permutations

    printf("All permutations of length %zu from the array [1, 2, 3]:\n", k);
    permutations(a, n, b, k);

    return 0;
}
