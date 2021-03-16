#include <bits/stdc++.h>
#include "../../../../Content/C++/geometry/2d/Line.h"
#include "../../../../Content/C++/geometry/2d/Angle.h"
#include "../../../../Content/C++/geometry/2d/Polygon.h"
using namespace std;

vector<pt> generateConvexPolygon(int N, mt19937_64 &rng) {
  uniform_real_distribution<T> dis(0, 1);
  vector<T> X(N), Y(N);
  for (int i = 0; i < N; i++) X[i] = dis(rng);
  for (int i = 0; i < N; i++) Y[i] = dis(rng);
  sort(X.begin(), X.end());
  sort(Y.begin(), Y.end());
  T xmin = X[0], xmax = X[N - 1], ymin = Y[0], ymax = Y[N - 1];
  vector<T> xv, yv;
  T lastTop = xmin, lastBot = xmin;
  for (int i = 1; i < N - 1; i++) {
    if (rng() % 2) {
      xv.push_back(X[i] - lastTop);
      lastTop = X[i];
    } else {
      xv.push_back(lastBot - X[i]);
      lastBot = X[i];
    }
  }
  xv.push_back(xmax - lastTop);
  xv.push_back(lastBot - xmax);
  T lastLeft = ymin, lastRight = ymin;
  for (int i = 1; i < N - 1; i++) {
    if (rng() % 2) {
      yv.push_back(Y[i] - lastLeft);
      lastLeft = Y[i];
    } else {
      yv.push_back(lastRight - Y[i]);
      lastRight = Y[i];
    }
  }
  yv.push_back(ymax - lastLeft);
  yv.push_back(lastRight - ymax);
  shuffle(yv.begin(), yv.end(), rng);
  vector<pt> V(N), P;
  for (int i = 0; i < N; i++) V[i] = pt(xv[i], yv[i]);
  sort(V.begin(), V.end(), [&] (const pt &a, const pt &b) {
    return Angle(a) < Angle(b);
  });
  T x = 0, xminPoly = 0, y = 0, yminPoly = 0;
  for (int i = 0; i < N; i++) {
    P.emplace_back(x, y);
    xminPoly = min(xminPoly, x += V[i].x);
    yminPoly = min(yminPoly, y += V[i].y);
  }
  x = xmin - xminPoly;
  y = ymin - yminPoly;
  for (int i = 0; i < N; i++) P[i] += pt(x, y);
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
    assert(segSegIntersects(P[i], P[mod(i + 1, N)], P[j], P[mod(j + 1, N)]) != 1);
  if (N == 2) assert(P[0] != P[1]);
  if (N >= 3) for (int i = 0; i < N; i++) assert(ccw(P[mod(i + N - 1, N)], P[i], P[mod(i + 1, N)]) > 0);
  return P;
}

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 10 + 1;
    vector<pt> poly = generateConvexPolygon(N, rng);
    int Q = 100;
    uniform_real_distribution<T> dis(-10, 10);
    for (int i = 0; i < Q; i++) {
      pt p;
      do {
        p = pt(dis(rng), dis(rng));
      } while (isInConvexPolygon(poly, p) <= 0);
      if (N >= 2 && rng() % 10 == 0) {
        int j = rng() % N;
        int k = mod(j + 1, N);
        if (rng() % 2) swap(j, k);
        p = poly[j] * T(2) - poly[k];
      }
      pair<int, int> tangent = convexPolygonPointTangent(poly, p);
      assert(0 <= tangent.first && tangent.first < N);
      assert(0 <= tangent.second && tangent.second < N);
      checkSum = 31 * checkSum + tangent.first;
      checkSum = 31 * checkSum + tangent.second;
      if (N == 1) {
        assert(tangent.first == tangent.second);
      } else if (N == 2) {
        if (tangent.first == tangent.second) assert(ccw(poly[0], poly[1], p) == 0);
      } else assert(tangent.first != tangent.second);
      Line l1(p, poly[tangent.first]), l2(p, poly[tangent.second]);
      for (int i = 0; i < N; i++) {
        assert(l1.onLeft(poly[i]) < 0 || (l1.onLeft(poly[i]) == 0 && le(distSq(p, poly[tangent.first]), distSq(p, poly[i]))));
        assert(l2.onLeft(poly[i]) > 0 || (l2.onLeft(poly[i]) == 0 && le(distSq(p, poly[tangent.second]), distSq(p, poly[i]))));
      }
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 (Convex Polygon Point Tangent) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test2() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 5e4;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 10 + 1;
    vector<pt> poly = generateConvexPolygon(N, rng);
    int Q = 100;
    uniform_real_distribution<T> dis(-10, 10);
    for (int i = 0; i < Q; i++) {
      pt p;
      do {
        p = pt(dis(rng), dis(rng));
      } while (isInConvexPolygon(poly, p) <= 0);
      if (N >= 2 && rng() % 10 == 0) {
        int j = rng() % N;
        int k = mod(j + 1, N);
        if (rng() % 2) swap(j, k);
        p = poly[j] * T(2) - poly[k];
      }
      pt closest = closestPointOnConvexPolygon(poly, p);
      for (int j = 0; j < N; j++) {
        pt q = closestPtOnSeg(p, poly[j], poly[mod(j + 1, N)]);
        assert(le(distSq(p, closest), distSq(p, q)));
      }
      checkSum = 31 * checkSum + distSq(p, closest);
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Closest Point on Convex Polygon) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  test2();
  cout << "Test Passed" << endl;
  return 0;
}
