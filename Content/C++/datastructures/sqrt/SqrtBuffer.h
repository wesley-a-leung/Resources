#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the rank of an element in a multiset, allowing for multiple
//   insertions of the same element at the same time (including negative)
// Note that elements with a count of 0 or negative are still in the multiset
//   as elements cannot be removed
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the values being stored
//   CountType: the type of the count of elements in the set
//   Cmp: the comparator to compare two f(x) values,
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   v: a vector of type pair<T, CountType> to initialize the structure
//   SCALE: the value to scale sqrt by
// Functions:
//   rebuild(): rebuilds the multiset by moving all elements in the small
//     container to the large container
//   insert(p): inserts the element p.first with a count of p.second
//     into the multiset
//   emplace(v, c): inserts the element v with a count of c
//     into the multiset
//   aboveInd(val): returns the index of the smallest element where
//     val compares less than that element
//   ceilingInd(val): returns the index of the smallest element that
//     does not compare less than val
//   floorInd(val): returns the index of the largest element where
//     val does not compare less than that element
//   belowInd(val): returns the index of the largest element that
//     compares less than val
//   contains(val): returns whether val is in the multiset or not
//   count(lo, hi): returns the number of values in the range [lo, hi]
//   empty(): returns whether the multiset is empty or not
//   count(): returns the number of elements the multiset
//   clear(): clears the multiset
//   values(): returns a vector of pairs containing the sorted values and
//     their count in the multiset
// In practice, has a very small constant, performs similarly to RootArray,
//   and is faster than balanced binary search trees
// Time Complexity:
//   constructor: O(N)
//   insert: O(1) amortized
//   rebuild: O(sqrt N)
//   count(): O(1)
//   floor, ceiling, above, below, contains, count(val), count(lo, hi):
//     O(sqrt N) amortized
//   valuesAndCount: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/apio19p3
//   https://dmoj.ca/problem/ioi01p1
template <class T, class CountType, class Cmp = less<T>>
struct SqrtBuffer {
  CountType tot; double SCALE;
  vector<pair<T, CountType>> small, large;
  struct PairCmp {
    bool operator () (const pair<T, CountType> &a,
                      const pair<T, CountType> &b) const {
      return Cmp()(a.first, b.first);
    }
  };
  SqrtBuffer(double SCALE = 1) : tot(CountType()), SCALE(SCALE) {}
  SqrtBuffer(const vector<pair<T, CountType>> &v, double SCALE = 1)
      : SqrtBuffer(SCALE) {
    assert(is_sorted(v.begin(), v.end(), PairCmp()));
    large.insert(large.end(), v.begin(), v.end());
    resizeUnique(large); for (auto &&p : large) tot += p.second;
  }
  void resizeUnique(vector<pair<T, CountType>> &v) {
    if (!v.empty()) {
      int j = 0; for (int i = 1; i < int(v.size()); i++) {
        if (Cmp()(v[i].first, v[j].first) || Cmp()(v[j].first, v[i].first)) {
          v[++j] = v[i]; v[j].second += v[j - 1].second;
        } else v[j].second += v[i].second;
      }
      v.erase(v.begin() + j + 1, v.end());
    }
  }
  bool rebuild() {
    if (int(small.size()) > SCALE * sqrt(small.size() + large.size())) {
      int lSz = large.size(); sort(small.begin(), small.end(), PairCmp());
      for (int i = lSz - 1; i >= 1; i--)
        large[i].second -= large[i - 1].second;
      for (auto &&p : small) large.push_back(p);
      inplace_merge(large.begin(), large.begin() + lSz, large.end(),
                    PairCmp());
      resizeUnique(large); small.clear(); return true;
    }
    return false;
  }
  void insert(const pair<T, CountType> &p) {
    small.push_back(p); tot += p.second;
  }
  void emplace(const T &v, const CountType &c) {
    small.emplace_back(v, c); tot += c;
  }
  CountType aboveInd(const T &val) {
    rebuild();
    int ind = upper_bound(large.begin(), large.end(),
                          make_pair(val, CountType()), PairCmp())
        - large.begin();
    CountType ret = ind == 0 ? CountType() : large[ind - 1].second;
    for (auto &&p : small) if (!Cmp()(val, p.first)) ret += p.second;
    return ret;
  }
  CountType ceilingInd(const T &val) {
    rebuild();
    int ind = lower_bound(large.begin(), large.end(),
                          make_pair(val, CountType()), PairCmp())
        - large.begin();
    CountType ret = ind == 0 ? CountType() : large[ind - 1].second;
    for (auto &&p : small) if (Cmp()(p.first, val)) ret += p.second;
    return ret;
  }
  CountType floorInd(const T &val) { return aboveInd(val) - 1; }
  CountType belowInd(const T &val) { return ceilingInd(val) - 1; }
  bool contains(const T &val) {
    if (binary_search(large.begin(), large.end(),
        make_pair(val, CountType()), PairCmp())) return true;
    if (rebuild() && binary_search(large.begin(), large.end(),
                                   make_pair(val, CountType()), PairCmp()))
      return true;
    for (auto &&p : small) if (!Cmp()(val, p.first) && !Cmp()(p.first, val))
      return true;
    return false;
  }
  CountType count(const T &lo, const T &hi) {
    rebuild();
    int ind = upper_bound(large.begin(), large.end(),
                          make_pair(hi, CountType()), PairCmp())
        - large.begin();
    CountType ret = ind == 0 ? CountType() : large[ind - 1].second;
    ind = lower_bound(large.begin(), large.end(),
                      make_pair(lo, CountType()), PairCmp())
        - large.begin();
    ret -= ind == 0 ? CountType() : large[ind - 1].second;
    for (auto &&p : small) if (!Cmp()(p.first, lo) && !Cmp()(hi, p.first))
      ret += p.second;
    return ret;
  }
  CountType count() const { return tot; } 
  void clear() { tot = CountType(); small.clear(); large.clear(); }
  vector<pair<T, CountType>> valuesAndCount() const {
    vector<pair<T, CountType>> ret; ret.reserve(large.size() + small.size());
    for (auto &&p : small) ret.push_back(p);
    int mid = int(ret.size()); for (auto &&p : large) ret.push_back(p);
    inplace_merge(ret.begin(), ret.begin() + mid, ret.end(), PairCmp());
    resizeUnique(ret); return ret;
  }
};
