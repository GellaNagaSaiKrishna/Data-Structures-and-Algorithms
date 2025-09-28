#include <stdio.h>
#include <stdlib.h>

#define MAXV 100005 
#define MAXE 200005 

int head[MAXV]; 
int to[MAXE];   
int next[MAXE]; 
int visited[MAXV];
int topo[MAXV];
int n, m, edge_cnt = 0, topo_idx = 0;

void add_edge(int u, int v) {
    to[edge_cnt] = v;
    next[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

void dfs(int u) {
    visited[u] = 1;
    for(int i = head[u]; i != -1; i = next[i]) {
        int v = to[i];
        if(!visited[v]) dfs(v);
    }

    topo[topo_idx++] = u;
}

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 1;

    for(int i = 0; i < n; i++) head[i] = -1;

    for(int i = 0; i < m; i++) {
        int u_in, v_in;
        if (scanf("%d %d", &u_in, &v_in) != 2) break;
        add_edge(u_in - 1, v_in - 1); 
    }
    for(int i = 0; i < n; i++)
        if(!visited[i]) dfs(i);
    for(int i = topo_idx - 1; i >= 0; i--) {
        printf("%d ", topo[i] + 1); 
        
    }
    
    return 0;
}
