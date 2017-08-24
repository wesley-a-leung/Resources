#ifndef ALGORITHMS_GRAPH_MST_OFFLINEDYNAMICMST_H_
#define ALGORITHMS_GRAPH_MST_OFFLINEDYNAMICMST_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

struct UF {
private:
    int* parent;
    int* vis;
    int stamp;
    char* rank; // equivalent to byte

    void prop(int p) {
        if (vis[p] != stamp) {
            vis[p] = stamp;
            parent[p] = p;
        }
    }

public:

    UF(int n) {
        parent = new int[n];
        vis = new int[n];
        rank = new char[n];
        stamp = 0;
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            vis[i] = -1;
            rank[i] = 0;
        }
    }

    void reset() {
        stamp++;
    }

    int find(int p) {
        prop(p);
        return parent[p] == p ? p : parent[p] = find(parent[p]);
    }

    bool join(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return false;

        // make root of smaller rank point to root of larger rank
        if      (rank[rootP] < rank[rootQ]) parent[rootP] = rootQ;
        else if (rank[rootP] > rank[rootQ]) parent[rootQ] = rootP;
        else {
            parent[rootQ] = rootP;
            rank[rootP]++;
        }
        return true;
    }
};

class OfflineDynamicMST {
private:
    vector<double> ans;
    int *flag;
    int *ind;
    int curFlag;
    UF *uf1, *uf2;
    vector<WeightedEdge*> edges;
    set<pair<double, int>> small, large;

public:
    OfflineDynamicMST(int V, vector<WeightedEdge*> &graphEdges, vector<pair<int, double>> &queries) {
        for (int i = 0; i < graphEdges.size(); i++) {
            edges.push_back(graphEdges[i]);
            large.insert({graphEdges[i]->getWeight(), i});
        }
        flag = new int[edges.size()];
        ind = new int[queries.size()];
        uf1 = new UF(V);
        uf2 = new UF(V);
        for (int i = 0; i < edges.size(); i++) {
            flag[i] = -1;
        }
        curFlag = 0;
        int v, w, cnt;
        double forest, mst, weight;
        // In practice, block size sqrt(queries.size()) seems to work best
        for (int l = 0, b = (int) (sqrt(queries.size()) * sqrt((double) queries.size() / (double) edges.size())), r; l < queries.size(); l += b) {
            r = min(l + b - 1, (int) queries.size() - 1);
            curFlag++;
            uf1->reset();
            small.clear();
            for (int i = l; i <= r; i++) {
                flag[queries[i].first] = curFlag;
                v = edges[queries[i].first]->either();
                w = edges[queries[i].first]->other(v);
                uf1->join(v, w);
                small.insert({edges[queries[i].first]->getWeight(), queries[i].first});
            }
            cnt = 0;
            uf2->reset();
            for (pair<double, int> p : large) {
                if (flag[p.second] != curFlag) {
                    v = edges[p.second]->either();
                    w = edges[p.second]->other(v);
                    if (uf2->join(v, w)) ind[cnt++] = p.second;
                }
            }
            uf2->reset();
            forest = 0.0;
            for (int i = 0; i < cnt; i++) {
                v = edges[ind[i]]->either();
                w = edges[ind[i]]->other(v);
                weight = edges[ind[i]]->getWeight();
                if (uf1->join(v, w)) {
                    uf2->join(v, w);
                    forest += weight;
                } else {
                    small.insert({weight, ind[i]});
                }
            }
            for (int i = l; i <= r; i++) {
                v = edges[queries[i].first]->either();
                w = edges[queries[i].first]->other(v);
                weight = edges[queries[i].first]->getWeight();
                large.erase({weight, queries[i].first});
                small.erase({weight, queries[i].first});
                weight = queries[i].second;
                edges[queries[i].first] = new WeightedEdge(v, w, weight);
                large.insert({weight, queries[i].first});
                small.insert({weight, queries[i].first});
                uf1->reset();
                mst = 0.0;
                for (pair<double, int> p : small) {
                    v = edges[p.second]->either();
                    w = edges[p.second]->other(v);
                    if (uf1->join(uf2->find(v), uf2->find(w))) {
                        mst += edges[p.second]->getWeight();
                    }
                }
                ans.push_back(forest + mst);
            }
        }
    }

    OfflineDynamicMST(WeightedGraph *G, int Q, vector<pair<int, double>> &queries) : OfflineDynamicMST(G->getV(), G->edges(), queries) {}

    vector<double> &getAnswers() {
        return ans;
    }
};

#endif /* ALGORITHMS_GRAPH_MST_OFFLINEDYNAMICMST_H_ */
