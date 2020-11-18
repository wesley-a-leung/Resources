#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the assignment problem of matching N workers to M jobs with the
//   minimum cost where each worker can only be assigned to at most 1 job and
//   each job can only be assigned to at most 1 worker
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
    jobForWorker.assign(N, -1), workerForJob.assign(M + 1, N);
    vector<T> d1(N + 1, T()), d2(M + 1, T()); for (int i = 0; i < N; i++) {
      int j0 = M; workerForJob[j0] = i; vector<T> dist(M + 1, INF);
      vector<int> par(M + 1, -1); vector<bool> done(M + 1, false); do {
        done[j0] = true; int i0 = workerForJob[j0], j1 = M; T delta = INF;
        for (int j = 0; j < M; j++) if (!done[j]) {
          T d = A[i0][j] - d1[i0] - d2[j];
          if (d < dist[j]) { dist[j] = d; par[j] = j0; }
          if (dist[j] < delta) delta = dist[j1 = j];
        }
        j0 = j1; for (int j = 0; j <= M; j++) {
          if (done[j]) { d1[workerForJob[j]] += delta; d2[j] -= delta; }
          else dist[j] -= delta;
        }
      } while (workerForJob[j0] != N);
      for (; j0 != M; j0 = par[j0]) workerForJob[j0] = workerForJob[par[j0]];
    }
    workerForJob.pop_back(); for (int j = 0; j < M; j++) {
      if (workerForJob[j] == N) workerForJob[j] = -1;
      else jobForWorker[workerForJob[j]] = j;
    }
    cost = -d2[M]; if (rev) { swap(N, M); jobForWorker.swap(workerForJob); }
  }
};
