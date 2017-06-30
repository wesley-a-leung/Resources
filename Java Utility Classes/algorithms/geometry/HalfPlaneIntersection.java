package algorithms.geometry;

import java.util.Arrays;

import datastructures.geometry.Point2D;
import datastructures.geometry.Vector;

public class HalfPlaneIntersection {
    private static final double EPS = 1e-12;
    private int N;
    private Line[] all;
    private Line[] L;
    private int count;
    
    private static class Line implements Comparable<Line> {
        public Vector a, b;
        public double rad;
        public Line(Vector a, Vector b) {
            this.a = a;
            this.b = b;
            this.rad = b.angle();
        }
        
        public boolean onLeft(Vector v) {
            return this.b.cross2D(v.minus(this.a)) > EPS;
        }
        
        @Override
        public int compareTo(Line l) {
            if (rad < l.rad) return -1;
            else if (rad > l.rad) return 1;
            return 0;
        }
    }
    
    public HalfPlaneIntersection(Point2D[] low, Point2D[] high) {
        if (low.length != high.length) throw new IllegalArgumentException("Array lengths must be the same");
        N = low.length;
        all = new Line[N * 2];
        for (int i = 0; i < N; i++) {
            all[i * 2] = new Line(new Vector(0, high[i].y() / high[i].x()), new Vector(-1.0 / high[i].x(), 1.0));
            all[i * 2 + 1] = new Line(new Vector(0, low[i].y() / low[i].x()), new Vector(1.0 / low[i].x(), -1.0));
        }
        int l = 1;
        int r = N;
        count = 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (check(mid)) {
                count = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
    }
    
    public int countIntersections() {
        return count;
    }
    
    private boolean check(int n) {
        L = new Line[n * 2];
        for (int i = 0; i < n * 2; i++) {
            L[i] = all[i];
        }
        return halfPlaneIntersection(n * 2);
    }
    
    private Vector getLineIntersection(Line la, Line lb) {
        double t = (lb.b.cross2D(la.a.minus(lb.a))) / la.b.cross2D(lb.b);
        return la.a.plus(la.b.scale(t));
    }
    
    private boolean halfPlaneIntersection(int n) {
        Vector[] v = new Vector[n];
        Line[] q = new Line[n];
        Arrays.sort(L);
        int front = 0;
        int back = 0;
        q[0] = L[0];
        for (int i = 1; i < n; i++) {
            while (front < back && !L[i].onLeft(v[back - 1])) back--;
            while (front < back && !L[i].onLeft(v[front])) front++;
            if (Math.abs(q[back].b.cross2D(L[i].b)) < EPS) {
                if (q[back].onLeft(L[i].a)) q[back] = L[i];
            } else {
                q[++back] = L[i];
            }
            if (front < back) v[back - 1] = getLineIntersection(q[back - 1], q[back]);
        }
        while (front < back && !q[front].onLeft(v[back - 1])) back--;
        return (back - front) > 1;
    }
}
