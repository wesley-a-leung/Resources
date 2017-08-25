/*
 * Digraph.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_DIGRAPH_H_
#define DATASTRUCTURES_GRAPH_DIGRAPH_H_

#include <bits/stdc++.h>

using namespace std;

struct Digraph {
private:
    int V;
    int E;
    vector<int> *adjList;
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

    vector<int> &adj(int v) {
        return adjList[v];
    }

    int outdegree(int v) {
        return adjList[v].size();
    }

    int indegree(int v) {
        return indegreeArr[v];
    }

    Digraph *reverse() {
        Digraph *reverse = new Digraph(V);
        for (int v = 0; v < V; v++) {
            for (int w : adj(v)) {
                reverse->addEdge(w, v);
            }
        }
        return reverse;
    }

    vector<pair<int, int>> &edges() {
        vector<pair<int, int>> *list = new vector<pair<int, int>>;
        for (int v = 0; v < V; v++) {
            for (int w : adj(v)) {
                list->push_back(make_pair(v, w));
            }
        }
        return *list;
    }
};

#endif /* DATASTRUCTURES_GRAPH_DIGRAPH_H_ */
