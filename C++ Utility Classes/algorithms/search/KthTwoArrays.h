#ifndef ALGORITHMS_SEARCH_KTHTWOARRAYS_H_
#define ALGORITHMS_SEARCH_KTHTWOARRAYS_H_

#include <bits/stdc++.h>

using namespace std;

template<typename Comparable>
class FindKth{
public:
    /**
     * Finds the kth smallest element of two sorted arrays (0-indexed) between 2 indices in each array (inclusive)
     *
     * @param A the first sorted array
     * @param aStart the starting index for the first array (inclusive)
     * @param aEnd the ending index for the first array (inclusive)
     * @param B the second sorted array
     * @param bStart the starting index for the second array (inclusive)
     * @param bEnd the ending index for the second array (inclusive)
     * @param K the element to look for
     * @return the kth smallest element of arrays A and B between aStart and aEnd in array A and bStart and bEnd in array B (0-indexed)
     */
    static Comparable findKth(Comparable *A, int aStart, int aEnd, Comparable *B, int bStart, int bEnd, int k) {
        int aLen = aEnd - aStart + 1;
        int bLen = bEnd - bStart + 1;
        if (aLen == 0) return B[bStart + k];
        if (bLen == 0) return A[aStart + k];
        if (k == 0) return min(A[aStart], B[bStart]);
        int aMid = aLen * k / (aLen + bLen) + aStart;
        int bMid = k - aMid + aStart + bStart - 1;
        if (A[aMid] > B[bMid]) {
            k = k - bMid + bStart - 1;
            aEnd = aMid;
            bStart = bMid + 1;
        } else {
            k = k - aMid + aStart - 1;
            bEnd = bMid;
            aStart = aMid + 1;
        }
        return findKth(A, aStart, aEnd, B, bStart, bEnd, k);
    }

    /**
     * Finds the kth smallest element of two sorted arrays (0-indexed)
     *
     * @param A the first sorted array
     * @param N the length of the first array
     * @param B the second sorted array
     * @param M the length of the second array
     * @param K the element to look for
     * @return the kth smallest element of arrays A and B (0-indexed)
     */
    static Comparable findKth(Comparable *A, int N, Comparable *B, int M, int k) {
        return findKth(A, 0, N - 1, B, 0, M - 1, k);
    }

    /**
     * Finds the median element of two sorted arrays
     *
     * @param A the first sorted array
     * @param N the length of the first array
     * @param B the second sorted array
     * @param M the length of the second array
     * @return the median element of arrays A and B
     */
    static Comparable findMedian(Comparable *A, int N, Comparable *B, int M) {
        if ((N + M) % 2 == 1) return findKth(A, 0, N - 1, B, 0, M - 1, (N + M) / 2);
        else return (findKth(A, 0, N - 1, B, 0, M - 1, (N + M) / 2 - 1) + findKth(A, 0, N - 1, B, 0, M - 1, (N + M) / 2)) / 2.0;
    }
};

#endif /* ALGORITHMS_SEARCH_KTHTWOARRAYS_H_ */
