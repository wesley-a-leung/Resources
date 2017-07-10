from collections import deque


class BreadthFirstSearch:
    def __init__(self, G, s):
        self.__marked = [False] * G.getV()
        self.__distTo = [float('inf')] * G.getV()
        self.__edgeTo = [-1] * G.getV()
        self.__bfs(G, s)

    def __bfs(self, G, s):
        q = deque()
        self.__distTo[s] = 0
        self.__marked[s] = True
        q.append(s)
        while len(q) != 0:
            v = q.popleft()
            for w in G.adj(v):
                if not self.__marked[w]:
                    self.__edgeTo[w] = v
                    self.__distTo[w] = self.__distTo[v] + 1
                    self.__marked[w] = True
                    q.append(w)

    def getDistTo(self, v):
        return self.__distTo[v]

    def getEdgeTo(self, v):
        return self.__edgeTo[v]

    def hasPathTo(self, v):
        return self.__marked[v]

    def pathTo(self, v):
        if (not self.__hasPathTo(v)):
            return None
        path = []
        x = v
        while self.__distTo[x] != 0:
            path.append(x)
            x = self.__edgeTo[x]
        return path[::-1]


class DepthFirstSearch:
    def __init__(self, G, s):
        self.__marked = [False] * G.getV()
        self.__depth = [-1] * G.getV()
        self.__edgeTo = [-1] * G.getV()
        self.__dfs(G, s, 0)

    def __dfs(self, G, v, d):
        self.__marked[v] = True
        self.__depth[v] = d
        for w in G.adj(v):
            if not self.__marked[w]:
                self.__edgeTo[w] = v
                self.__dfs(G, w, d + 1)

    def getDepthOf(self, v):
        return self.__depth[v]

    def getEdgeTo(self, v):
        return self.__edgeTo[v]

    def hasPathTo(self, v):
        return self.__marked[v]

    def pathTo(self, v):
        if (not self.__hasPathTo(v)):
            return None
        path = []
        x = v
        while self.__depth[x] != 0:
            path.append(x)
            x = self.__edgeTo[x]
        return path[::-1]
