class Graph:
    __V = 0
    __E = 0
    __adjList = []

    def __init__(self, V):
        self.__V = V
        self.__E = 0
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


class Digraph:
    __V = 0
    __E = 0
    __adjList = []
    __indegreeArr = []

    def __init__(self, V):
        self.__V = V
        self.__E = 0
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
