package algorithms.math;

public class MaximumSubarraySum {
    private int max;
    private int start;
    private int end;
    
    public MaximumSubarraySum(int[] a) {
        max = 0;
        start = 0;
        end = 0;
        int currMax = 0;
        int currStart = 0;
        for (int i = 0; i < a.length; i++) {
            currMax += a[i];
            if (currMax < 0) {
                currMax = 0;
                currStart = i + 1;
            }
            if (max < currMax) {
                max = currMax;
                start = currStart;
                end = i;
            }
        }
    }
    
    public int max() {
        return this.max;
    }
    
    public int start() {
        return this.start;
    }
    
    public int end() {
        return this.end;
    }
}
