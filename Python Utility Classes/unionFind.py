class UF:
    def __init__(self, n):
        self._count = n
        self._parent = [i for i in range(n)]
        self._rank = [0] * n

    def find(self, p):
        while (p != self._parent[p]):
            self._parent[p] = self._parent[self._parent[p]]
            p = self._parent[p]
        return p

    def count(self):
        return self._count

    def connected(self, p, q):
        return self.find(p) == self.find(q)

    def rank(self, p):
        return self._rank[self.find(p)]

    def union(self, p, q):
        rootP = self.find(p)
        rootQ = self.find(q)
        if rootP == rootQ:
            return
        if self._rank[rootP] < self._rank[rootQ]:
            self._parent[rootP] = rootQ
        elif self._rank[rootP] > self._rank[rootQ]:
            self._parent[rootQ] = rootP
        else:
            self._parent[rootQ] = rootP
            self._rank[rootP] += 1
        self._count -= 1


class WeightedUF:
    def __init__(self, n):
        self._count = n
        self._parent = [i for i in range(n)]
        self._size = [1] * n

    def find(self, p):
        while (p != self._parent[p]):
            self._parent[p] = self._parent[self._parent[p]]
            p = self._parent[p]
        return p

    def count(self):
        return self._count

    def connected(self, p, q):
        return self.find(p) == self.find(q)

    def size(self, p):
        return self._size[self.find(p)]

    def union(self, p, q):
        rootP = self.find(p)
        rootQ = self.find(q)
        if rootP == rootQ:
            return
        if self._size[rootP] < self._size[rootQ]:
            self._parent[rootP] = rootQ
            self._size[rootQ] += self._size[rootP]
        else:
            self._parent[rootQ] = rootP
            self._size[rootP] += self._size[rootQ]
        self._count -= 1
