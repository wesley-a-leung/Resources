#ifndef ALGORITHMS_GRAPH_CYCLE_BIPARTITEX_H_
#define ALGORITHMS_GRAPH_CYCLE_BIPARTITEX_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 *  The {@code BipartiteX} class represents a data type for
 *  determining whether an undirected graph is bipartite or whether
 *  it has an odd-length cycle.
 *  The <em>isBipartite</em> operation determines whether the graph is
 *  bipartite. If so, the <em>color</em> operation determines a
 *  bipartition; if not, the <em>oddCycle</em> operation determines a
 *  cycle with an odd number of edges.
 *  <p>
 *  This implementation uses breadth-first search and is nonrecursive.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>isBipartite</em> and <em>color</em> operations
 *  take constant time; the <em>oddCycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  See {@link Bipartite} for a recursive version that uses depth-first search.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a>
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class BipartiteX {
private:
    bool WHITE = false;
    bool BLACK = true;

    bool bipartite;   // is the graph bipartite?
    bool *color;       // color[v] gives vertices on one side of bipartition
    bool *marked;      // marked[v] = true if v has been visited in DFS
    int *edgeTo;          // edgeTo[v] = last edge on path to v
    queue<int> cycle;  // odd-length cycle
    vector<int> vectorCycle;

    void bfs(Graph *G, int s) {
        queue<int> q;
        color[s] = WHITE;
        marked[s] = true;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int w : G->adj(v)) {
                if (!marked[w]) {
                    marked[w] = true;
                    edgeTo[w] = v;
                    color[w] = !color[v];
                    q.push(w);
                }
                else if (color[w] == color[v]) {
                    bipartite = false;

                    // to form odd cycle, consider s-v path and s-w path
                    // and let x be closest node to v and w common to two paths
                    // then (w-x path) + (x-v path) + (edge v-w) is an odd-length cycle
                    // Note: distTo[v] == distTo[w];
                    queue<int> temp;
                    swap(temp, cycle);
                    stack<int> stk;
                    int x = v, y = w;
                    while (x != y) {
                        stk.push(x);
                        cycle.push(y);
                        x = edgeTo[x];
                        y = edgeTo[y];
                    }
                    stk.push(x);
                    while (!stk.empty()) {
                        cycle.push(stk.top());
                        stk.pop();
                    }
                    cycle.push(w);
                    return;
                }
            }
        }
    }

public:
    /**
     * Determines whether an undirected graph is bipartite and finds either a
     * bipartition or an odd-length cycle.
     *
     * @param  G the graph
     */
    BipartiteX(Graph *G) {
        bipartite = true;
        color  = new bool[G->getV()];
        marked = new bool[G->getV()];
        edgeTo = new int[G->getV()];
        for (int i = 0; i < G->getV(); i++) {
            color[i] = false;
            marked[i] = false;
        }
        for (int v = 0; v < G->getV() && bipartite; v++) {
            if (!marked[v]) {
                bfs(G, v);
            }
        }
        while (!cycle.empty()) {
            vectorCycle.push_back(cycle.front());
            cycle.pop();
        }
    }

    /**
     * Returns true if the graph is bipartite.
     *
     * @return {@code true} if the graph is bipartite; {@code false} otherwise
     */
    bool isBipartite() {
        return bipartite;
    }

    /**
     * Returns the side of the bipartite that vertex {@code v} is on.
     *
     * @param  v the vertex
     * @return the side of the bipartition that vertex {@code v} is on; two vertices
     *         are in the same side of the bipartition if and only if they have the
     *         same color
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     * @throws UnsupportedOperationException if this method is called when the graph
     *         is not bipartite
     */
    bool getColor(int v) {
        if (!bipartite)
            throw runtime_error("Graph is not bipartite");
        return color[v];
    }


    /**
     * Returns an odd-length cycle if the graph is not bipartite, and
     * {@code null} otherwise.
     *
     * @return an odd-length cycle if the graph is not bipartite
     *         (and hence has an odd-length cycle), and {@code null}
     *         otherwise
     */
    vector<int> oddCycle() {
        return vectorCycle;
    }
};

#endif /* ALGORITHMS_GRAPH_CYCLE_BIPARTITEX_H_ */
