/*
 * FlowNetwork.h
 *
 *  Created on: Jun 27, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_NETWORKFLOW_FLOWNETWORK_H_
#define DATASTRUCTURES_GRAPH_NETWORKFLOW_FLOWNETWORK_H_

#include <bits/stdc++.h>
#include <datastructures/graph/networkflow/FlowEdge.h>

using namespace std;

/**
 *  The {@code FlowNetwork} class represents a capacitated network
 *  with vertices named 0 through <em>V</em> - 1, where each directed
 *  edge is of type {@link FlowEdge} and has a real-valued capacity
 *  and flow.
 *  It supports the following two primary operations: add an edge to the network,
 *  iterate over all of the edges incident to or from a vertex. It also provides
 *  methods for returning the number of vertices <em>V</em> and the number
 *  of edges <em>E</em>. Parallel edges and self-loops are permitted.
 *  <p>
 *  This implementation uses an adjacency-lists representation, which
 *  is a vertex-indexed array of {@link Bag} objects.
 *  All operations take constant time (in the worst case) except
 *  iterating over the edges incident to a given vertex, which takes
 *  time proportional to the number of such edges.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/64maxflow">Section 6.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
struct FlowNetwork {
private:
    int V;
    int E;
    vector<FlowEdge*>* adjList;

public:
    /**
     * Initializes an empty flow network with {@code V} vertices and 0 edges.
     * @param V the number of vertices
     * @throws invalid_argument if {@code V < 0}
     */
    FlowNetwork(int V) {
        if (V < 0) throw new invalid_argument("Number of vertices in a Graph must be nonnegative");
        this->V = V;
        this->E = 0;
        adjList = new vector<FlowEdge*>[V];
    }

    /**
     * Returns the number of vertices in the edge-weighted graph.
     * @return the number of vertices in the edge-weighted graph
     */
    int getV() {
        return V;
    }

    /**
     * Returns the number of edges in the edge-weighted graph.
     * @return the number of edges in the edge-weighted graph
     */
    int getE() {
        return E;
    }

    /**
     * Adds the edge {@code e} to the network.
     * @param e the edge
     */
    void addEdge(FlowEdge *e) {
        int v = e->from();
        int w = e->to();
        adjList[v].push_back(e);
        adjList[w].push_back(e);
        E++;
    }

    /**
     * Returns the edges incident on vertex {@code v} (includes both edges pointing to
     * and from {@code v}).
     * @param v the vertex
     * @return the edges incident on vertex {@code v} as an Iterable
     */
    vector<FlowEdge*> adj(int v) {
        return adjList[v];
    }

    // return list of all edges - excludes self loops
    vector<FlowEdge*> edges() {
        vector<FlowEdge*> list;
        for (int v = 0; v < V; v++)
            for (FlowEdge *e : adj(v)) {
                if (e->to() != v)
                    list.push_back(e);
            }
        return list;
    }
};

#endif /* DATASTRUCTURES_GRAPH_NETWORKFLOW_FLOWNETWORK_H_ */
