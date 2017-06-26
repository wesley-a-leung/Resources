/*
 * DirectedWeightedEdge.h
 *
 *  Created on: Jun 23, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_
#define DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_

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
        return (weight > e.weight);
    }
};

#endif /* DATASTRUCTURES_GRAPH_WEIGHTEDEDGE_H_ */
