package algorithms.math;

import java.util.ArrayList;

public class Sieve {
    public static ArrayList<Integer> primes(int start, int end) {
        if (start > 1) start -= 1;
        int sqrtE = (int) Math.sqrt(end);
        int range = end - start;
        boolean[] sieve1 = new boolean[sqrtE];
        ArrayList<Integer> list = new ArrayList<Integer>();
        sieve1[0] = true;
        for (int x = 1; x < sqrtE; x++) {
            if (sieve1[x] == false) {
                list.add(x + 1);
                for (int y = (x + 1) * (x + 1) - 1; y < sqrtE; y += (x + 1)) {
                    sieve1[y] = true;
                }
            }
        }
        boolean[] sieve2 = new boolean[range];
        for (int x : list) {
            for (int y = x * x - 1; y < end; y += x) {
                if (y < start) continue;
                sieve2[y - start] = true;
            }
        }
        ArrayList<Integer> primes = new ArrayList<Integer>();
        for (int x = 0; x < range; x++) {
            if (sieve2[x] == false) primes.add(start + x + 1);
        }
        return primes;
    }
}
