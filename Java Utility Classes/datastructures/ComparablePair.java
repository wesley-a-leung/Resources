package datastructures;

public class ComparablePair<Item extends Comparable<Item>, Item2 extends Comparable<Item2>> implements Comparable<ComparablePair<Item, Item2>> {
    public Item first;
    public Item2 second;

    public ComparablePair(Item first, Item2 second) {
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
        if (!(o instanceof ComparablePair)) return false;
        ComparablePair p = (ComparablePair) o;
        return p.first.equals(first) && p.second.equals(second);
    }

    @Override
    public int compareTo(ComparablePair<Item, Item2> p) {
        if (first.compareTo(p.first) != 0) return first.compareTo(p.first);
        return (second.compareTo(p.second));
    }
}
