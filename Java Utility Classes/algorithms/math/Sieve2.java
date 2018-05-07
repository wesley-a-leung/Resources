package algorithms.math;

import java.util.ArrayList;

public class Sieve2 {
    public static boolean[] isPrime;
    public static ArrayList<Integer> primes;
    public static int[] SPF;
    
    public static void sieve(int N) {
        isPrime = new boolean[N + 1];
        for (int i = 0; i <= N; i++) isPrime[i] = true;
        primes = new ArrayList<Integer>();
        SPF = new int[N + 1];
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; i <= N; i++) {
            if (isPrime[i]) {
                primes.add(i);
                SPF[i] = i;
            }
            for (int j = 0; j < (int) primes.size() && i * primes.get(j) <= N && primes.get(j) <= SPF[i]; j++) {
                isPrime[i * primes.get(j)] = false;
                SPF[i * primes.get(j)] = primes.get(j);
            }
        }
    }
}
