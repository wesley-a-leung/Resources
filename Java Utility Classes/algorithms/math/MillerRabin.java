package algorithms.math;

import java.util.Random;

public class MillerRabin {
    public static boolean isPrime(long n, int iterations) {
        if (n < 2 || (n != 2 && n % 2 == 0)) return false;
        long s = n - 1;
        while (s % 2 == 0) s /= 2;
        Random rand = new Random();
        for (int i = 0; i < iterations; i++) {
            long temp = s;
            long r = powMod(Math.abs(rand.nextLong()) % (n - 1) + 1, temp, n);
            while (temp != n - 1 && r != 1 && r != n - 1) {
                r = mulMod(r, r, n);
                temp *= 2;
            }
            if (r != n - 1 && temp % 2 == 0) return false;
        }
        return true;
    }
    
    public static long powMod(long base, long pow, long mod) {
        long x = 1, y = base;
        for (; pow > 0; pow /= 2) {
            if (pow % 2 == 1) x = mulMod(x, y, mod);
            y = mulMod(y, y, mod);
        }
        return x % mod;
    }
    
    public static long mulMod(long a, long b, long mod) {
        long x = 0, y = a % mod;
        for (; b > 0; b /= 2) {
            if (b % 2 == 1) x = (x + y) % mod;
            y = (y * 2) % mod;
        }
        return x % mod;
    }
}
