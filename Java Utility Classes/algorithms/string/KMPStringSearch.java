package algorithms.string;

/**
 *  The {@code KMP} class finds the first occurrence of a pattern string
 *  in a text string.
 *  <p>
 *  This implementation uses a version of the Knuth-Morris-Pratt substring search
 *  algorithm. The version takes time and space proportional to
 *  <em>N</em> + <em>M</em> in the worst case, where <em>N</em> is the length
 *  of the text string and <em>M</em> is the length of the pattern.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/53substring">Section 5.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 */
public class KMPStringSearch {
    private int[] LCP;

    private char[] pattern;    // either the character array for the pattern
    private String pat;        // or the pattern string

    /**
     * Preprocesses the pattern string.
     *
     * @param pat the pattern string
     */
    public KMPStringSearch(String pat) {
        this.pat = pat;
        int m = pat.length();
        LCP = new int[m + 1];
        LCP[0] = -1;
        for (int i = 0, j = -1; i < m; i++, j++, LCP[i] = j) {
            while (j >= 0 && pat.charAt(i) != pat.charAt(j)) {
                j = LCP[j];
            }
        }
    } 

    /**
     * Preprocesses the pattern string.
     *
     * @param pattern the pattern string
     */
    public KMPStringSearch(char[] pattern) {
        this.pattern = pattern;
        int m = pattern.length;
        LCP = new int[m + 1];
        LCP[0] = -1;
        for (int i = 0, j = -1; i < m; i++, j++, LCP[i] = j) {
            while (j >= 0 && pattern[i] != pattern[j]) {
                j = LCP[j];
            }
        }
    } 

    /**
     * Returns the index of the first occurrrence of the pattern string
     * in the text string.
     *
     * @param  txt the text string
     * @return the index of the first occurrence of the pattern string
     *         in the text string; -1 if no such match
     */
    public int search(String txt) {
        int m = pat.length();
        int n = txt.length();
        for (int i = 0, j = 0; i < n; i++, j++) {
            while (j >= 0 && txt.charAt(i) != pat.charAt(j)) {
                j = LCP[j];
            }
            if (j == m - 1) {
                return i - j;
            }
        }
        return -1;
    }

    /**
     * Returns the index of the first occurrence of the pattern string
     * in the text string.
     *
     * @param  text the text string
     * @return the index of the first occurrence of the pattern string
     *         in the text string; -1 if no such match
     */
    public int search(char[] text) {
        int m = pattern.length;
        int n = text.length;
        for (int i = 0, j = 0; i < n; i++, j++) {
            while (j >= 0 && text[i] != pattern[j]) {
                j = LCP[j];
            }
            if (j == m - 1) {
                return i - j;
            }
        }
        return -1;
    }
}
