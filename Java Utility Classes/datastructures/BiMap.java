package datastructures;

import java.util.HashMap;
import java.util.Set;

public class BiMap<First extends Comparable<First>, Second extends Comparable<Second>>  {
    private HashMap<First, Second> one;
    private HashMap<Second, First> two;
    public BiMap() {
        one = new HashMap<First, Second>();
        two = new HashMap<Second, First>();
    }
    
    public void clear() {
        one.clear();
        two.clear();
    }
    
    public boolean containsFirst(First key) {
        return one.containsKey(key);
    }
    
    public boolean containsSecond(Second key) {
        return two.containsKey(key);
    }
    
    public First getFirst(Second key) {
        return two.get(key);
    }
    
    public Second getSecond(First key) {
        return one.get(key);
    }
    
    public Set<First> firstKeySet() {
        return one.keySet();
    }
    
    public Set<Second> secondKeySet() {
        return two.keySet();
    }
    
    public void put(First first, Second second) {
        if (one.containsKey(first)) {
            two.remove(one.remove(first));
        } else if (two.containsKey(second)) {
            one.remove(two.remove(second));
        }
        one.put(first, second);
        two.put(second, first);
    }
    
    public boolean remove(Object o) {
        First check1 = null;
        try {
            check1 = two.remove(one.remove(o));
        } catch (Exception e1) {
            Second check2 = null;
            try {
                check2 = one.remove(two.remove(o));
            } catch (Exception e2) {
                return false;
            }
            if (check2 != null) return true;
        }
        if (check1 != null) return true;
        return false;
    }
    
    public int size() {
        return one.size();
    }
}
