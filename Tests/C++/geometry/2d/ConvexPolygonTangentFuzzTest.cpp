#include <bits/stdc++.h>
#include "../../../../Content/C++/geometry/2d/Line.h"
#include "../../../../Content/C++/geometry/2d/Angle.h"
#include "../../../../Content/C++/geometry/2d/Circle.h"
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
      for (int j = 0; j < N; j++) {
        assert(l1.onLeft(poly[j]) < 0 || (l1.onLeft(poly[j]) == 0 && le(distSq(p, poly[tangent.first]), distSq(p, poly[j]))));
        assert(l2.onLeft(poly[j]) > 0 || (l2.onLeft(poly[j]) == 0 && le(distSq(p, poly[tangent.second]), distSq(p, poly[j]))));
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
  const int TESTCASES = 5e3;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 10 + 1;
    vector<pt> poly = generateConvexPolygon(N, rng);
    int Q = 100;
    uniform_real_distribution<T> dis(-10, 10), dis2(0, 10);
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
      T r;
      int iter = 0;
      auto good = [&] {
        if (iter++ >= 100000) {
          do {
            p = pt(dis(rng), dis(rng));
          } while (isInConvexPolygon(poly, p) <= 0);
          iter = 0;
        }
        if (gt(polygonCircleIntersectionArea(poly, Circle(p, r)), 0)) return false;
        for (int j = 0; j < N; j++) if (Circle(p, r).contains(poly[j]) <= 0) return false;
        if (N > 1) for (int j = 0; j < N; j++) {
          vector<pt> inter = circleLineIntersection(Circle(p, r), Line(poly[j], poly[mod(j + 1, N)]));
          if (!inter.empty() && !segSegIntersection(poly[j], poly[mod(j + 1, N)], inter[0], inter.back()).empty()) return false;
        }
        return true;
      };
      do {
        r = dis2(rng);
      } while (!good());
      if (rng() % 10 == 0) r = 0;
      bool inner = rng() % 2;
      Circle c(p, r);
      pair<int, int> tangent = convexPolygonCircleTangent(poly, c, inner);
      assert(0 <= tangent.first && tangent.first < N);
      assert(0 <= tangent.second && tangent.second < N);
      checkSum = 31 * checkSum + tangent.first;
      checkSum = 31 * checkSum + tangent.second;
      if (N == 1) {
        assert(tangent.first == tangent.second);
      }
      if (r == 0) {
        assert(tangent == convexPolygonPointTangent(poly, p));
      } else {
        vector<pair<pt, pt>> t1, t2;
        circleCircleTangent(Circle(poly[tangent.first], 0), c, inner, t1);
        circleCircleTangent(Circle(poly[tangent.second], 0), c, inner, t2);
        pt a = t1[0].second, b = t2[1].second;
        Line l1(a, poly[tangent.first]), l2(b, poly[tangent.second]);
        if (inner) {
          assert(eq(circleHalfPlaneIntersectionArea(c, Line(-l1.v, -l1.c)), 0));
          assert(eq(circleHalfPlaneIntersectionArea(c, l2), 0));
        } else {
          assert(eq(circleHalfPlaneIntersectionArea(c, l1), 0));
          assert(eq(circleHalfPlaneIntersectionArea(c, Line(-l2.v, -l2.c)), 0));
        }
        for (int j = 0; j < N; j++) {
          assert(l1.onLeft(poly[j]) < 0 || (l1.onLeft(poly[j]) == 0 && le(distSq(a, poly[tangent.first]), distSq(a, poly[j]))));
          assert(l2.onLeft(poly[j]) > 0 || (l2.onLeft(poly[j]) == 0 && le(distSq(b, poly[tangent.second]), distSq(b, poly[j]))));
        }
      }
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 2 (Convex Polygon Circle Tangent) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test3() {
  const auto start_time = chrono::system_clock::now();
  mt19937_64 rng(0);
  const int TESTCASES = 1e5;
  long long checkSum = 0;
  for (int ti = 0; ti < TESTCASES; ti++) {
    int N = rng() % 10 + 1, M = rng() % 10 + 1;
    bool inner = rng() % 2;
    vector<pt> poly1 = generateConvexPolygon(N, rng), poly2 = generateConvexPolygon(M, rng);
    pt add = rng() % 2 ? pt(1, 0) : pt(0, 1);
    for (auto &&p : poly2) p += add;
    if (rng() % 2) {
      swap(N, M);
      swap(poly1, poly2);
    }
    vector<pair<int, int>> tangent = convexPolygonConvexPolygonTangent(poly1, poly2, inner);
    assert(0 <= tangent[0].first && tangent[0].first < N);
    assert(0 <= tangent[1].first && tangent[1].first < N);
    assert(0 <= tangent[0].second && tangent[0].second < M);
    assert(0 <= tangent[1].second && tangent[1].second < M);
    checkSum = 31 * checkSum + tangent[0].first;
    checkSum = 31 * checkSum + tangent[0].second;
    checkSum = 31 * checkSum + tangent[1].first;
    checkSum = 31 * checkSum + tangent[1].second;
    pt a = poly1[tangent[0].first], b = poly1[tangent[1].first], c = poly2[tangent[0].second], d = poly2[tangent[1].second];
    Line l1(a, c), l2(b, d);
    for (int i = 0; i < N; i++) {
      assert(l1.onLeft(poly1[i]) > 0 || (l1.onLeft(poly1[i]) == 0 && le(distSq(c, a), distSq(c, poly1[i]))));
      assert(l2.onLeft(poly1[i]) < 0 || (l2.onLeft(poly1[i]) == 0 && le(distSq(d, b), distSq(d, poly1[i]))));
    }
    for (int i = 0; i < M; i++) {
      assert(l1.onLeft(poly2[i]) == (inner ? -1 : 1) || (l1.onLeft(poly2[i]) == 0 && le(distSq(a, c), distSq(a, poly2[i]))));
      assert(l2.onLeft(poly2[i]) == (inner ? 1 : -1) || (l2.onLeft(poly2[i]) == 0 && le(distSq(b, d), distSq(b, poly2[i]))));
    }
  }
  const auto end_time = chrono::system_clock::now();
  double sec = ((end_time - start_time).count() / double(chrono::system_clock::period::den));
  cout << "Subtest 3 (Convex Polygon Convex Polygon Tangent) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

void test4() {
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
  cout << "Subtest 4 (Closest Point on Convex Polygon) Passed" << endl;
  cout << "  Time: " << fixed << setprecision(3) << sec << "s" << endl;
  cout << "  Checksum: " << checkSum << endl;
}

int main() {
  // test1();
  // test2();
  // test3();
  test4();
  cout << "Test Passed" << endl;
  return 0;
}
