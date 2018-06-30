#ifndef ALGORITHMS_STRING_SUFFIXAUTOMATAX_H_
#define ALGORITHMS_STRING_SUFFIXAUTOMATAX_H_

#include <bits/stdc++.h>
using namespace std;

class SuffixAutomata {
public: // these should really be private
    vector<unordered_map<char, int>> to;
    vector<int> len, link;

private:
    int last;

public:
    void reset() {
        to.clear();
        len.clear();
        link.clear();
        last = 0;
        to.emplace_back();
        len.push_back(0);
        link.push_back(0);
    }

    void addLetter(char c) {
        int p = last, q;
        if (to[p].count(c)) {
            q = to[p][c];
            if (len[q] == len[p] + 1) {
                last = q;
                return;
            }
            to.push_back(to[q]);
            len.push_back(len[p] + 1);
            link.push_back(link[q]);
            link[q] = ((int) link.size()) - 1;
            while (to[p][c] == q) {
                to[p][c] = ((int) to.size()) - 1;
                p = link[p];
            }
        } else {
            last = (int) to.size();
            to.emplace_back();
            len.push_back(len[p] + 1);
            link.push_back(0);
            while (to[p][c] == 0) {
                to[p][c] = last;
                p = link[p];
            }
            if (to[p][c] == last) {
                link[last] = p;
                return;
            }
            q = to[p][c];
            if (len[q] == len[p] + 1) {
                link[last] = q;
                return;
            }
            to.push_back(to[q]);
            len.push_back(len[p] + 1);
            link.push_back(link[q]);
            link[q] = ((int) link.size()) - 1;
            link[last] = ((int) link.size()) - 1;
            while (to[p][c] == q) {
                to[p][c] = ((int) to.size()) - 1;
                p = link[p];
            }
        }
    }

    SuffixAutomata() {
        reset();
    }

    void add(string &s) {
        last = 0;
        for (char c : s) addLetter(c);
    }
};

// longest common substring
int LCS(SuffixAutomata &SA, string &s) {
    int p = 0, len = 0, ret = 0;
    for (char c : s) {
        while (p != 0 && !SA.to[p].count(c)) {
            p = SA.link[p];
            len = SA.len[p];
        }
        if (SA.to[p].count(c)) {
            p = SA.to[p][c];
            len++;
        }
        ret = max(ret, len);
    }
    return ret;
}

#endif /* ALGORITHMS_STRING_SUFFIXAUTOMATAX_H_ */
