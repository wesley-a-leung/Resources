public class ConvexHullTrick {
    private long[] M, B;
    private int front, back;

    public ConvexHullTrick(int N) {
        M = new long[N];
        B = new long[N];
        front = back = 0;
    }

    public void addLine(long m, long b) {
        while (back >= 2 && (B[back - 2] - B[back - 1]) * (m - M[back - 1]) >= (B[back - 1] - b) * (M[back - 1] - M[back - 2])) back--;
        M[back] = m;
        B[back++] = b;
    }

    public long getMax(long x) {
        while (front < back - 1 && M[front + 1] * x + B[front + 1] >= M[front] * x + B[front]) front++;
        return M[front] * x + B[front];
    }
}
