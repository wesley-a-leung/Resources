#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines whether a majority element exists.
// The first pass will find the majority element if it exists,
// but a second pass is required to determine if it is the majority element.
// Time Complexity: O(N)
// Memory Complexity: O(1)
template <class It> pair<typename std::iterator_traits<It>::value_type, bool> boyerMooreMajority(It st, It en) {
    typename std::iterator_traits<It>::value_type ret = *st; int cnt = 0;
    for (It cur = st; cur != en; cur++) {
        if (cnt == 0) { ret = *cur; cnt++; }
        else if (*cur == ret) cnt++;
        else cnt--;
    }
    cnt = 0;
    for (It cur = st; cur != en; cur++) if (*cur == ret) cnt++;
    return make_pair(ret, cnt > (en - st) / 2);
}
