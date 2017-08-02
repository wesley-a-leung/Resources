/*
 * WeightedDigraph.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_
#define DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_

#include <bits/stdc++.h>

using namespace std;

struct DirectedWeightedEdge {
private:
    int v, w;
    double weight;

public:
    DirectedWeightedEdge(int v, int w, double weight) {
        this->v = v;
        this->w = w;
        this->weight = weight;
    }

    double getWeight() {
        return weight;
    }

    int from() {
        return v;
    }

    int to() {
        return w;
    }

    bool operator == (const DirectedWeightedEdge &e) const {
        return (v == e.v && w == e.w && weight == e.weight);
    }

    bool operator != (const DirectedWeightedEdge &e) const {
        return !(v == e.v && w == e.w && weight == e.weight);
    }

    bool operator < (const DirectedWeightedEdge &e) const {
        return (weight < e.weight);
    }

    bool operator <= (const DirectedWeightedEdge &e) const {
        return (weight <= e.weight);
    }

    bool operator > (const DirectedWeightedEdge &e) const {
        return (weight > e.weight);
    }

    bool operator >= (const DirectedWeightedEdge &e) const {
        return (weight >= e.weight);
    }
};

struct WeightedDigraph {
private:
    int V;
    int E;
    vector<DirectedWeightedEdge*>* adjList;
    int *indegreeArr;

public:
    WeightedDigraph(int V) {
        this->V = V;
        this->E = 0;
        adjList = new vector<DirectedWeightedEdge*>[V];
        indegreeArr = new int[V];
    }

    int getV() {
        return this->V;
    }

    int getE() {
        return this->E;
    }

    void addEdge(DirectedWeightedEdge *e) {
        int v = e->from();
        int w = e->to();
        adjList[v].push_back(e);
        indegreeArr[w]++;
        E++;
    }

    vector<DirectedWeightedEdge*> &adj(int v) {
        return adjList[v];
    }

    int outdegree(int v) {
        return adjList[v].size();
    }

    int indegree(int v) {
        return indegreeArr[v];
    }

    vector<DirectedWeightedEdge*> &edges() {
        vector<DirectedWeightedEdge*> list;
        for (int v = 0; v < V; v++) {
            for (DirectedWeightedEdge *e : adj(v)) {
                list.push_back(e);
            }
        }
        return list;
    }
};

#endif /* DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_ */
