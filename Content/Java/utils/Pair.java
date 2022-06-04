public class Pair<Item1 extends Comparable<Item1>, Item2 extends Comparable<Item2>> implements Comparable<Pair<Item1, Item2>> {
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
    return p.first.equals(first) && p.second.equals(second);
  }
  @Override
  public int compareTo(Pair<Item1, Item2> p) {
    int o = first.compareTo(p.first);
    return o == 0 ? second.compareTo(p.second) : o;
  }
}
