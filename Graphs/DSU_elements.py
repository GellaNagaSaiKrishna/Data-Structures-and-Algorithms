from collections import defaultdict
from typing import List

class Solution:
    def connectedComponents(self, n: int, edges: List[List[int]]) -> List[List[int]]:
        par = [i for i in range(n)]
        rank = [1] * n

        def find(x):
            while x != par[x]:
                par[x] = par[par[x]]  # path compression
                x = par[x]
            return x

        def union(x, y):
            p1, p2 = find(x), find(y)
            if p1 == p2:
                return
            if rank[p1] < rank[p2]:
                par[p1] = p2
                rank[p2] += rank[p1]
            else:
                par[p2] = p1
                rank[p1] += rank[p2]

        # Apply unions
        for u, v in edges:
            union(u, v)

        # Group nodes by root parent
        comp = defaultdict(list)
        for i in range(n):
            comp[find(i)].append(i)

        return list(comp.values())
sol = Solution()
print(sol.connectedComponents(5, [[0,1],[1,2],[3,4]]))
