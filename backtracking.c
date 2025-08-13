#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

#include<math.h>
#ifdef _WIN32
typedef long ssize_t;
#endif


struct node{
    size_t n;
    size_t *queens;
    size_t k;//current row
    ssize_t last;
};

void root(size_t n,struct node*p){
    p->n=n;
    p->queens=malloc(n*sizeof(size_t));
    assert(p->queens);
    p->k=0;
    p->last=-1;
}

bool leaf(struct node*p){
    if(p->k==p->n){return true;}
    return false;
}

bool ok(struct node *p) {
    for (size_t i = 0; i < p->k; i++) {
        if (p->queens[i] == p->last) return false; // same column
        if (p->queens[i] + (p->k - i) == (size_t)p->last) return false; // ↘ diagonal
        if (p->queens[i] - (p->k - i) == (size_t)p->last) return false; // ↗ diagonal
    }
    return true;
}

bool up(struct node*p){
    if(p->k==0){return false;}
    p->k--;
    p->last = p->queens[p->k];
    return true;
}

bool down(struct node*p){
   size_t start_col = (p->last == -1 ? 0 : (size_t)p->last + 1);
    for(size_t i = start_col; i < p->n; i++){
        p->last = i;
        if(ok(p)){ // check immediately
            p->queens[p->k] = i;
            p->k++;
            p->last = -1;
            return true;
        }
    }
    return false;
}

size_t nqueens(size_t n){
    struct node p;
    root(n,&p);
    size_t c = 0;
    while(true){
        if(leaf(&p)){
            if(ok(&p)){c++;} 
            if(!up(&p)){
            break;
        }     }
        else{
            if(!ok(&p)){
                if(!up(&p)){
                    break;
                }}
            else{
                if(!down(&p)){
                    if(!up(&p)){
                        break;}     }
            }  }
    }
      free(p.queens);
    return c;
}
int main(){
    size_t n;
    printf("Enter the value of n(number of queens): ");
    scanf("%zu", &n);
    size_t result = nqueens(n);
    printf("Number of solutions for %zu queens: %zu\n", n, result);
    return 0;
}