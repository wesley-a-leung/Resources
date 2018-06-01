package datastructures;

public class Pair<Item1, Item2> {
    public Item1 first;
    public Item2 second;

    public Pair(Item1 first, Item2 second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public int hashCode() {
        return 31 * first.hashCode() + second.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof Pair)) return false;
        Pair p = (Pair) o;
        return first.equals(p.first) && second.equals(p.second);
    }
    
    @Override
    public String toString() {
        return "(" + first.toString() + ", " + second.toString() + ")";
    }
}
