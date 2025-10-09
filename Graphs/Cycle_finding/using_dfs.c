#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXV 1005 

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

Node* adj[MAXV];
bool visited[MAXV];
int parent[MAXV];      
int V;                
int E;                
void addEdge(int u, int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = adj[u];
    adj[u] = newNode;
}

void print_cycle(int u_cycle, int v_cycle) {
    int cycle_path[MAXV];
    int path_len = 0;
    int current = u_cycle;
    while (current != v_cycle) {
        cycle_path[path_len++] = current;
        current = parent[current];
    }
    cycle_path[path_len++] = v_cycle; 
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%d ", cycle_path[i] + 1); 
    }
    printf("%d", cycle_path[path_len - 1] + 1); 
    printf("\n");
}

bool dfs(int u) {
    visited[u] = true;
    Node* temp = adj[u];
    while (temp) {
        int v = temp->vertex;
        if (!visited[v]) {
            parent[v] = u;
            if (dfs(v)) {
                return true;
            }
        } else if (v != parent[u]) {
            print_cycle(u, v);
            return true;
        }
        temp = temp->next;
    }
    return false;
}
void freeGraph() {
    for (int i = 0; i < V; i++) {
        Node *current = adj[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
        adj[i] = NULL;
    }
}
int main() {
    int u_in, v_in;
    if (scanf("%d %d", &V, &E) != 2) return 1;
    for(int i = 0; i < V; i++) {
        adj[i] = NULL;
        parent[i] = -1; 
        visited[i] = false;
    }
    for(int i = 0; i < E; i++) {
        if (scanf("%d %d", &u_in, &v_in) != 2) break;
        int u = u_in - 1;
        int v = v_in - 1;
        addEdge(u, v);
        addEdge(v, u); 
    }
    bool cycle_found = false;
    for(int i = 0; i < V; i++) {
        if (!visited[i]) {
            parent[i] = -1;
            if (dfs(i)) {
                cycle_found = true;
                break;
            }
        }
    }
    if (!cycle_found) {
        printf("N\n");
    }
    freeGraph();
    return 0;
}
