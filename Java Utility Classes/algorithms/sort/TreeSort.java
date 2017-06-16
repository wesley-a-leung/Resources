package algorithms.sort;

import java.util.Map;
import java.util.TreeMap;

/**
 * The {@code TreeSort} class provides a static methods for treesorting
 * an array using TreeMaps to store frequencies.
 * 
 * @author Wesley Leung
 *
 */
public class TreeSort {
    
    // This class should not be instantiated.
    private TreeSort() { }

    /**
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        TreeMap<Comparable, Integer> tree = new TreeMap<Comparable, Integer>();
        for (Comparable x: a) {
            Integer count = tree.get(x);
            if (count == null) tree.put(x, 1);
            else tree.put(x, count + 1);
        }
        int i = 0;
        for (Map.Entry<Comparable, Integer> x: tree.entrySet()) {
            for (int j = 0; j < x.getValue(); j++) {
                a[i++] = x.getKey();
            }
        }
    }
}