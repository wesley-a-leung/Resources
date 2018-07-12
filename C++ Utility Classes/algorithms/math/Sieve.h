#ifndef ALGORITHMS_MATH_SIEVE_H_
#define ALGORITHMS_MATH_SIEVE_H_

#include <bits/stdc++.h>
using namespace std;

// generates all primes between start and end inclusive
vector<int> primes(int start, int end) {
    if (start > 1) start -= 1;
    int sqrtE = (int) sqrt(end);
    int range = end - start;
    vector<bool> sieve1(sqrtE, false);
    vector<int> list;
    sieve1[0] = true;
    for (int x = 1; x < sqrtE; x++) {
        if (sieve1[x] == false) {
            list.push_back(x + 1);
            for (int y = (x + 1) * (x + 1) - 1; y < sqrtE; y += (x + 1)) {
                sieve1[y] = true;
            }
        }
    }
    vector<bool> sieve2(range, false);
    for (int x : list) {
        for (int y = x * x - 1; y < end; y += x) {
            if (y < start) continue;
            sieve2[y - start] = true;
        }
    }
    vector<int> primes;
    for (int x = 0; x < range; x++) {
        if (sieve2[x] == false) primes.push_back(start + x + 1);
    }
    return primes;
}

#endif /* ALGORITHMS_MATH_SIEVE_H_ */
