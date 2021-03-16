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
  return P;
}

void test1() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 10 + 1;
    vector<pt> poly = generateConvexPolygon(N, rng);
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
      assert(segSegIntersects(poly[i], poly[mod(i + 1, N)], poly[j], poly[mod(j + 1, N)]) != 1);
    if (N == 2) assert(poly[0] != poly[1]);
    if (N >= 3) for (int i = 0; i < N; i++) assert(ccw(poly[mod(i + N - 1, N)], poly[i], poly[mod(i + 1, N)]) > 0);
    int Q = rng() % 100 + 1;
    uniform_real_distribution<T> dis(-10, 10);
    for (int i = 0; i < Q; i++) {
      pt p;
      do {
        p = pt(dis(rng), dis(rng));
      } while (isInConvexPolygon(poly, p) <= 0);
      if (N == 2 && rng() % 10 == 0) p = poly[1] * T(2) - poly[0];
      pair<int, int> tangent = convexPolygonPointTangent(poly, p);
      checkSum = 31 * checkSum + tangent.first;
      checkSum = 31 * checkSum + tangent.second;
      if (N == 1) {
        assert(tangent.first == tangent.second && tangent.first == 0);
      } else if (N == 2) {
        if (tangent.first == tangent.second) assert(ccw(poly[0], poly[1], p) == 0);
      } else assert(tangent.first != tangent.second);
      for (int j = tangent.first; j != tangent.second; j = (j + 1) % N) assert(ccw(poly[j], poly[mod(j + 1, N)], p) < 0);
      bool isFirst = true;
      for (int j = tangent.second; isFirst || j != tangent.first; j = (j + 1) % N) {
        isFirst = false;
        assert(ccw(poly[j], poly[mod(j + 1, N)], p) >= 0);
      }
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 1 Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  test1();
  cout << "Test Passed" << endl;
  return 0;
}
