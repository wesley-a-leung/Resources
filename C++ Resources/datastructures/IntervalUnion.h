#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adding and removing intervals from a set
// Time Complexity:
//   addInterval, removeInterval: O(log N)
// Memory Complexity: O(N)
template <class T> struct IntervalUnion : set<pair<T, T>> {
    typename set<pair<T, T>>::iterator addInterval(T L, T R) { // adds the interval [L, R)
        if (L == R) return this->end();
        auto it = this->lower_bound({L, R}), before = it;
        while (it != this->end() && it->first <= R) { R = max(R, it->second); before = it = this->erase(it); }
        if (it != this->begin() && (--it)->second >= L) { L = min(L, it->first); R = max(R, it->second); this->erase(it); }
        return this->insert(before, {L, R});
    }
    void removeInterval(T L, T R) { // removes the interval [L, R)
        if (L == R) return;
        auto it = addInterval(L, R);
        auto r2 = it->second;
        if (it->first == L) this->erase(it);
        else it->second = L;
        if (R != r2) this->emplace(R, r2);
    }
};
