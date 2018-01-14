#ifndef ALGORITHMS_GRAPH_MAXIMUMNONADJACENTSUM_H_
#define ALGORITHMS_GRAPH_MAXIMUMNONADJACENTSUM_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Digraph.h>

using namespace std;

/**
 * Computes the maximum sum of non adjacent vertices on any path between a source
 * and destination vertex on a Directed Acyclic Graph. If no destination vertex is
 * selected, then any end point is a valid path.
 *
 * Takes time proportional to <em>V</em> where V is the number of vertices in the graph.
 *
 * Takes space proportional to <em>V</em> where V is the number of vertices in the graph.
 */
class MaximumNonadjacentSum {
private:
    int src, dest;
    Digraph *G;
    long long *value, MOD;
    pair<long long, long long> ans, **dp;

    pair<long long, long long> solve(int v, bool take) {
        if (v == dest) {
            if (take) return make_pair(value[v], 1LL);
            else return make_pair(0LL, 1LL);
        }
        if (dp[v][take] != make_pair(-1LL, -1LL)) return dp[v][take];
        pair<long long, long long> ret = make_pair(LONG_LONG_MIN, 0LL);
        pair<long long, long long> hold;
        for (int w : G->adj(v)) {
            if (take) {
                hold = solve(w, false);
                hold.first += value[v];
                if (hold.first > ret.first) ret = hold;
                else if (hold.first == ret.first) ret.second = (ret.second + hold.second) % MOD;
            }
            hold = solve(w, true);
            if (hold.first > ret.first) ret = hold;
            else if (hold.first == ret.first) ret.second = (ret.second + hold.second) % MOD;
        }
        return dp[v][take] = ret;
    }

public:
    /**
     * Computes the maximum sum of non adjacent vertices on any path between src and dest,
     * as well as the number of paths between src and dest with this sum.
     *
     * @param G the directed acyclic graph
     * @param value the value for each vertex
     * @param src the source vertex
     * @param dest the destination vertex, -1 if no specific destination
     * @param MOD the modulus for the number of paths that have the same value
     */
    MaximumNonadjacentSum(Digraph *G, long long *value, int src, int dest, long long MOD) {
        this->G = G;
        this->value = value;
        this->src = src;
        this->dest = dest;
        this->MOD = MOD;
        dp = new pair<long long, long long> *[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            dp[v] = new pair<long long, long long>[2];
            dp[v][0] = make_pair(-1LL, -1LL);
            dp[v][1] = make_pair(-1LL, -1LL);
        }
        ans = solve(src, true);
    }

    /**
     * Returns the maximum sum of non adjacent vertices on any path between src and dest.
     */
    long long getValue() {
        return ans.first;
    }

    /**
     * Returns the number of paths modulus MOD between src and dest with the maximum sum.
     */
    long long getCount() {
        return ans.second;
    }
};

#endif /* ALGORITHMS_GRAPH_MAXIMUMNONADJACENTSUM_H_ */
