/*
 * Cycle.h
 *
 *  Created on: Jun 28, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_CYCLE_CYCLE_H_
#define ALGORITHMS_GRAPH_CYCLE_CYCLE_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 *  The {@code Cycle} class represents a data type for
 *  determining whether an undirected graph has a cycle.
 *  The <em>hasCycle</em> operation determines whether the graph has
 *  a cycle and, if so, the <em>cycle</em> operation returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasCycle</em> operation takes constant time;
 *  the <em>cycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a>
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class Cycle {
private:
    bool *marked;
    int *edgeTo;
    vector<int> cycle;

    // does this graph have a self loop?
    // side effect: initialize cycle to be self loop
    bool hasSelfLoop(Graph *G) {
        for (int v = 0; v < G->getV(); v++) {
            for (int w : G->adj(v)) {
                if (v == w) {
                    cycle.clear();
                    cycle.push_back(v);
                    cycle.push_back(v);
                    return true;
                }
            }
        }
        return false;
    }

    // does this graph have two parallel edges?
    // side effect: initialize cycle to be two parallel edges
    bool hasParallelEdges(Graph *G) {
        marked = new bool[G->getV()];
        for (int i = 0; i < G->getV(); i++) {
            marked[i] = false;
        }

        for (int v = 0; v < G->getV(); v++) {

            // check for parallel edges incident to v
            for (int w : G->adj(v)) {
                if (marked[w]) {
                    cycle.clear();
                    cycle.push_back(v);
                    cycle.push_back(w);
                    cycle.push_back(v);
                    return true;
                }
                marked[w] = true;
            }

            // reset so marked[v] = false for all v
            for (int w : G->adj(v)) {
                marked[w] = false;
            }
        }
        return false;
    }

    void dfs(Graph *G, int u, int v) {
        marked[v] = true;
        for (int w : G->adj(v)) {

            // short circuit if cycle already found
            if (cycle.size() != 0) return;

            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(G, v, w);
            }

            // check for cycle (but disregard reverse of edge leading to v)
            else if (w != u) {
                cycle.clear();
                for (int x = v; x != w; x = edgeTo[x]) {
                    cycle.push_back(x);
                }
                cycle.push_back(w);
                cycle.push_back(v);
            }
        }
    }

public:
    /**
     * Determines whether the undirected graph {@code G} has a cycle and,
     * if so, finds such a cycle.
     *
     * @param G the undirected graph
     */
    Cycle(Graph *G) {
        if (hasSelfLoop(G)) return;
        if (hasParallelEdges(G)) return;
        marked = new bool[G->getV()];
        for (int i = 0; i < G->getV(); i++) {
            marked[i] = false;
        }
        edgeTo = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++)
            if (!marked[v])
                dfs(G, -1, v);
    }

    /**
     * Returns true if the graph {@code G} has a cycle.
     *
     * @return {@code true} if the graph has a cycle; {@code false} otherwise
     */
    bool hasCycle() {
        return cycle.size() != 0;
    }

     /**
     * Returns a cycle in the graph {@code G}.
     * @return a cycle if the graph {@code G} has a cycle,
     *         and {@code null} otherwise
     */
    vector<int> getCycle() {
        return cycle;
    }
};

#endif /* ALGORITHMS_GRAPH_CYCLE_CYCLE_H_ */
