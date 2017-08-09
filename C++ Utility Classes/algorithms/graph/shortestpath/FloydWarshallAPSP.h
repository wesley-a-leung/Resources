/*
 * FloydWarshallAPSP.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_FLOYDWARSHALLAPSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_FLOYDWARSHALLAPSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

double **distTo;
DirectedWeightedEdge ***edgeTo;

// takes time proportional to V^3 and space proportional to V^2
void floydWarshallAPSP(int V, double **adj) {
    distTo = new double *[V];
    edgeTo = new DirectedWeightedEdge **[V];
    for (int i = 0; i < V; i++) {
        distTo[i] = new double[V];
        edgeTo[i] = new DirectedWeightedEdge *[V];
    }

    // initialize distances to infinity
    for (int v = 0; v < V; v++) {
        for (int w = 0; w < V; w++) {
            distTo[v][w] = numeric_limits<double>::infinity();
        }
    }

    // initialize distances using edge-weighted digraph's
    for (int v = 0; v < V; v++) {
        for (int w = 0; w < V; w++) {
            if (adj[v][w] == 0) continue;
            distTo[v][w] = adj[v][w];
            edgeTo[v][w] = new DirectedWeightedEdge(v, w, adj[v][w]);
        }
        // in case of self-loops
        if (distTo[v][v] >= 0.0) {
            distTo[v][v] = 0.0;
            edgeTo[v][v] = nullptr;
        }
    }

    // Floyd-Warshall updates
    for (int i = 0; i < V; i++) {
        // compute shortest paths using only 0, 1, ..., i as intermediate vertices
        for (int v = 0; v < V; v++) {
            if (edgeTo[v][i] == nullptr) continue;  // optimization, can also check if dist[v][i] == 0.0
            for (int w = 0; w < V; w++) {
                if (distTo[v][w] > distTo[v][i] + distTo[i][w]) {
                    distTo[v][w] = distTo[v][i] + distTo[i][w];
                    edgeTo[v][w] = edgeTo[i][w];
                }
            }
            // check for negative cycle
            if (distTo[v][v] < 0.0) throw runtime_error("Graph has a negative cycle.");
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_FLOYDWARSHALLAPSP_H_ */
