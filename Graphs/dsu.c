#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int*data;
    int size;
    int capacity;
} DynamicArray;

void initarray(DynamicArray*arr,int capacity){
    arr->data=(int*)malloc(capacity*sizeof(int));
    arr->size=0;
    arr->capacity=capacity;
}

void append(DynamicArray*arr,int value){
    if(arr->size==arr->capacity){
        arr->capacity*=2;
        arr->data=(int*)realloc(arr->data,arr->capacity*sizeof(int));
    }
    arr->data[arr->size++]=value;
}

void freearray(DynamicArray*arr){
    free(arr->data);
    arr->size=0;
    arr->capacity=0;
}

int find(int*parent,int x){
    if(parent[x]!=x){
        parent[x]=find(parent,parent[x]);
    }
    return parent[x];
}

void union_sets(int*parent,int*rank,int a,int b){
    int x = find(parent,a);
    int y = find(parent,b);
    if(x==y)return;
    if (rank[x]<rank[y]) {
      parent[x] = y;
      rank[y]+=rank[x];
    } else {
      parent[y] = x;
      rank[x]+=rank[y];
    }
}


int main(){
    int n =5;
    int m = 3;
    int edges[3][2] = {{0,1},{1,2},{3,4}};
    int*parent = (int*)malloc(n*sizeof(int));
    int*rank = (int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++){
        parent[i]=i;
        rank[i]=1;
    }
    for(int i=0;i<m;i++){
        union_sets(parent,rank,edges[i][0],edges[i][1]);
    }
    // PRINT COMPONENTS
    DynamicArray* comps = (DynamicArray*)malloc(n * sizeof(DynamicArray));
    for (int i = 0; i < n; i++)
        initarray(&comps[i], 1);
    for (int i = 0; i < n; i++) {
        int root = find(parent, i);
        append(&comps[root], i);
    }
    printf("Connected Components:\n[\n");
    for (int i = 0; i < n; i++) {
        if (comps[i].size > 0) {
            printf("  [");
            for (int j = 0; j < comps[i].size; j++) {
                printf("%d", comps[i].data[j]);
                if (j < comps[i].size - 1) printf(", ");
            }
            printf("]\n");
        }
        freearray(&comps[i]);
    }
    printf("]\n");
    
    free(comps);
    free(parent);
    free(rank);
    
    return 0;
}
