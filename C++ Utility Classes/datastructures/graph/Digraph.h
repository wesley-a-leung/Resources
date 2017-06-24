/*
 * Digraph.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_
#define DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_

#include <bits/stdc++.h>

using namespace std;

struct Digraph {
private:
    int V;
    int E;
    vector<int>* adjList;
    int *indegreeArr;

public:
    Digraph(int V) {
        this->V = V;
        this->E = 0;
        adjList = new vector<int>[V];
        indegreeArr = new int[V];
    }

    int getV() {
        return this->V;
    }

    int getE() {
        return this->E;
    }

    void addEdge(int v, int w) {
        adjList[v].push_back(w);
        indegreeArr[w]++;
        E++;
    }

    vector<int> adj(int v) {
        return adjList[v];
    }

    int outdegree(int v) {
        return adjList[v].size();
    }

    int indegree(int v) {
        return indegreeArr[v];
    }
};

#endif /* DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_ */
