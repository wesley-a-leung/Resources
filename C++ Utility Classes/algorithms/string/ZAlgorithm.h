#ifndef ALGORITHMS_STRING_ZALGORITHM_H_
#define ALGORITHMS_STRING_ZALGORITHM_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * The Z algorithm produces an array Z where Z[i] is the length of the longest substring starting from S[i] which is also a prefix of S.
 * I.E. the maximum k such that S[i + j] = S[j] for all 0 <= j < k
 *
 * Time complexity: O(N)
 */
class ZAlgorithm {
private:
    string text;
    int *z;

    void compute() {
        int l = 0, r = 0;
        for (int i = 1; i < text.length(); i++) {
            if (i > r) {
                l = r = i;
                while (r < text.length() && text[r] == text[r - l]) r++;
                r--;
                z[i] = r - l + 1;
            } else {
                int j = i - l;
                if (z[j] < r - i + 1) z[i] = z[j];
                else {
                    l = i;
                    while (r < text.length() && text[r] == text[r - l]) r++;
                    r--;
                    z[i] = r - l + 1;
                }
            }
        }
    }

public:
    ZAlgorithm(string text) {
        this->text = text;
        z = new int[text.length()];
        compute();
    }

    int getPrefixSubstringLength(int i) {
        return z[i];
    }

    int* getPrefixSubstringLengths() {
        return z;
    }
};

#endif /* ALGORITHMS_STRING_ZALGORITHM_H_ */
