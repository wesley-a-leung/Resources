/*
 * ConnectedComponents.h
 *
 *  Created on: Aug 1, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_COMPONENTS_CONNECTEDCOMPONENTS_H_
#define ALGORITHMS_GRAPH_COMPONENTS_CONNECTEDCOMPONENTS_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

/**
 *  The {@code ConnectedComponents} class represents a data type for
 *  determining the connected components in an undirected graph.
 *  The <em>id</em> operation determines in which connected component
 *  a given vertex lies; the <em>connected</em> operation
 *  determines whether two vertices are in the same connected component;
 *  the <em>count</em> operation determines the number of connected
 *  components; and the <em>size</em> operation determines the number
 *  of vertices in the connect component containing a given vertex.

 *  The <em>component identifier</em> of a connected component is one of the
 *  vertices in the connected component: two vertices have the same component
 *  identifier if and only if they are in the same connected component.

 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>id</em>, <em>count</em>, <em>connected</em>,
 *  and <em>size</em> operations take constant time.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a>
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class ConnectedComponents {
private:
    bool *marked;      // marked[v] = has vertex v been marked?
    int *id;           // id[v] = id of connected component containing v
    vector<int> size;  // size[id] = number of vertices in given component
    int count;         // number of connected components

    // depth-first search for a Graph
    void dfs(Graph *G, int v) {
        marked[v] = true;
        id[v] = count;
        size[count]++;
        for (int w : G->adj(v)) {
            if (!marked[w]) {
                dfs(G, w);
            }
        }
    }

    // depth-first search for an EdgeWeightedGraph
    void dfs(WeightedGraph *G, int v) {
        marked[v] = true;
        id[v] = count;
        size[count]++;
        for (WeightedEdge *e : G->adj(v)) {
            int w = e->other(v);
            if (!marked[w]) {
                dfs(G, w);
            }
        }
    }

public:
    /**
     * Computes the connected components of the undirected graph {@code G}.
     *
     * @param G the undirected graph
     */
    ConnectedComponents(Graph *G) {
        marked = new bool[G->getV()];
        id = new int[G->getV()];
        size = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            marked[v] = false;
            size[v] = 0;
        }
        count = 0;
        for (int v = 0; v < G->getV(); v++) {
            if (!marked[v]) {
                size.push_back(0);
                dfs(G, v);
                count++;
            }
        }
    }

    /**
     * Computes the connected components of the edge-weighted graph {@code G}.
     *
     * @param G the edge-weighted graph
     */
    ConnectedComponents(WeightedGraph *G) {
        marked = new bool[G->getV()];
        id = new int[G->getV()];
        size = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            marked[v] = false;
            size[v] = 0;
        }
        count = 0;
        for (int v = 0; v < G->getV(); v++) {
            if (!marked[v]) {
                dfs(G, v);
                count++;
            }
        }
    }

    /**
     * Returns the component id of the connected component containing vertex {@code v}.
     *
     * @param  v the vertex
     * @return the component id of the connected component containing vertex {@code v}
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    int getId(int v) {
        return id[v];
    }

    /**
     * Returns the number of vertices in the connected component containing vertex {@code v}.
     *
     * @param  v the vertex
     * @return the number of vertices in the connected component containing vertex {@code v}
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    int getSize(int v) {
        return size[id[v]];
    }

    /**
     * Returns the number of vertices in the connected component of id {@code id}.
     *
     * @param  v the vertex
     * @return the number of vertices in the connected component of id {@code id}
     * @throws IllegalArgumentException unless {@code 0 <= id < size.length}
     */
    int getSizeOfId(int id) {
        return size[id];
    }

    /**
     * Returns the number of connected components in the graph {@code G}.
     *
     * @return the number of connected components in the graph {@code G}
     */
    int getCount() {
        return count;
    }

    /**
     * Returns true if vertices {@code v} and {@code w} are in the same
     * connected component.
     *
     * @param  v one vertex
     * @param  w the other vertex
     * @return {@code true} if vertices {@code v} and {@code w} are in the same
     *         connected component; {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     * @throws IllegalArgumentException unless {@code 0 <= w < V}
     */
    bool connected(int v, int w) {
        return getId(v) == getId(w);
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_CONNECTEDCOMPONENTS_H_ */
