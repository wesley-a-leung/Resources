/*
 * FlowNetwork.h
 *
 *  Created on: Jun 27, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_NETWORKFLOW_FLOWNETWORK_H_
#define DATASTRUCTURES_GRAPH_NETWORKFLOW_FLOWNETWORK_H_

#include <bits/stdc++.h>

using namespace std;

/**
 *  The {@code FlowEdge} class represents a capacitated edge with a
 *  flow in a {@link FlowNetwork}. Each edge consists of two integers
 *  (naming the two vertices), a real-valued capacity, and a real-valued
 *  flow. The data type provides methods for accessing the two endpoints
 *  of the directed edge and the weight. It also provides methods for
 *  changing the amount of flow on the edge and determining the residual
 *  capacity of the edge.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/64maxflow">Section 6.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
struct FlowEdge {
private:
    // to deal with floating-point roundoff errors
    double FLOATING_POINT_EPSILON = 1E-10;

    int v;             // from
    int w;             // to
    double capacity;   // capacity
    double flow;             // flow

public:
    /**
     * Initializes an edge from vertex {@code v} to vertex {@code w} with
     * the given {@code capacity} and zero flow.
     * @param v the tail vertex
     * @param w the head vertex
     * @param capacity the capacity of the edge
     * @throws invalid_argument if either {@code v} or {@code w}
     *    is a negative integer
     * @throws invalid_argument if {@code capacity < 0.0}
     */
    FlowEdge(int v, int w, double capacity) {
        if (v < 0) throw invalid_argument("vertex index must be a non-negative integer");
        if (w < 0) throw invalid_argument("vertex index must be a non-negative integer");
        if (!(capacity >= 0.0)) throw invalid_argument("Edge capacity must be non-negative");
        this->v         = v;
        this->w         = w;
        this->capacity  = capacity;
        this->flow      = 0.0;
    }

    /**
     * Initializes an edge from vertex {@code v} to vertex {@code w} with
     * the given {@code capacity} and {@code flow}.
     * @param v the tail vertex
     * @param w the head vertex
     * @param capacity the capacity of the edge
     * @param flow the flow on the edge
     * @throws invalid_argument if either {@code v} or {@code w}
     *    is a negative integer
     * @throws invalid_argument if {@code capacity} is negative
     * @throws invalid_argument unless {@code flow} is between
     *    {@code 0.0} and {@code capacity}.
     */
    FlowEdge(int v, int w, double capacity, double flow) {
        if (v < 0) throw invalid_argument("vertex index must be a non-negative integer");
        if (w < 0) throw invalid_argument("vertex index must be a non-negative integer");
        if (!(capacity >= 0.0))  throw invalid_argument("edge capacity must be non-negative");
        if (!(flow <= capacity)) throw invalid_argument("flow exceeds capacity");
        if (!(flow >= 0.0))      throw invalid_argument("flow must be non-negative");
        this->v         = v;
        this->w         = w;
        this->capacity  = capacity;
        this->flow      = flow;
    }

    /**
     * Initializes a flow edge from another flow edge.
     * @param e the edge to copy
     */
    FlowEdge(FlowEdge *e) {
        this->v         = e->v;
        this->w         = e->w;
        this->capacity  = e->capacity;
        this->flow      = e->flow;
    }

    /**
     * Returns the tail vertex of the edge.
     * @return the tail vertex of the edge
     */
    int from() {
        return v;
    }

    /**
     * Returns the head vertex of the edge.
     * @return the head vertex of the edge
     */
    int to() {
        return w;
    }

    /**
     * Returns the capacity of the edge.
     * @return the capacity of the edge
     */
    double getCapacity() {
        return capacity;
    }

    /**
     * Returns the flow on the edge.
     * @return the flow on the edge
     */
    double getFlow() {
        return flow;
    }

    /**
     * Returns the endpoint of the edge that is different from the given vertex
     * (unless the edge represents a self-loop in which case it returns the same vertex).
     * @param vertex one endpoint of the edge
     * @return the endpoint of the edge that is different from the given vertex
     *   (unless the edge represents a self-loop in which case it returns the same vertex)
     * @throws invalid_argument if {@code vertex} is not one of the endpoints
     *   of the edge
     */
    int other(int vertex) {
        if      (vertex == v) return w;
        else if (vertex == w) return v;
        else throw invalid_argument("invalid endpoint");
    }

    /**
     * Returns the residual capacity of the edge in the direction
     *  to the given {@code vertex}.
     * @param vertex one endpoint of the edge
     * @return the residual capacity of the edge in the direction to the given vertex
     *   If {@code vertex} is the tail vertex, the residual capacity equals
     *   {@code capacity() - flow()}; if {@code vertex} is the head vertex, the
     *   residual capacity equals {@code flow()}.
     * @throws invalid_argument if {@code vertex} is not one of the endpoints of the edge
     */
    double residualCapacityTo(int vertex) {
        if      (vertex == v) return flow;              // backward edge
        else if (vertex == w) return capacity - flow;   // forward edge
        else throw invalid_argument("invalid endpoint");
    }

    /**
     * Increases the flow on the edge in the direction to the given vertex.
     *   If {@code vertex} is the tail vertex, this increases the flow on the edge by {@code delta};
     *   if {@code vertex} is the head vertex, this decreases the flow on the edge by {@code delta}.
     * @param vertex one endpoint of the edge
     * @param delta amount by which to increase flow
     * @throws invalid_argument if {@code vertex} is not one of the endpoints
     *   of the edge
     * @throws invalid_argument if {@code delta} makes the flow on
     *   on the edge either negative or larger than its capacity
     * @throws invalid_argument if {@code delta} is {@code NaN}
     */
    void addResidualFlowTo(int vertex, double delta) {
        if (!(delta >= 0.0)) throw invalid_argument("Delta must be nonnegative");

        if      (vertex == v) flow -= delta;           // backward edge
        else if (vertex == w) flow += delta;           // forward edge
        else throw invalid_argument("invalid endpoint");

        // round flow to 0 or capacity if within floating-point precision
        if (abs(flow) <= FLOATING_POINT_EPSILON)
            flow = 0;
        if (abs(flow - capacity) <= FLOATING_POINT_EPSILON)
            flow = capacity;

        if (!(flow >= 0.0))      throw invalid_argument("Flow is negative");
        if (!(flow <= capacity)) throw invalid_argument("Flow exceeds capacity");
    }
};

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
