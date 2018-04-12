package datastructures.geometry;

import java.util.Arrays;
import java.util.ArrayDeque;
import java.util.Queue;

public class KdTree {
    private static final boolean VERTICAL = false;
    private static final boolean HORIZONTAL = true;
    
    private final double XMIN, YMIN, XMAX, YMAX;
    
    private static class Node {
        Point2D p;
        Node leftUp, rightDown;
        Rectangle r;
        
        Node(Point2D p, Rectangle r) {
            this.p = p;
            this.r = r;
        }
    }
    
    private int size;
    private Node root;
    
    public KdTree(double xmin, double ymin, double xmax, double ymax) {
        root = null;
        size = 0;
        XMIN = xmin;
        YMIN = ymin;
        XMAX = xmax;
        YMAX = ymax;
    }
    
    public KdTree(double xmin, double ymin, double xmax, double ymax, Point2D[] points) {
        size = points.length;
        XMIN = xmin;
        YMIN = ymin;
        XMAX = xmax;
        YMAX = ymax;
        root = construct(root, points, 0, points.length - 1, VERTICAL, XMIN, YMIN, XMAX, YMAX);
    }
    
    private Node construct(Node n, Point2D[] points, int lo, int hi, boolean partition, double xmin, double ymin, double xmax, double ymax) {
        if (lo > hi) return null;
        if (partition == VERTICAL) Arrays.sort(points, lo, hi + 1, Point2D.X_ORDER);
        else Arrays.sort(points, lo, hi + 1, Point2D.Y_ORDER);
        int mid = lo + (hi - lo) / 2;
        Point2D p = points[mid];
        n = new Node(p, new Rectangle(xmin, ymin, xmax, ymax));
        if (partition == VERTICAL) {
            n.leftUp = construct(n.leftUp, points, lo, mid - 1, !partition, xmin, ymin, n.p.x(), ymax);
            n.rightDown = construct(n.rightDown, points, mid + 1, hi, !partition, n.p.x(), ymin, xmax, ymax);
        } else {
            n.leftUp = construct(n.leftUp, points, lo, mid - 1, !partition, xmin, ymin, xmax, n.p.y());
            n.rightDown = construct(n.rightDown, points, mid + 1, hi, !partition, xmin, n.p.y(), xmax, ymax);
        }
        return n;
    }
    
    public boolean isEmpty() {
        return size == 0;
    }
    
    public int size() {
        return size;
    }
    
    public void insert(Point2D p) {
        if (p == null) throw new IllegalArgumentException("Argument cannot be null.");
        root = insert(root, p, VERTICAL, XMIN, YMIN, XMAX, YMAX);
    }
    
    private Node insert(Node n, Point2D p, boolean partition, double xmin, double ymin, double xmax, double ymax) {
        if (n == null) {
            size++;
            return new Node(p, new Rectangle(xmin, ymin, xmax, ymax));
        }
        if (n.p.equals(p)) return n;
        if (partition == VERTICAL) {
            int cmp = Point2D.X_ORDER.compare(p, n.p);
            if (cmp < 0) n.leftUp = insert(n.leftUp, p, !partition, xmin, ymin, n.p.x(), ymax);
            else n.rightDown = insert(n.rightDown, p, !partition, n.p.x(), ymin, xmax, ymax);
        } else {
            int cmp = Point2D.Y_ORDER.compare(p, n.p);
            if (cmp < 0) n.leftUp = insert(n.leftUp, p, !partition, xmin, ymin, xmax, n.p.y());
            else n.rightDown = insert(n.rightDown, p, !partition, xmin, n.p.y(), xmax, ymax);
        }
        return n;
    }
    
    public boolean contains(Point2D p) {
        if (p == null) throw new IllegalArgumentException("Argument cannot be null.");
        return contains(root, p, VERTICAL);
    }
    
    private boolean contains(Node n, Point2D p, boolean partition) {
        if (n == null) return false;
        if (n.p.equals(p)) return true;
        if (partition == VERTICAL) {
            int cmp = Point2D.X_ORDER.compare(p, n.p);
            if (cmp < 0) return contains(n.leftUp, p, !partition);
            else return contains(n.rightDown, p, !partition);
        } else {
            int cmp = Point2D.Y_ORDER.compare(p, n.p);
            if (cmp < 0) return contains(n.leftUp, p, !partition);
            else return contains(n.rightDown, p, !partition);
        }
    }
    
    public Iterable<Point2D> range(Rectangle rect) {
        if (rect == null) throw new IllegalArgumentException("Argument cannot be null.");
        Queue<Point2D> q = new ArrayDeque<Point2D>();
        range(root, q, rect);
        return q;
    }
    
    private void range(Node n, Queue<Point2D> q, Rectangle rect) {
        if (n == null || !rect.intersects(n.r)) return;
        if (rect.contains(n.p)) q.offer(n.p);
        range(n.leftUp, q, rect);
        range(n.rightDown, q, rect);
    }
    
    public Point2D nearest(Point2D p) {
        if (p == null) throw new IllegalArgumentException("Argument cannot be null.");
        if (isEmpty()) return null;
        return nearest(root, p, null);
    }
    
    private Point2D nearest(Node n, Point2D p, Point2D nearest) {
        if (n == null || (nearest != null && nearest.distanceSquaredTo(p) < n.r.distanceSquaredTo(p))) return nearest;
        if (nearest == null || n.p.distanceSquaredTo(p) < nearest.distanceSquaredTo(p)) nearest = n.p;
        if (n.leftUp != null && n.leftUp.r.contains(p)) {
            nearest = nearest(n.leftUp, p, nearest);
            nearest = nearest(n.rightDown, p, nearest);
        } else {
            nearest = nearest(n.rightDown, p, nearest);
            nearest = nearest(n.leftUp, p, nearest);
        }
        return nearest;
    }
}
