package datastructures;

public class Pair<Item, Item2> {
	private Item first;
	private Item2 second;
	
	public Pair(Item first, Item2 second) {
		this.first = first;
		this.second = second;
	}
	
	public Item getFirst() {
		return this.first;
	}

	public void setFirst(Item first) {
		this.first = first;
	}

	public Item2 getSecond() {
		return this.second;
	}

	public void setSecond(Item2 second) {
		this.second = second;
	}
	
	public void set(Item first, Item2 second) {
		this.first = first;
		this.second = second;
	}
	
    @Override
    public int hashCode() {
        return 31 * (first.hashCode() + second.hashCode());
    }
	
    @Override
	public boolean equals(Object o) {
    	if (o == this) return true;
        if (!(o instanceof Pair)) {
            return false;
        }
        Pair p = (Pair) o;
		return p.first.equals(first) && p.second.equals(second);
	}
}
