package algorithms.math;

/**
 * Approximates the a root of a function, given its derivative.
 * 
 * @author Wesley Leung
 */
public class Newton {
    public static double EPS = 1e-9;

    interface Function {
        public abstract double eval(double x);
    }

    // Finds a root of function f with derivative df based on initial guess x0
    public static double newton(Function f, Function df, double x0) {
        double cur = x0, next = x0;
        do {
            cur = next;
            next = cur - f.eval(cur) / df.eval(cur);
        } while (Math.abs(next - cur) > EPS);
        return next;
    }

    // Finds a root of function f with derivative df based on initial guess x0 by
    // repeating newton's approximation a specified number of times
    public static double newton(Function f, Function df, double x0, int iterations) {
        double cur = x0;
        while (iterations--) cur = cur - f(cur) / df(cur);
        return cur;
    }
}
