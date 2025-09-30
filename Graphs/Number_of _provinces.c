void dfs(int node, int isConnectedSize, int** isConnected, int* visited) {
    visited[node] = 1;
    for (int neighbor = 0; neighbor < isConnectedSize; neighbor++) {
        if (isConnected[node][neighbor] == 1 && !visited[neighbor]) {
            dfs(neighbor, isConnectedSize, isConnected, visited);
        }
    }
}

int findCircleNum(int** isConnected, int isConnectedSize, int* isConnectedColSize) {
    int *visited = calloc(isConnectedSize,sizeof(int));
    int count=0;
    for(int i=0;i<isConnectedSize;i++){
        if(!visited[i]){
            dfs(i,isConnectedSize,isConnected,visited);
        count++;}
    }
return count;}
