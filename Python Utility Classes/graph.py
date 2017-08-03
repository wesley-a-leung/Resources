class Graph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = []
        for v in range(V):
            self.__adjList.append([])

    def V(self):
        return self.__V

    def E(self):
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
    def __init__(self):
        self.__V = 0
        self.__E = 0
        self.__adjList = {}
        self.__indegreeArr = {}
        self.__verticies = set()

    def V(self):
        return self.__V

    def E(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v] = self.__adjList.get(v, []) + [w]
        self.__adjList[w] = self.__adjList.get(w, []) + [v]
        self.__verticies.add(v)
        self.__verticies.add(w)
        self.__V = len(self.__verticies)
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

    def V(self):
        return self.__V

    def E(self):
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
    def __init__(self):
        self.__V = 0
        self.__E = 0
        self.__adjList = {}
        self.__indegreeArr = {}
        self.__verticies = set()

    def V(self):
        return self.__V

    def E(self):
        return self.__E

    def addEdge(self, v, w):
        self.__adjList[v] = self.__adjList.get(v, []) + [w]
        self.__indegreeArr[w] = self.__indegreeArr.get(w, 0) + 1
        self.__verticies.add(v)
        self.__verticies.add(w)
        self.__V = len(self.__verticies)
        self.__E += 1

    def adj(self, v):
        return self.__adjList.get(v, [])

    def verticies(self):
        return self.__verticies

    def outdegree(self, v):
        return len(self.__adjList.get(v, []))

    def indegree(self, v):
        return self.__indegreeArr.get(v, 0)


class WeightedEdge:
    def __init__(self, v, w, weight):
        self.__v = v
        self.__w = w
        self.__weight = weight

    def getWeight(self):
        return self.__weight

    def either(self):
        return self.__v

    def other(self, vertex):
        if vertex == self.__v:
            return self.__w
        elif vertex == self.__w:
            return self.__v
        else:
            raise ValueError('Illegal endpoint')

    def __eq__(self, e):
        return ((self.__v == e.v and self.__w == e.w) or (self.__v == e.w and self.__w == e.v)) and self.__weight == e.weight

    def __ne__(self, e):
        return not (((self.__v == e.v and self.__w == e.w) or (self.__v == e.w and self.__w == e.v)) and self.__weight == e.weight)

    def _lt__(self, e):
        return self.weight < e.weight

    def _le__(self, e):
        return self.weight <= e.weight

    def _gt__(self, e):
        return self.weight > e.weight

    def _ge__(self, e):
        return self.weight >= e.weight


class WeightedGraph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = []
        for v in range(V):
            self.__adjList.append([])

    def V(self):
        return self.__V

    def E(self):
        return self.__E

    def addEdge(self, e):
        v = e.either()
        w = e.other(v)
        self.__adjList[v].append(e)
        self.__adjList[w].append(e)
        self.__E += 1

    def adj(self, v):
        return self.__adjList[v]

    def degree(self, v):
        return len(self.__adjList[v])

    def edges(self):
        allEdges = []
        for v in range(self.__V):
            selfLoops = 0
            for e in self.adj(v):
                if e.other(v) > v:
                    allEdges.append(e)
                elif e.other(v) == v:
                    if selfLoops % 2 == 0:
                        allEdges.append(e)
                    selfLoops += 1
        return allEdges


class DirectedWeightedEdge:
    def __init__(self, v, w, weight):
        self.__v = v
        self.__w = w
        self.__weight = weight

    def getWeight(self):
        return self.__weight

    def start(self):
        return self.__v

    def end(self, vertex):
        return self.__w

    def __eq__(self, e):
        return self.__v == e.v and self.__w == e.w and self.__weight == e.weight

    def __ne__(self, e):
        return not self.__v == e.v and self.__w == e.w and self.__weight == e.weight

    def _lt__(self, e):
        return self.weight < e.weight

    def _le__(self, e):
        return self.weight <= e.weight

    def _gt__(self, e):
        return self.weight > e.weight

    def _ge__(self, e):
        return self.weight >= e.weight


class WeightedDigraph:
    def __init__(self, V):
        self.__V = V
        self.__E = 0
        self.__adjList = []
        for v in range(V):
            self.__adjList.append([])
        self.__indegreeArr = [0] * V

    def V(self):
        return self.__V

    def E(self):
        return self.__E

    def addEdge(self, e):
        v = e.start()
        w = e.end()
        self.__adjList[v].append(e)
        self.__indegreeArr[w] += 1
        self.__E += 1

    def adj(self, v):
        return self.__adjList[v]

    def outdegree(self, v):
        return len(self.__adjList[v])

    def indegree(self, v):
        return self.__indegree[v]

    def edges(self):
        allEdges = []
        for v in range(self.__V):
            for e in self.adj(v):
                allEdges.append(e)
        return allEdges
