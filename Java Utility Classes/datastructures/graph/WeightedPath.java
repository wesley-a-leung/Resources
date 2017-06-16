package datastructures.graph;

import java.util.ArrayList;
import java.util.Iterator;

public class WeightedPath implements Comparable<WeightedPath>, Iterable<WeightedEdge>{
    private ArrayList<WeightedEdge> list;
    private double length;
    private double longest;
    private double shortest;
    
    public WeightedPath() {
        list = new ArrayList<WeightedEdge>();
        length = 0;
        longest = 0;
        shortest = Double.MAX_VALUE;
    }
    
    public WeightedPath(Iterable<WeightedEdge> iter) {
        list = new ArrayList<WeightedEdge>();
        length = 0;
        longest = 0;
        shortest = Double.MAX_VALUE;
        if (iter == null) return;
        for (WeightedEdge e: iter) {
            list.add(e);
            length += e.weight();
            shortest = Double.min(shortest, e.weight());
            longest = Double.max(longest, e.weight());
        }
    }
    
    public double length() {
        return length;
    }
    
    public int size() {
        return list.size();
    }
    
    public double longest() {
        return longest;
    }
    
    public double shortest() {
        return shortest;
    }
    
    public WeightedEdge get(int i) {
        if (i < 0 || i >= list.size()) throw new IndexOutOfBoundsException("Index is not between 0 and " + (list.size()-1));
        return list.get(i);
    }
    
    public void add(WeightedEdge e) {
        list.add(e);
        length += e.weight();
        shortest = Double.min(shortest, e.weight());
        longest = Double.max(longest, e.weight());
    }
    
    public void addAll(Iterable<WeightedEdge> iter) {
        if (iter == null) return;
        for (WeightedEdge e: iter) {
            list.add(e);
            length += e.weight();
            shortest = Double.min(shortest, e.weight());
            longest = Double.max(longest, e.weight());
        }
    }
    
    public void addAll(WeightedPath path) {
        if (path == null) return;
        for (WeightedEdge e: path) {
            list.add(e);
            length += e.weight();
            shortest = Double.min(shortest, e.weight());
            longest = Double.max(longest, e.weight());
        }
    }

    @Override
    public int compareTo(WeightedPath p) {
        if (length > p.length) return 1;
        else if (length < p.length) return -1;
        if (shortest > p.shortest) return 1;
        else if (shortest < p.shortest) return -1;
        return 0;
    }

    @Override
    public Iterator<WeightedEdge> iterator() {
        return list.iterator();
    }
    
    @Override
    public boolean equals(Object o){
        if (o == this) return true;
        if (!(o instanceof WeightedPath)) {
            return false;
        }
        WeightedPath p = (WeightedPath) o;
        return list.equals(p.list);
    }
}
