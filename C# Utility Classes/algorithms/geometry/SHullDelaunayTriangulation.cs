using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using static System.Diagnostics.Debug;

namespace Algorithms.Geometry {
    public class DelaunayTriangulation {
        public static double EPS = 1e-9;

        public class Point {
            public double x, y;

            protected Point() { }

            public Point(double x, double y) {
                this.x = x;
                this.y = y;
            }

            public double distSqTo(Point other) {
                double dx = x - other.x, dy = y - other.y;
                return dx * dx + dy * dy;
            }

            public override string ToString() {
                return string.Format("({0:F4}, {1:F4})", x, y);
            }
        }


        public class Triad {
            public int a, b, c, ab, bc, ac;
            public double circumcircleR2, circumcircleX,circumcircleY;

            public Triad(int x, int y, int z) {
                a = x;
                b = y;
                c = z;
                circumcircleR2 = ab = bc = ac = -1;
            }

            public void init(int a, int b, int c, int ab, int bc, int ac, List<Point> points) {
                this.a = a;
                this.b = b;
                this.c = c;
                this.ab = ab;
                this.bc = bc;
                this.ac = ac;
                findCircumcircle(points);
            }

            public void makeClockwise(List<Point> points) {
                double centroidX = (points[a].x + points[b].x + points[c].x) / 3.0;
                double centroidY = (points[a].y + points[b].y + points[c].y) / 3.0;
                double dxa = points[a].x - centroidX;
                double dya = points[a].y - centroidY;
                double dxba = points[b].x - points[a].x;
                double dyba = points[b].y - points[a].y;
                double df = dyba * dxa - dxba * dya;
                if (df > 0) {
                    int t = b;
                    b = c;
                    c = t;
                    t = ab;
                    ab = ac;
                    ac = t;
                }
            }

            public bool findCircumcircle(List<Point> points) {
                double dxba = points[b].x - points[a].x;
                double dyba = points[b].y - points[a].y;
                double dxca = points[c].x - points[a].x;
                double dyca = points[c].y - points[a].y;
                double absq = dxba * dxba + dyba * dyba;
                double acsq = dxca * dxca + dyca * dyca;
                double D = dxba * dyca - dyba * dxca;
                if (Math.Abs(D) <= EPS) {
                    circumcircleX = circumcircleY = 0;
                    circumcircleR2 = -1;
                    return false;
                }
                double offX = 0.5 * (dyca * absq - dyba * acsq) / D;
                double offY = 0.5 * (dxba * acsq - dxca * absq) / D;
                circumcircleR2 = offX * offX + offY * offY;
                circumcircleX = points[a].x + offX;
                circumcircleY = points[a].y + offY;
                return true;
            }

            public bool insideCircumcircle(Point p) {
                double dx = circumcircleX - p.x, dy = circumcircleY - p.y;
                return dx * dx + dy * dy <= circumcircleR2 + EPS;
            }

            public void changeAdjInd(int oldInd, int newInd) {
                if (ab == oldInd) ab = newInd;
                else if (bc == oldInd) bc = newInd;
                else if (ac == oldInd) ac = newInd;
                else Assert(false);
            }

            public void findAdj(int pInd, int tInd, out int opp, out int l, out int r) {
                if (ab == tInd) {
                    opp = c;
                    if (a == pInd) {
                        l = ac;
                        r = bc;
                    } else {
                        l = bc;
                        r = ac;
                    }
                } else if (ac == tInd) {
                    opp = b;
                    if (a == pInd) {
                        l = ab;
                        r = bc;
                    } else {
                        l = bc;
                        r = ab;
                    }
                } else if (bc == tInd) {
                    opp = a;
                    if (b == pInd) {
                        l = ab;
                        r = ac;
                    } else {
                        l = ac;
                        r = ab;
                    }
                } else {
                    opp = l = r = -1;
                    Assert(false);
                }
            }
        }

        public class HullPoint : Point {
            public int pInd;
            public int tInd;

            public HullPoint(List<Point> points, int pInd) {
                x = points[pInd].x;
                y = points[pInd].y;
                this.pInd = pInd;
                tInd = 0;
            }
        }

        public class Hull : List<HullPoint> {
            private void next(int ind, out double dx, out double dy) {
                Point p = this[ind], q = this[(ind + 1) % Count];
                dx = q.x - p.x;
                dy = q.y - p.y;
            }

            public bool edgeVisibleFrom(int ind, double dx, double dy) {
                double dxi, dyi;
                next(ind, out dxi, out dyi);
                return dx * dyi - dy * dxi <= EPS;
            }

            public bool edgeVisibleFrom(int ind, Point p) {
                double dxi, dyi;
                next(ind, out dxi, out dyi);
                double dx = p.x - this[ind].x;
                double dy = p.y - this[ind].y;
                return dx * dyi - dy * dxi <= EPS;
            }
        }

        private void solve(List<Point> points, Hull hull, List<Triad> triads, bool hullOnly) {
            int N = points.Count;
            double[] distSqCentre = new double[N];
            int[] sortedInd = new int[N];
            for (int i = 0; i < N; i++) {
                distSqCentre[i] = points[0].distSqTo(points[i]);
                sortedInd[i] = i;
            }
            Array.Sort(distSqCentre, sortedInd);
            int mid = -1;
            double rmin2 = double.PositiveInfinity, circumcentreX = 0, circumcentreY = 0;
            Triad tri = new Triad(sortedInd[0], sortedInd[1], sortedInd[2]);
            for (int i = 2; i < N; i++) {
                tri.c = sortedInd[i];
                if (tri.findCircumcircle(points) && tri.circumcircleR2 < rmin2) {
                    mid = i;
                    rmin2 = tri.circumcircleR2;
                    circumcentreX = tri.circumcircleX;
                    circumcentreY = tri.circumcircleY;
                } else if (rmin2 * 4 < distSqCentre[i]) break;
            }
            if (mid != 2) {
                int midInd = sortedInd[mid];
                double distSqMid = distSqCentre[mid];
                Array.Copy(sortedInd, 2, sortedInd, 3, mid - 2);
                Array.Copy(distSqCentre, 2, distSqCentre, 3, mid - 2);
                sortedInd[2] = midInd;
                distSqCentre[2] = distSqMid;
            }
            tri.c = sortedInd[2];
            tri.makeClockwise(points);
            tri.findCircumcircle(points);
            triads.Add(tri);
            hull.Add(new HullPoint(points, tri.a));
            hull.Add(new HullPoint(points, tri.b));
            hull.Add(new HullPoint(points, tri.c));
            Point centre = new Point(circumcentreX, circumcentreY);
            for (int i = 3; i < N; i++) distSqCentre[i] = points[sortedInd[i]].distSqTo(centre);
            Array.Sort(distSqCentre, sortedInd, 3, N - 3);
            int T = 0;
            for (int i = 3; i < N; i++) {
                int pInd = sortedInd[i];
                HullPoint p = new HullPoint(points, pInd);
                double dx = p.x - hull[0].x, dy = p.y - hull[0].y;
                int H = hull.Count, hInd = 0;
                List<int> pInds = new List<int>(), tInds = new List<int>();
                if (hull.edgeVisibleFrom(0, dx, dy)) {
                    hInd = 0;
                    if (hull.edgeVisibleFrom(H - 1, dx, dy)) {
                        pInds.Add(hull[H - 1].pInd);
                        tInds.Add(hull[H - 1].tInd);
                        for (int h = 0; h < H - 1; h++) {
                            pInds.Add(hull[h].pInd);
                            tInds.Add(hull[h].tInd);
                            if (hull.edgeVisibleFrom(h, p)) {
                                hull.RemoveAt(h);
                                h--;
                                H--;
                            } else {
                                hull.Insert(0, p);
                                H++;
                                break;
                            }
                        }
                        for (int h = H - 2; h > 0; h--) {
                            if (hull.edgeVisibleFrom(h, p)) {
                                pInds.Insert(0, hull[h].pInd);
                                tInds.Insert(0, hull[h].tInd);
                                hull.RemoveAt(h + 1);
                            } else break;
                        }
                    } else {
                        hInd = 1;
                        pInds.Add(hull[0].pInd);
                        tInds.Add(hull[0].tInd);
                        for (int h = 1; h < H; h++) {
                            pInds.Add(hull[h].pInd);
                            tInds.Add(hull[h].tInd);
                            if (hull.edgeVisibleFrom(h, p)) {
                                hull.RemoveAt(h);
                                h--;
                                H--;
                            } else {
                                hull.Insert(h, p);
                                break;
                            }
                        }
                    }
                } else {
                    int e1 = -1, e2 = H;
                    for (int h = 1; h < H; h++) {
                        if (hull.edgeVisibleFrom(h, p)) {
                            if (e1 < 0) e1 = h;
                        } else {
                            if (e1 > 0) {
                                e2 = h;
                                break;
                            }
                        }
                    }
                    if (e2 < H) {
                        for (int e = e1; e <= e2; e++) {
                            pInds.Add(hull[e].pInd);
                            tInds.Add(hull[e].tInd);
                        }
                    } else {
                        for (int e = e1; e < e2; e++) {
                            pInds.Add(hull[e].pInd);
                            tInds.Add(hull[e].tInd);
                        }
                        pInds.Add(hull[0].pInd);
                    }
                    if (e1 < e2 - 1) hull.RemoveRange(e1 + 1, e2 - e1 - 1);
                    hull.Insert(e1 + 1, p);
                    hInd = e1 + 1;
                }
                if (hullOnly) continue;
                int a = pInd, T0;
                int P = pInds.Count();
                T = T0 = triads.Count();
                for (int j = 0; j < P - 1; j++) {
                    Triad t = new Triad(a, pInds[j], pInds[j + 1]);
                    t.findCircumcircle(points);
                    t.bc = tInds[j];
                    if (j > 0) t.ab = T - 1;
                    t.ac = T + 1;
                    Triad u = triads[tInds[j]];
                    if ((t.b == u.a && t.c == u.b) || (t.b == u.b && t.c == u.a)) u.ab = T;
                    else if ((t.b == u.a && t.c == u.c) || (t.b == u.c && t.c == u.a)) u.ac = T;
                    else if ((t.b == u.b && t.c == u.c) || (t.b == u.c && t.c == u.b)) u.bc = T;
                    triads.Add(t);
                    T++;
                }
                triads[T - 1].ac = -1;
                hull[hInd].tInd = T - 1;
                if (hInd > 0) hull[hInd - 1].tInd = T0;
                else {
                    H = hull.Count;
                    hull[hull.Count - 1].tInd = T0;
                }
            }
        }

        private bool flipTriangle(List<Point> points, List<Triad> triads, int testInd, out int flipInd) {
            int opp = 0, e1, e2, e3 = 0, e4 = 0;
            flipInd = 0;
            Triad t = triads[testInd];
            if (t.bc >= 0) {
                flipInd = t.bc;
                Triad t2 = triads[flipInd];
                t2.findAdj(t.b, testInd, out opp, out e3, out e4);
                if (t.insideCircumcircle(points[opp])) {
                    e1 = t.ab;
                    e2 = t.ac;
                    if (e1 != e3 && e2 != e4) {
                        int ta = t.a, tb = t.b, tc = t.c;
                        t.init(ta, tb, opp, e1, e3, flipInd, points);
                        t2.init(ta, tc, opp, e2, e4, testInd, points);
                        if (e3 >= 0) triads[e3].changeAdjInd(flipInd, testInd);
                        if (e2 >= 0) triads[e2].changeAdjInd(testInd, flipInd);
                        return true;
                    }
                }
            }
            if (t.ab >= 0) {
                flipInd = t.ab;
                Triad t2 = triads[flipInd];
                t2.findAdj(t.a, testInd, out opp, out e3, out e4);
                if (t.insideCircumcircle(points[opp])) {
                    e1 = t.ac;
                    e2 = t.bc;
                    if (e1 != e3 && e2 != e4) {
                        int ta = t.a, tb = t.b, tc = t.c;
                        t.init(tc, ta, opp, e1, e3, flipInd, points);
                        t2.init(tc, tb, opp, e2, e4, testInd, points);
                        if (e3 >= 0) triads[e3].changeAdjInd(flipInd, testInd);
                        if (e2 >= 0) triads[e2].changeAdjInd(testInd, flipInd);
                        return true;
                    }
                }
            }
            if (t.ac >= 0) {
                flipInd = t.ac;
                Triad t2 = triads[flipInd];
                t2.findAdj(t.a, testInd, out opp, out e3, out e4);
                if (t.insideCircumcircle(points[opp])) {
                    e1 = t.ab;
                    e2 = t.bc;
                    if (e1 != e3 && e2 != e4) {
                        int ta = t.a, tb = t.b, tc = t.c;
                        t.init(tb, ta, opp, e1, e3, flipInd, points);
                        t2.init(tb, tc, opp, e2, e4, testInd, points);
                        if (e3 >= 0) triads[e3].changeAdjInd(flipInd, testInd);
                        if (e2 >= 0) triads[e2].changeAdjInd(testInd, flipInd);
                        return true;
                    }
                }
            }
            return false;
        }

        private int flipTriangles(List<Point> points, List<Triad> triads, HashSet<int> indFlipped) {
            int T = triads.Count;
            indFlipped.Clear();
            int flipped = 0;
            for (int i = 0; i < T; i++) {
                int j;
                if (flipTriangle(points, triads, i, out j)) {
                    flipped += 2;
                    indFlipped.Add(i);
                    indFlipped.Add(j);
                }
            }
            return flipped;
        }

        private int flipTriangles(List<Point> points, List<Triad> triads, HashSet<int> indTest, HashSet<int> indFlipped) {
            indFlipped.Clear();
            int flipped = 0;
            foreach (int i in indTest) {
                int j;
                if (flipTriangle(points, triads, i, out j)) {
                    flipped += 2;
                    indFlipped.Add(i);
                    indFlipped.Add(j);
                }
            }
            return flipped;
        }

        private List<Triad> triads;

        public DelaunayTriangulation(List<Point> points) {
            triads = new List<Triad>();
            Hull hull = new Hull();
            solve(points, hull, triads, false);
            int T = triads.Count;
            HashSet<int> setA = new HashSet<int>(), setB = new HashSet<int>();
            int flipped = flipTriangles(points, triads, setA);
            int iterations = 1;
            while (flipped > 0 && iterations < 20) { // shouldn't need more than log(n) steps
                if (iterations % 2 == 1) flipped = flipTriangles(points, triads, setA, setB);
                else flipped = flipTriangles(points, triads, setB, setA);
                iterations++;
            }
        }

        public List<Triad> getTriads() {
            return triads;
        }
    }
}
