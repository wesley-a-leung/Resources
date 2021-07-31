#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the assignment problem of matching N workers to M jobs with the
//   minimum cost where each worker can only be assigned to at most 1 job and
//   each job can only be assigned to at most 1 worker and there are exactly
//   min(N, M) assignments
// Maximum cost assignment can be found by negating all non infinity values in
//   the matrix and taking the negative of the minimum cost
// Maximum cost matching can be found by negating all non infinity values in
//   the matrix, setting all infinity values to 0 and taking the negative of
//   the minimum cost
// Template Arguments:
//   T: the type of the cost
// Constructor Arguments:
//   A: a matrix of size N x M with A[i][j] representing the cost of assigning
//     worker i to job j
//   INF: a value for infinity
// Fields:
//   N: the number of workers
//   M: the number of jobs
//   cost: the minimum cost for a valid assignment
//   jobForWorker: a vector representing the job assigned to each worker, or -1
//     if that worker is not assigned any job
//   workerForJob: a vector representing the worker assigned to each job, or -1
//     if that job is not assigned any worker
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N^2 M)
// Memory Complexity: O(NM)
// Tested:
//   https://judge.yosupo.jp/problem/assignment
//   https://open.kattis.com/problems/cordonbleu
//   https://www.spoj.com/problems/BABY/
//   https://dmoj.ca/problem/tle17c7p5
//   https://ecna20.kattis.com/problems/ecna20.workers
template <class T> struct HungarianAlgorithm {
  int N, M; T cost; vector<int> jobForWorker, workerForJob;
  HungarianAlgorithm(vector<vector<T>> A, T INF = numeric_limits<T>::max())
      : N(A.size()), M(A.empty() ? 0 : A[0].size()), cost(T()) {
    bool rev = N > M; if (rev) {
      swap(N, M); vector<vector<T>> B(N, vector<T>(M));
      for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
        B[i][j] = A[j][i];
      A.swap(B);
    }
    jobForWorker.assign(N, -1); workerForJob.assign(M + 1, N);
    auto add = [&] (pair<int, T> &a, const pair<int, T> &b) {
      a.first += b.first; a.second += b.second;
    };
    auto sub = [&] (pair<int, T> &a, const pair<int, T> &b) {
      a.first -= b.first; a.second -= b.second;
    };
    vector<pair<int, T>> d1(N + 1, make_pair(0, T()));
    vector<pair<int, T>> d2(M + 1, make_pair(0, T()));
    for (int i = 0; i < N; i++) {
      int j0 = M; workerForJob[j0] = i;
      vector<pair<int, T>> dist(M + 1, make_pair(1, T()));
      vector<int> par(M + 1, -1); vector<bool> done(M + 1, false); do {
        done[j0] = true; int i0 = workerForJob[j0], j1 = M;
        pair<int, T> delta = make_pair(1, T());
        for (int j = 0; j < M; j++) if (!done[j]) {
          pair<int, T> d = A[i0][j] >= INF ? make_pair(1, T())
                                           : make_pair(0, A[i0][j]);
          sub(d, d1[i0]); sub(d, d2[j]);
          if (dist[j].first > 0 || d < dist[j]) { dist[j] = d; par[j] = j0; }
          if (dist[j] < delta) delta = dist[j1 = j];
        }
        j0 = j1; for (int j = 0; j <= M; j++) {
          if (done[j]) { add(d1[workerForJob[j]], delta); sub(d2[j], delta); }
          else sub(dist[j], delta);
        }
      } while (workerForJob[j0] != N);
      for (; j0 != M; j0 = par[j0]) workerForJob[j0] = workerForJob[par[j0]];
    }
    workerForJob.pop_back(); for (int j = 0; j < M; j++) {
      if (workerForJob[j] == N) workerForJob[j] = -1;
      else jobForWorker[workerForJob[j]] = j;
    }
    cost = d2[M].first < 0 ? INF : -d2[M].second;
    if (rev) { swap(N, M); jobForWorker.swap(workerForJob); }
  }
};
