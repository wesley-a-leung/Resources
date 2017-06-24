/*
 * WeightedEdge.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_
#define DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_

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

    bool operator == (const WeightedEdge &e) {
        return (v == e.v && w == e.w && weight == e.weight);
    }

    bool operator != (const WeightedEdge &e) {
        return !(v == e.v && w == e.w && weight == e.weight);
    }

    bool operator < (const WeightedEdge &e) {
        return (weight < e.weight);
    }

    bool operator <= (const WeightedEdge &e) {
        return (weight <= e.weight);
    }

    bool operator > (const WeightedEdge &e) {
        return (weight > e.weight);
    }

    bool operator >= (const WeightedEdge &e) {
        return (weight > e.weight);
    }
};

#endif /* DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_ */
