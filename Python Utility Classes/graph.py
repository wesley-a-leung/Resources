class Graph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = []
        for v in range(V):
            self.__adjList.append([])

    def getV(self):
        return self.__V

    def getE(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v].append(w)
        self.__adjList[w].append(v)
        self.__E += 1

    def adj(self, v):
        return self.__adjList[v]

    def degree(self, v):
        return len(self.__adjList[v])


class DictGraph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = {}
        self.__indegreeArr = {}

    def getV(self):
        return self.__V

    def getE(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v] = self.__adjList.get(v, []) + [w]
        self.__adjList[w] = self.__adjList.get(w, []) + [v]
        self.__E += 1

    def adj(self, v):
        return self.__adjList.get(v, [])

    def degree(self, v):
        return len(self.__adjList.get(v, []))


class Digraph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = []
        for v in range(V):
            self.__adjList.append([])
        self.__indegreeArr = [0] * V

    def getV(self):
        return self.__V

    def getE(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v].append(w)
        self.__indegreeArr[w] += 1
        self.__E += 1

    def adj(self, v):
        return self.__adjList[v]

    def outdegree(self, v):
        return len(self.__adjList[v])

    def indegree(self, v):
        return self.__indegree[v]


class DictDigraph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = {}
        self.__indegreeArr = {}

    def getV(self):
        return self.__V

    def getE(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v] = self.__adjList.get(v, []) + [w]
        self.__indegreeArr[w] = self.__indegreeArr.get(w, 0) + 1
        self.__E += 1

    def adj(self, v):
        return self.__adjList.get(v, [])

    def outdegree(self, v):
        return len(self.__adjList.get(v, []))

    def indegree(self, v):
        return self.__indegreeArr.get(v, 0)
