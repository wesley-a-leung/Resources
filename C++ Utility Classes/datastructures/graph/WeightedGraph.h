/*
 * WeightedGraph.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_
#define DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_

#include <bits/stdc++.h>

using namespace std;

struct WeightedEdge {
private:
    int v, w;
    double weight;

public:
    WeightedEdge(int v, int w, double weight) {
        this->v = v;
        this->w = w;
        this->weight = weight;
    }

    double getWeight() {
        return weight;
    }

    int either() {
        return v;
    }

    int other(int vertex) {
        if (vertex == v) return w;
        else if (vertex == w) return v;
        else throw new invalid_argument("Illegal endpoint");
    }

    bool operator == (const WeightedEdge &e) const {
        return (((v == e.v && w == e.w) || (v == e.w && w == e.v)) && weight == e.weight);
    }

    bool operator != (const WeightedEdge &e) const {
        return !(((v == e.v && w == e.w) || (v == e.w && w == e.v)) && weight == e.weight);
    }

    bool operator < (const WeightedEdge &e) const {
        return (weight < e.weight);
    }

    bool operator <= (const WeightedEdge &e) const {
        return (weight <= e.weight);
    }

    bool operator > (const WeightedEdge &e) const {
        return (weight > e.weight);
    }

    bool operator >= (const WeightedEdge &e) const {
        return (weight >= e.weight);
    }
};

struct WeightedGraph {
private:
    int V;
    int E;
    vector<WeightedEdge*>* adjList;

public:
    WeightedGraph(int V) {
        this->V = V;
        this->E = 0;
        adjList = new vector<WeightedEdge*>[V];
    }

    int getV() {
        return this->V;
    }

    int getE() {
        return this->E;
    }

    void addEdge(WeightedEdge *e) {
        int v = e->either();
        int w = e->other(v);
        adjList[v].push_back(e);
        adjList[w].push_back(e);
        E++;
    }

    vector<WeightedEdge*> adj(int v) {
        return adjList[v];
    }

    int degree(int v) {
        return adjList[v].size();
    }

    vector<WeightedEdge*> edges() {
        vector<WeightedEdge*> list;
        for (int v = 0; v < V; v++) {
            int selfLoops = 0;
            for (WeightedEdge *e : adj(v)) {
                if (e->other(v) > v) {
                    list.push_back(e);
                }
                // only add one copy of each self loop (self loops will be consecutive)
                else if (e->other(v) == v) {
                    if (selfLoops % 2 == 0) list.push_back(e);
                    selfLoops++;
                }
            }
        }
        return list;
    }
};

#endif /* DATASTRUCTURES_GRAPH_WEIGHTEDGRAPH_H_ */
