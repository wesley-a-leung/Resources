#ifndef ALGORITHMS_MATH_SIEVE2_H_
#define ALGORITHMS_MATH_SIEVE2_H_

#include <bits/stdc++.h>
using namespace std;

bool *isPrime;
vector<int> primes;
int *SPF;

// Linear Time Sieve
void sieve(int N) {
    isPrime = new bool[N + 1];
    for (int i = 0; i <= N; i++) isPrime[i] = true;
    SPF = new int[N + 1];
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= N; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            SPF[i] = i;
        }
        for (int j = 0; j < (int) primes.size() && i * primes[j] <= N && primes[j] <= SPF[i]; j++) {
            isPrime[i * primes[j]] = false;
            SPF[i * primes[j]] = primes[j];
        }
    }
}

#endif /* ALGORITHMS_MATH_SIEVE2_H_ */
