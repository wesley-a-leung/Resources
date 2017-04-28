package datastructures.graph;

import java.util.ArrayList;
import java.util.Iterator;

public class DirectedWeightedPath implements Comparable<DirectedWeightedPath>, Iterable<DirectedWeightedEdge>{
	private ArrayList<DirectedWeightedEdge> list;
	private double length;
	private double longest;
	private double shortest;
	
	public DirectedWeightedPath() {
		list = new ArrayList<DirectedWeightedEdge>();
		length = 0;
		longest = 0;
		shortest = Double.MAX_VALUE;
	}
	
	public DirectedWeightedPath(Iterable<DirectedWeightedEdge> iter) {
		list = new ArrayList<DirectedWeightedEdge>();
		length = 0;
		longest = 0;
		shortest = Double.MAX_VALUE;
		if (iter == null) return;
		for (DirectedWeightedEdge e: iter) {
			list.add(e);
			length += e.weight();
			shortest = Double.min(shortest, e.weight());
			longest = Double.max(longest, e.weight());
		}
	}
	
	public double length() {
		if (list == null) return -1;
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
	
	public DirectedWeightedEdge get(int i) {
		if (i < 0 || i >= list.size()) throw new IndexOutOfBoundsException("Index is not between 0 and " + (list.size()-1));
		return list.get(i);
	}
	
	public void add(DirectedWeightedEdge e) {
		list.add(e);
		length += e.weight();
		shortest = Double.min(shortest, e.weight());
		longest = Double.max(longest, e.weight());
	}
	
	public void addAll(Iterable<DirectedWeightedEdge> iter) {
		if (iter == null) return;
		for (DirectedWeightedEdge e: iter) {
			list.add(e);
			length += e.weight();
			shortest = Double.min(shortest, e.weight());
			longest = Double.max(longest, e.weight());
		}
	}
	
	public void addAll(DirectedWeightedPath path) {
		if (path == null) return;
		for (DirectedWeightedEdge e: path) {
			list.add(e);
			length += e.weight();
			shortest = Double.min(shortest, e.weight());
			longest = Double.max(longest, e.weight());
		}
	}

	@Override
	public int compareTo(DirectedWeightedPath o) {
		if (length > o.length) {
			return 1;
		} else if (length < o.length) {
			return -1;
		}
		return 0;
	}

	@Override
	public Iterator<DirectedWeightedEdge> iterator() {
		return list.iterator();
	}
	
	@Override
	public boolean equals(Object o){
		if (o == this) return true;
        if (!(o instanceof DirectedWeightedPath)) {
            return false;
        }
        DirectedWeightedPath p = (DirectedWeightedPath) o;
        return list.equals(p.list);
	}
}
